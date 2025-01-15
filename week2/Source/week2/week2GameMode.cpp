// Copyright Epic Games, Inc. All Rights Reserved.

#include "week2GameMode.h"
#include "week2Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Aweek2TargetCube.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerInput.h"
#include "week2PickUpComponent.h"
#include "week2WeaponComponent.h"

Aweek2GameMode::Aweek2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

    // 设置初始状态
    bIsPickupWeapon = false;
    bIsGameRunning = false;
    bIsRoundInProgress = false;
    TotalScore = 0;
    RoundScore = 0;
    RoundCount = 0;

	// 默认方块数量
	TotalBlockCount = 11;
	ImportantBlockCount = 5;

	// 设置默认的方块类
	static ConstructorHelpers::FClassFinder<AAweek2TargetCube> TargetCubeBP(TEXT("/Game/_Game/Game/Target/BP_TargetCube2.BP_TargetCube2_C")); 
	if (TargetCubeBP.Succeeded())
	{
		TargetCubeClass = TargetCubeBP.Class;
	}
   
}

void Aweek2GameMode::BeginPlay()
{
	Super::BeginPlay();
    // 创建并初始化输入组件
    InputComponent = NewObject<UInputComponent>(this, TEXT("GameModeInputComponent"));
    InputComponent->RegisterComponent();

    // 确保输入组件绑定到玩家输入
    EnableInput(GetWorld()->GetFirstPlayerController());
    SaveInitial();
    StartRound();
}

//拾起枪代表开始游戏
void Aweek2GameMode::HandlePickupEvent()
{
    UE_LOG(LogTemp, Log, TEXT("Game started"));
    CountdownTimer();
}


//计时器
void Aweek2GameMode::CountdownTimer()
{
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,          // 计时器句柄
        this,                 // 所属的对象
        &Aweek2GameMode::OnTimerEnd, // 在计时结束时调用的函数
        20.0f,                // 时间间隔（20 秒）
        false                 // 是否循环（false 表示不循环）
    );

    // 可选：记录日志，确认计时器启动
    UE_LOG(LogTemp, Log, TEXT("20-second timer started"));
}

void Aweek2GameMode::OnTimerEnd()
{
    UE_LOG(LogTemp, Log, TEXT("Timer ended. Resetting game..."));
    PrintScores();

    ResetGame();
    StartRound();
    
}

//初始化记录游戏状态
void Aweek2GameMode::SaveInitial()
{

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetCubeClass, InitialCubes);

    // 存储每个方块的初始变换信息
    for (AActor* Cube : InitialCubes)
    {
        if (Cube)
        {
            InitialTransforms.Add(Cube->GetActorTransform());
        }
    }

    Aweek2Character* PlayerCharacter = Cast<Aweek2Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (PlayerCharacter)
    {
        // 记录角色的初始位置和旋转
        InitialCharacterLocation = PlayerCharacter->GetActorLocation();
        InitialCharacterRotation = PlayerCharacter->GetActorRotation();
    }

}

void Aweek2GameMode::ResetGame()
{
    // 首先删除当前的方块
    for (AActor* Cube : InitialCubes)
    {
        if (Cube && Cube->IsValidLowLevel())
        {
            Cube->Destroy();
        }
    }

    InitialCubes.Empty(); // 清空当前的方块数组

    // 重新生成初始方块
    for (const FTransform& Transform : InitialTransforms)
    {
        AActor* NewCube = GetWorld()->SpawnActor<AAweek2TargetCube>(TargetCubeClass, Transform);
        if (NewCube)
        {
            InitialCubes.Add(NewCube); // 更新数组
        }
    }
    GenerateRandomTargets();

    Aweek2Character* PlayerCharacter = Cast<Aweek2Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (PlayerCharacter)
    {
        // 重置角色位置和旋转
        PlayerCharacter->SetActorLocation(InitialCharacterLocation);
        PlayerCharacter->SetActorRotation(InitialCharacterRotation);
    }
    bIsRoundInProgress = false;
}


//设置重要方块
void Aweek2GameMode::GenerateRandomTargets()
{
    UE_LOG(LogTemp, Log, TEXT("start randomize"));
    
    if (!TargetCubeClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetCubeClass is not set!"));
        return;
    }

    // 获取场景中所有的方块
    TArray<AActor*> AllTargetCubes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetCubeClass, AllTargetCubes);

    // 已生成的重要方块数量（如果需要控制最大数量）
    int32 CurrentImportantBlockCount = 0;

    for (AActor* Actor : AllTargetCubes)
    {
        AAweek2TargetCube* TargetCube = Cast<AAweek2TargetCube>(Actor);
        if (TargetCube)
        {
            // 随机决定方块是否为重要
            bool bIsImportant = FMath::RandBool(); // 或者使用一些逻辑来决定重要性

            TargetCube->SetAsImportant(bIsImportant);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Randomized %d blocks (%d important, %d normal)."), AllTargetCubes.Num(), CurrentImportantBlockCount, AllTargetCubes.Num() - CurrentImportantBlockCount);
}


void Aweek2GameMode::StartRound()
{

    if (!bIsRoundInProgress)
    {
        bIsRoundInProgress = true;
        RoundCount++;  // 新的一轮
        // 每轮开始时重置得分
        RoundScore = 0;
        UE_LOG(LogTemp, Log, TEXT("Round %d start"), RoundCount);
        if (RoundCount == 1) {
            // 调用生成方块函数
            GenerateRandomTargets();
        }
        else if (RoundCount > 1 && RoundCount <= 10) {
            UE_LOG(LogTemp, Log, TEXT("Resting after 5 seconds"));
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle,          // 计时器句柄
                this,                 // 所属的对象
                &Aweek2GameMode::CountdownTimer, // 在计时结束时调用的函数
                5.0f,                // 时间间隔（20 秒）
                false                 // 是否循环（false 表示不循环）
            );
        }
        else if(RoundCount > 10)
        {
            UE_LOG(LogTemp, Log, TEXT("Game Ends"));
        }
    }
}

void Aweek2GameMode::PrintScores()
{
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    Aweek2Character* MyCharacter = Cast<Aweek2Character>(PlayerCharacter);
    if (MyCharacter)
    {
        int32 CurrentScore = MyCharacter->GetScore();
        RoundScore = CurrentScore;
    }
    MyCharacter->ResetScore();
    // 打印当前轮得分和总得分
    UE_LOG(LogTemp, Log, TEXT("Round %d score: %d"), RoundCount, RoundScore);
    TotalScore += RoundScore;
    UE_LOG(LogTemp, Log, TEXT("Total score after round %d: %d"), RoundCount, TotalScore);
}


