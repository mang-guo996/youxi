// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Core/CQGameInstance.h"
#include "Core/CQSaveGame.h"
#include "Quest/CQQuestManager.h"
#include "CrystalBeast/CQBeastDataTypes.h"
#include "Kismet/GameplayStatics.h"
#include "ContractQuadrant.h"

UCQGameInstance::UCQGameInstance()
{
}

void UCQGameInstance::Init()
{
	Super::Init();
	InitQuestManager();

	UE_LOG(LogContractQuadrant, Log, TEXT("游戏实例初始化完成"));
}

// ============================================================================
// 存档系统
// ============================================================================

bool UCQGameInstance::SaveGame(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= MaxSaveSlots)
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("无效的存档槽位: %d"), SlotIndex);
		return false;
	}

	UCQSaveGame* SaveGameInstance = Cast<UCQSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UCQSaveGame::StaticClass()));

	if (!SaveGameInstance)
	{
		return false;
	}

	// 保存玩家基础数据
	SaveGameInstance->PlayerName = PlayerName;
	SaveGameInstance->PlayerGold = PlayerGold;
	SaveGameInstance->Inventory = Inventory;
	SaveGameInstance->CurrentArea = CurrentArea;
	SaveGameInstance->UnlockedTeleportPoints = UnlockedTeleportPoints;

	// 保存晶兽数据
	SaveGameInstance->PartyBeasts = PartyBeasts;
	SaveGameInstance->StorageBeasts = StorageBeasts;
	SaveGameInstance->DiscoveredBeastIDs = DiscoveredBeastIDs;

	// 保存任务进度
	if (QuestManager)
	{
		SaveGameInstance->CompletedQuestIDs = QuestManager->GetCompletedQuestIDs();
		SaveGameInstance->ActiveQuestIDs = QuestManager->GetActiveQuestIDs();
	}

	// 保存玩家位置
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC && PC->GetPawn())
	{
		SaveGameInstance->PlayerLocation = PC->GetPawn()->GetActorLocation();
		SaveGameInstance->PlayerRotation = PC->GetPawn()->GetActorRotation();
	}

	// 写入存档
	FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);
	bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);

	if (bSuccess)
	{
		UE_LOG(LogContractQuadrant, Log, TEXT("游戏存档成功: 槽位 %d"), SlotIndex);
	}

	return bSuccess;
}

bool UCQGameInstance::LoadGame(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= MaxSaveSlots)
	{
		return false;
	}

	FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);

	UCQSaveGame* SaveGameInstance = Cast<UCQSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (!SaveGameInstance)
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("加载存档失败: 槽位 %d"), SlotIndex);
		return false;
	}

	// 恢复玩家数据
	PlayerName = SaveGameInstance->PlayerName;
	PlayerGold = SaveGameInstance->PlayerGold;
	Inventory = SaveGameInstance->Inventory;
	CurrentArea = SaveGameInstance->CurrentArea;
	UnlockedTeleportPoints = SaveGameInstance->UnlockedTeleportPoints;

	// 恢复晶兽数据
	PartyBeasts = SaveGameInstance->PartyBeasts;
	StorageBeasts = SaveGameInstance->StorageBeasts;
	DiscoveredBeastIDs = SaveGameInstance->DiscoveredBeastIDs;

	// 恢复任务进度
	if (QuestManager)
	{
		QuestManager->LoadQuestProgress(
			SaveGameInstance->CompletedQuestIDs,
			SaveGameInstance->ActiveQuestIDs);
	}

	UE_LOG(LogContractQuadrant, Log, TEXT("游戏加载成功: 槽位 %d"), SlotIndex);
	return true;
}

bool UCQGameInstance::DoesSaveExist(int32 SlotIndex) const
{
	FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

bool UCQGameInstance::DeleteSave(int32 SlotIndex)
{
	FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);
	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

// ============================================================================
// 背包系统
// ============================================================================

void UCQGameInstance::AddItem(FName ItemID, int32 Count)
{
	if (Count <= 0) return;

	// 查找已有物品
	for (FCQInventoryItem& Item : Inventory)
	{
		if (Item.ItemID == ItemID)
		{
			Item.Count += Count;
			return;
		}
	}

	// 新物品
	FCQInventoryItem NewItem;
	NewItem.ItemID = ItemID;
	NewItem.Count = Count;
	Inventory.Add(NewItem);
}

bool UCQGameInstance::RemoveItem(FName ItemID, int32 Count)
{
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i].ItemID == ItemID)
		{
			if (Inventory[i].Count < Count)
			{
				return false; // 数量不足
			}

			Inventory[i].Count -= Count;
			if (Inventory[i].Count <= 0)
			{
				Inventory.RemoveAt(i);
			}
			return true;
		}
	}
	return false; // 物品不存在
}

int32 UCQGameInstance::GetItemCount(FName ItemID) const
{
	for (const FCQInventoryItem& Item : Inventory)
	{
		if (Item.ItemID == ItemID)
		{
			return Item.Count;
		}
	}
	return 0;
}

void UCQGameInstance::AddGold(int32 Amount)
{
	PlayerGold = FMath::Max(0, PlayerGold + Amount);
}

// ============================================================================
// 内部方法
// ============================================================================

void UCQGameInstance::InitQuestManager()
{
	QuestManager = NewObject<UCQQuestManager>(this, UCQQuestManager::StaticClass());
	if (QuestManager)
	{
		QuestManager->Initialize();
	}
}
