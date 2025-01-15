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
	// ��Ϸ״̬��־
	bool bIsPickupWeapon; //�Ƿ�ʰ������
	bool bIsGameRunning;      // ��Ϸ�Ƿ����ڽ���
	bool bIsRoundInProgress;  // ��ǰ���Ƿ������

	UPROPERTY(BlueprintReadWrite)
	int32 TotalScore;         // �ܷ�

	int32 RoundScore;         // ��ǰ�ַ���
	int32 RoundCount;         // ����������
	FTimerHandle TimerHandle; // ���ڹ����ʱ��
	
	// �����ʼ״̬
	FVector InitialPlayerLocation;
	FRotator InitialPlayerRotation;
	TArray<AActor*> InitialCubes; // �洢��ʼ�ķ���
	TArray<FTransform> InitialTransforms; // �洢��ʼ�����λ�á���ת��������Ϣ
	FVector InitialCharacterLocation; // ��¼��ɫ��ʼλ��
	FRotator InitialCharacterRotation; // ��¼��ɫ��ʼ��ת
	FVector InitialWeaponLocation; // ��¼������ʼλ��
	FRotator InitialWeaponRotation; // ��¼������ʼ��ת


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

	// ������ɷ���ĺ���
	void GenerateRandomTargets();

	// ÿ�ֵ���ʱ20��
	void StartRound();
	void PrintScores();

	//virtual void SetupInputComponent() override;

	// ������ࣨ��ͼ�� C++ �ࣩ
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AAweek2TargetCube> TargetCubeClass;

	// ��������ı߽�
	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector2D MinBounds;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector2D MaxBounds;

	// ������������Ҫ�������
	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 TotalBlockCount;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 ImportantBlockCount;
};



