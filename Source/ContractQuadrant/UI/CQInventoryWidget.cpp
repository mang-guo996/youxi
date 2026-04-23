// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "UI/CQInventoryWidget.h"
#include "Core/CQGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ContractQuadrant.h"

void UCQInventoryWidget::RefreshInventory()
{
	UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
	if (!GI) return;

	// 更新金币显示
	if (Txt_Gold)
	{
		Txt_Gold->SetText(FText::FromString(FString::Printf(TEXT("金币: %d"), GI->PlayerGold)));
	}

	// 物品列表的动态Widget创建由蓝图处理
	UE_LOG(LogContractQuadrant, Log, TEXT("刷新背包: %d 种物品"), GI->Inventory.Num());
}

void UCQInventoryWidget::UseItem(FName ItemID)
{
	UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
	if (!GI) return;

	if (GI->RemoveItem(ItemID, 1))
	{
		UE_LOG(LogContractQuadrant, Log, TEXT("使用物品: %s"), *ItemID.ToString());
		RefreshInventory();
	}
}

void UCQInventoryWidget::ShowItemDetail(const FCQItemData& ItemData, int32 Count)
{
	if (Txt_ItemName) Txt_ItemName->SetText(ItemData.ItemName);
	if (Txt_ItemDescription) Txt_ItemDescription->SetText(ItemData.Description);
	if (Txt_ItemCount) Txt_ItemCount->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Count)));
}
