// Fill out your copyright notice in the Description page of Project Settings.


#include "Aweek2TargetCube.h"
#include <week2/week2Character.h>


// Sets default values
AAweek2TargetCube::AAweek2TargetCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

    // 创建一个静态网格组件作为方块的显示
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComp->SetStaticMesh(CubeMesh.Object); // 设置静态网格
    }

    // 加载材质
    static ConstructorHelpers::FObjectFinder<UMaterial> NormalMaterial(TEXT("/Game/StarterContent/Materials/M_Metal_Copper"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ImportantMaterial(TEXT("/Game/StarterContent/Materials/M_Metal_Gold"));

    if (NormalMaterial.Succeeded())
    {
        NormalTargetMaterial = NormalMaterial.Object;
    }

    if (ImportantMaterial.Succeeded())
    {
        ImportantTargetMaterial = ImportantMaterial.Object;
    }

    // 初始化重要目标标志
    bIsImportantTarget = false; // 默认不是重要目标

    // 设置方块的默认分数
    ScoreValue = 10;
    ScaleFactor = 0.5f;
    bHitOnce = false;

    UpdateMaterial();
}

// Called when the game starts or when spawned
void AAweek2TargetCube::BeginPlay()
{
	Super::BeginPlay();
	
    // 给方块加上标签，方便识别
    Tags.Add(FName("Target"));
}

// Called every frame
void AAweek2TargetCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAweek2TargetCube::HandleHit(AActor* InstigatorActor)
{
   
    if (!InstigatorActor)
    {
        UE_LOG(LogTemp, Error, TEXT("InstigatorActor is nullptr!"));
        return;
    }

    Aweek2Character* PlayerCharacter = Cast<Aweek2Character>(InstigatorActor);
    if (PlayerCharacter)
    {
        int32 FinalScore = ScoreValue;
        if (bIsImportantTarget)
        {
            FinalScore *= 2; // 重要目标得分加倍
        }
        PlayerCharacter->AddScore(FinalScore);
    }

    if (bHitOnce)
    {
        // 如果已经被击中过，销毁方块
        Destroy();
    }
    else
    {
        // 第一次被击中时，缩放方块
        FVector CurrentScale = GetActorScale3D();
        SetActorScale3D(CurrentScale * ScaleFactor);
        bHitOnce = true;
    }
}

void AAweek2TargetCube::UpdateMaterial()
{
    if (bIsImportantTarget && ImportantTargetMaterial)
    {
        // 创建动态材质实例
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ImportantTargetMaterial, this);
        MeshComp->SetMaterial(0, DynamicMaterial);
    }
    else if (NormalTargetMaterial)
    {
        // 创建动态材质实例
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(NormalTargetMaterial, this);
        MeshComp->SetMaterial(0, DynamicMaterial);
    }
}

void AAweek2TargetCube::SetAsImportant(bool bIsImportant)
{
    bIsImportantTarget = bIsImportant;

    // 更新材质
    UpdateMaterial();
}