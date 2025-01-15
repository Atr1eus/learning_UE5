// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "week2Character.h"
#include "week2GameMode.generated.h"


UCLASS(minimalapi)
class Aweek2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// 游戏状态标志
	bool bIsPickupWeapon; //是否拾起武器
	bool bIsGameRunning;      // 游戏是否正在进行
	bool bIsRoundInProgress;  // 当前轮是否进行中

	UPROPERTY(BlueprintReadWrite)
	int32 TotalScore;         // 总分

	int32 RoundScore;         // 当前轮分数
	int32 RoundCount;         // 轮数计数器
	FTimerHandle TimerHandle; // 用于管理计时器
	
	// 保存初始状态
	FVector InitialPlayerLocation;
	FRotator InitialPlayerRotation;
	TArray<AActor*> InitialCubes; // 存储初始的方块
	TArray<FTransform> InitialTransforms; // 存储初始方块的位置、旋转和缩放信息
	FVector InitialCharacterLocation; // 记录角色初始位置
	FRotator InitialCharacterRotation; // 记录角色初始旋转
	FVector InitialWeaponLocation; // 记录武器初始位置
	FRotator InitialWeaponRotation; // 记录武器初始旋转


	Aweek2GameMode();

	//virtual void StartPlay() override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandlePickupEvent();

	void CountdownTimer();

	UFUNCTION()
	void OnTimerEnd();

	UFUNCTION()
	void ResetGame();

	UFUNCTION()
	void  SaveInitial();

	// 随机生成方块的函数
	void GenerateRandomTargets();

	// 每轮倒计时20秒
	void StartRound();
	void PrintScores();

	//virtual void SetupInputComponent() override;

	// 方块的类（蓝图或 C++ 类）
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AAweek2TargetCube> TargetCubeClass;

	// 生成区域的边界
	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector2D MinBounds;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector2D MaxBounds;

	// 方块数量和重要方块比例
	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 TotalBlockCount;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 ImportantBlockCount;
};



