// Fill out your copyright notice in the Description page of Project Settings.


#include "Aweek2TargetCube.h"
#include <week2/week2Character.h>


// Sets default values
AAweek2TargetCube::AAweek2TargetCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

    // ����һ����̬���������Ϊ�������ʾ
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComp->SetStaticMesh(CubeMesh.Object); // ���þ�̬����
    }

    // ���ز���
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

    // ��ʼ����ҪĿ���־
    bIsImportantTarget = false; // Ĭ�ϲ�����ҪĿ��

    // ���÷����Ĭ�Ϸ���
    ScoreValue = 10;
    ScaleFactor = 0.5f;
    bHitOnce = false;

    UpdateMaterial();
}

// Called when the game starts or when spawned
void AAweek2TargetCube::BeginPlay()
{
	Super::BeginPlay();
	
    // ��������ϱ�ǩ������ʶ��
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
            FinalScore *= 2; // ��ҪĿ��÷ּӱ�
        }
        PlayerCharacter->AddScore(FinalScore);
    }

    if (bHitOnce)
    {
        // ����Ѿ������й������ٷ���
        Destroy();
    }
    else
    {
        // ��һ�α�����ʱ�����ŷ���
        FVector CurrentScale = GetActorScale3D();
        SetActorScale3D(CurrentScale * ScaleFactor);
        bHitOnce = true;
    }
}

void AAweek2TargetCube::UpdateMaterial()
{
    if (bIsImportantTarget && ImportantTargetMaterial)
    {
        // ������̬����ʵ��
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ImportantTargetMaterial, this);
        MeshComp->SetMaterial(0, DynamicMaterial);
    }
    else if (NormalTargetMaterial)
    {
        // ������̬����ʵ��
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(NormalTargetMaterial, this);
        MeshComp->SetMaterial(0, DynamicMaterial);
    }
}

void AAweek2TargetCube::SetAsImportant(bool bIsImportant)
{
    bIsImportantTarget = bIsImportant;

    // ���²���
    UpdateMaterial();
}