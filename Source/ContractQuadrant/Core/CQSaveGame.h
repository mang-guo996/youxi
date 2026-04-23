// Copyright (C) 2025 契约象限. All Rights Reserved.
// 存档数据类 - 可序列化的游戏存档

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Core/CQTypes.h"
#include "CrystalBeast/CQBeastDataTypes.h"
#include "CQSaveGame.generated.h"

UCLASS()
class CONTRACTQUADRANT_API UCQSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UCQSaveGame();

	// ========== 玩家基础信息 ==========

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 PlayerGold;

	UPROPERTY()
	TArray<FCQInventoryItem> Inventory;

	UPROPERTY()
	ECQWorldArea CurrentArea;

	UPROPERTY()
	TArray<FName> UnlockedTeleportPoints;

	// ========== 玩家位置 ==========

	UPROPERTY()
	FVector PlayerLocation;

	UPROPERTY()
	FRotator PlayerRotation;

	// ========== 晶兽数据 ==========

	UPROPERTY()
	TArray<FCQBeastInstanceData> PartyBeasts;

	UPROPERTY()
	TArray<FCQBeastInstanceData> StorageBeasts;

	UPROPERTY()
	TArray<FName> DiscoveredBeastIDs;

	// ========== 任务进度 ==========

	UPROPERTY()
	TArray<FName> CompletedQuestIDs;

	UPROPERTY()
	TArray<FName> ActiveQuestIDs;

	// ========== 元数据 ==========

	UPROPERTY()
	FDateTime SaveDateTime;

	UPROPERTY()
	float PlayTimeSeconds;

	UPROPERTY()
	FString SaveVersion;
};
