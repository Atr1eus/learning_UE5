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

    // ���ó�ʼ״̬
    bIsPickupWeapon = false;
    bIsGameRunning = false;
    bIsRoundInProgress = false;
    TotalScore = 0;
    RoundScore = 0;
    RoundCount = 0;

	// Ĭ�Ϸ�������
	TotalBlockCount = 11;
	ImportantBlockCount = 5;

	// ����Ĭ�ϵķ�����
	static ConstructorHelpers::FClassFinder<AAweek2TargetCube> TargetCubeBP(TEXT("/Game/_Game/Game/Target/BP_TargetCube2.BP_TargetCube2_C")); 
	if (TargetCubeBP.Succeeded())
	{
		TargetCubeClass = TargetCubeBP.Class;
	}
   
}

void Aweek2GameMode::BeginPlay()
{
	Super::BeginPlay();
    // ��������ʼ���������
    InputComponent = NewObject<UInputComponent>(this, TEXT("GameModeInputComponent"));
    InputComponent->RegisterComponent();

    // ȷ����������󶨵��������
    EnableInput(GetWorld()->GetFirstPlayerController());
    SaveInitial();
    StartRound();
}

//ʰ��ǹ����ʼ��Ϸ
void Aweek2GameMode::HandlePickupEvent()
{
    UE_LOG(LogTemp, Log, TEXT("Game started"));
    CountdownTimer();
}


//��ʱ��
void Aweek2GameMode::CountdownTimer()
{
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,          // ��ʱ�����
        this,                 // �����Ķ���
        &Aweek2GameMode::OnTimerEnd, // �ڼ�ʱ����ʱ���õĺ���
        20.0f,                // ʱ������20 �룩
        false                 // �Ƿ�ѭ����false ��ʾ��ѭ����
    );

    // ��ѡ����¼��־��ȷ�ϼ�ʱ������
    UE_LOG(LogTemp, Log, TEXT("20-second timer started"));
}

void Aweek2GameMode::OnTimerEnd()
{
    UE_LOG(LogTemp, Log, TEXT("Timer ended. Resetting game..."));
    PrintScores();

    ResetGame();
    StartRound();
    
}

//��ʼ����¼��Ϸ״̬
void Aweek2GameMode::SaveInitial()
{

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetCubeClass, InitialCubes);

    // �洢ÿ������ĳ�ʼ�任��Ϣ
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
        // ��¼��ɫ�ĳ�ʼλ�ú���ת
        InitialCharacterLocation = PlayerCharacter->GetActorLocation();
        InitialCharacterRotation = PlayerCharacter->GetActorRotation();
    }

}

void Aweek2GameMode::ResetGame()
{
    // ����ɾ����ǰ�ķ���
    for (AActor* Cube : InitialCubes)
    {
        if (Cube && Cube->IsValidLowLevel())
        {
            Cube->Destroy();
        }
    }

    InitialCubes.Empty(); // ��յ�ǰ�ķ�������

    // �������ɳ�ʼ����
    for (const FTransform& Transform : InitialTransforms)
    {
        AActor* NewCube = GetWorld()->SpawnActor<AAweek2TargetCube>(TargetCubeClass, Transform);
        if (NewCube)
        {
            InitialCubes.Add(NewCube); // ��������
        }
    }
    GenerateRandomTargets();

    Aweek2Character* PlayerCharacter = Cast<Aweek2Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (PlayerCharacter)
    {
        // ���ý�ɫλ�ú���ת
        PlayerCharacter->SetActorLocation(InitialCharacterLocation);
        PlayerCharacter->SetActorRotation(InitialCharacterRotation);
    }
    bIsRoundInProgress = false;
}


//������Ҫ����
void Aweek2GameMode::GenerateRandomTargets()
{
    UE_LOG(LogTemp, Log, TEXT("start randomize"));
    
    if (!TargetCubeClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetCubeClass is not set!"));
        return;
    }

    // ��ȡ���������еķ���
    TArray<AActor*> AllTargetCubes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetCubeClass, AllTargetCubes);

    // �����ɵ���Ҫ���������������Ҫ�������������
    int32 CurrentImportantBlockCount = 0;

    for (AActor* Actor : AllTargetCubes)
    {
        AAweek2TargetCube* TargetCube = Cast<AAweek2TargetCube>(Actor);
        if (TargetCube)
        {
            // ������������Ƿ�Ϊ��Ҫ
            bool bIsImportant = FMath::RandBool(); // ����ʹ��һЩ�߼���������Ҫ��

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
        RoundCount++;  // �µ�һ��
        // ÿ�ֿ�ʼʱ���õ÷�
        RoundScore = 0;
        UE_LOG(LogTemp, Log, TEXT("Round %d start"), RoundCount);
        if (RoundCount == 1) {
            // �������ɷ��麯��
            GenerateRandomTargets();
        }
        else if (RoundCount > 1 && RoundCount <= 10) {
            UE_LOG(LogTemp, Log, TEXT("Resting after 5 seconds"));
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle,          // ��ʱ�����
                this,                 // �����Ķ���
                &Aweek2GameMode::CountdownTimer, // �ڼ�ʱ����ʱ���õĺ���
                5.0f,                // ʱ������20 �룩
                false                 // �Ƿ�ѭ����false ��ʾ��ѭ����
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
    // ��ӡ��ǰ�ֵ÷ֺ��ܵ÷�
    UE_LOG(LogTemp, Log, TEXT("Round %d score: %d"), RoundCount, RoundScore);
    TotalScore += RoundScore;
    UE_LOG(LogTemp, Log, TEXT("Total score after round %d: %d"), RoundCount, TotalScore);
}


