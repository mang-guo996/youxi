// Copyright (C) 2025 契约象限. All Rights Reserved.
// 游戏实例 - 持久化数据管理，跨关卡保持

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Core/CQTypes.h"
#include "CQGameInstance.generated.h"

class UCQQuestManager;
class UCQSaveGame;
struct FCQBeastInstanceData;

UCLASS()
class CONTRACTQUADRANT_API UCQGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCQGameInstance();

	virtual void Init() override;

	// ========== 存档系统 ==========

	// 保存游戏
	UFUNCTION(BlueprintCallable, Category = "存档")
	bool SaveGame(int32 SlotIndex = 0);

	// 加载游戏
	UFUNCTION(BlueprintCallable, Category = "存档")
	bool LoadGame(int32 SlotIndex = 0);

	// 检查存档是否存在
	UFUNCTION(BlueprintCallable, Category = "存档")
	bool DoesSaveExist(int32 SlotIndex = 0) const;

	// 删除存档
	UFUNCTION(BlueprintCallable, Category = "存档")
	bool DeleteSave(int32 SlotIndex = 0);

	// ========== 玩家数据 ==========

	// 玩家名称
	UPROPERTY(BlueprintReadWrite, Category = "玩家数据")
	FString PlayerName = TEXT("训练师");

	// 玩家金币
	UPROPERTY(BlueprintReadWrite, Category = "玩家数据")
	int32 PlayerGold = 500;

	// 背包物品列表
	UPROPERTY(BlueprintReadWrite, Category = "玩家数据")
	TArray<FCQInventoryItem> Inventory;

	// 当前所在区域
	UPROPERTY(BlueprintReadWrite, Category = "玩家数据")
	ECQWorldArea CurrentArea = ECQWorldArea::CrystalTown;

	// 已解锁的传送点
	UPROPERTY(BlueprintReadWrite, Category = "玩家数据")
	TArray<FName> UnlockedTeleportPoints;

	// ========== 晶兽数据（跨关卡持久化）==========

	// 当前队伍中的晶兽实例数据
	UPROPERTY(BlueprintReadWrite, Category = "晶兽")
	TArray<FCQBeastInstanceData> PartyBeasts;

	// 仓库中的晶兽
	UPROPERTY(BlueprintReadWrite, Category = "晶兽")
	TArray<FCQBeastInstanceData> StorageBeasts;

	// 已发现的晶兽ID列表（图鉴）
	UPROPERTY(BlueprintReadWrite, Category = "晶兽")
	TArray<FName> DiscoveredBeastIDs;

	// ========== 任务管理器 ==========

	UPROPERTY(BlueprintReadOnly, Category = "任务")
	TObjectPtr<UCQQuestManager> QuestManager;

	// ========== 便捷方法 ==========

	// 添加物品到背包
	UFUNCTION(BlueprintCallable, Category = "背包")
	void AddItem(FName ItemID, int32 Count = 1);

	// 移除物品
	UFUNCTION(BlueprintCallable, Category = "背包")
	bool RemoveItem(FName ItemID, int32 Count = 1);

	// 检查物品数量
	UFUNCTION(BlueprintCallable, Category = "背包")
	int32 GetItemCount(FName ItemID) const;

	// 添加金币
	UFUNCTION(BlueprintCallable, Category = "经济")
	void AddGold(int32 Amount);

	// 最大存档槽位数
	static constexpr int32 MaxSaveSlots = 3;

private:
	// 创建/获取任务管理器
	void InitQuestManager();
};
