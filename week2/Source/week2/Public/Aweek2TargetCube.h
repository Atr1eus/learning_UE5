// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Aweek2TargetCube.generated.h"

class UStaticMeshComponent;
class UMaterial;
class UMaterialInstanceDynamic;

UCLASS()
class WEEK2_API AAweek2TargetCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAweek2TargetCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetAsImportant(bool bIsImportant);
	//Add
	// 
	// ��̬����������������ʾ���ݣ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
	// �����鱻���еĺ���
	void HandleHit(AActor* InstigatorActor);


	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* NormalTargetMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* ImportantTargetMaterial;


	// ����ķ���ֵ�����к��õĻ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	bool bIsImportantTarget;
	int32 ScoreValue;

	// �������ű����������к��С
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	float ScaleFactor;

	void UpdateMaterial();

private:
	// ��Ƿ����Ƿ��ѱ�����
	bool bHitOnce;
	
};
