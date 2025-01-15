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
	// 静态网格组件（方块的显示内容）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
	// 处理方块被命中的函数
	void HandleHit(AActor* InstigatorActor);


	//材质
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* NormalTargetMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* ImportantTargetMaterial;


	// 方块的分数值，击中后获得的积分
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	bool bIsImportantTarget;
	int32 ScoreValue;

	// 方块缩放倍数，被命中后变小
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	float ScaleFactor;

	void UpdateMaterial();

private:
	// 标记方块是否已被击中
	bool bHitOnce;
	
};
