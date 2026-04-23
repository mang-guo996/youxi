// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Core/CQGameMode.h"
#include "Core/CQGameInstance.h"
#include "Core/CQPlayerController.h"
#include "Core/CQPlayerCharacter.h"
#include "Core/CQGameState.h"
#include "Battle/CQBattleManager.h"
#include "Kismet/GameplayStatics.h"
#include "ContractQuadrant.h"

ACQGameMode::ACQGameMode()
{
	// 设置默认类
	DefaultPawnClass = ACQPlayerCharacter::StaticClass();
	PlayerControllerClass = ACQPlayerController::StaticClass();
	GameStateClass = ACQGameState::StaticClass();
}

void ACQGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetGamePlayMode(ECQGamePlayMode::Exploration);

	UE_LOG(LogContractQuadrant, Log, TEXT("游戏模式开始运行"));
}

// ============================================================================
// 游戏模式切换
// ============================================================================

void ACQGameMode::SetGamePlayMode(ECQGamePlayMode NewMode)
{
	if (CurrentGamePlayMode == NewMode)
	{
		return;
	}

	ECQGamePlayMode OldMode = CurrentGamePlayMode;
	CurrentGamePlayMode = NewMode;

	// 通知所有监听者
	OnGamePlayModeChanged.Broadcast(NewMode);

	UE_LOG(LogContractQuadrant, Log, TEXT("游戏模式切换: %d -> %d"),
		static_cast<int32>(OldMode), static_cast<int32>(NewMode));
}

// ============================================================================
// 战斗系统
// ============================================================================

void ACQGameMode::StartWildBattle(FName WildBeastID, int32 BeastLevel)
{
	if (CurrentGamePlayMode == ECQGamePlayMode::Battle)
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("已在战斗中，无法开始新战斗"));
		return;
	}

	// 切换到战斗模式
	SetGamePlayMode(ECQGamePlayMode::Battle);

	// 生成战斗管理器
	if (BattleManagerClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		CurrentBattleManager = GetWorld()->SpawnActor<ACQBattleManager>(
			BattleManagerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (CurrentBattleManager)
		{
			// 获取玩家晶兽队伍
			UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
			if (GI)
			{
				CurrentBattleManager->InitWildBattle(GI->PartyBeasts, WildBeastID, BeastLevel);
			}
		}
	}
}

void ACQGameMode::StartTrainerBattle(const TArray<FName>& EnemyBeastIDs, const TArray<int32>& EnemyBeastLevels)
{
	if (CurrentGamePlayMode == ECQGamePlayMode::Battle)
	{
		return;
	}

	SetGamePlayMode(ECQGamePlayMode::Battle);

	if (BattleManagerClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		CurrentBattleManager = GetWorld()->SpawnActor<ACQBattleManager>(
			BattleManagerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (CurrentBattleManager)
		{
			UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
			if (GI)
			{
				CurrentBattleManager->InitTrainerBattle(GI->PartyBeasts, EnemyBeastIDs, EnemyBeastLevels);
			}
		}
	}
}

void ACQGameMode::EndBattle()
{
	if (CurrentBattleManager)
	{
		CurrentBattleManager->Destroy();
		CurrentBattleManager = nullptr;
	}

	SetGamePlayMode(ECQGamePlayMode::Exploration);
}

// ============================================================================
// 场景管理
// ============================================================================

void ACQGameMode::TravelToArea(ECQWorldArea TargetArea)
{
	if (const TSoftObjectPtr<UWorld>* MapPtr = AreaMaps.Find(TargetArea))
	{
		FString MapPath = MapPtr->ToString();
		if (!MapPath.IsEmpty())
		{
			// 更新GameInstance中的区域记录
			UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
			if (GI)
			{
				GI->CurrentArea = TargetArea;
			}

			UGameplayStatics::OpenLevel(this, FName(*MapPath));
			UE_LOG(LogContractQuadrant, Log, TEXT("切换场景至: %s"), *MapPath);
		}
	}
}
