// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "UI/CQBeastCollectionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "ContractQuadrant.h"

void UCQBeastCollectionWidget::RefreshCollection()
{
	// 具体的列表项创建由蓝图处理（动态Widget生成）
	UE_LOG(LogContractQuadrant, Log, TEXT("刷新晶兽图鉴"));
}

void UCQBeastCollectionWidget::ShowBeastDetail(const FCQBeastBaseData& BaseData, bool bDiscovered)
{
	if (Txt_BeastName)
	{
		Txt_BeastName->SetText(bDiscovered ? BaseData.BeastName : FText::FromString(TEXT("???")));
	}

	if (Txt_BeastDescription)
	{
		Txt_BeastDescription->SetText(bDiscovered ? BaseData.Description : FText::FromString(TEXT("尚未发现")));
	}

	if (Txt_BeastRarity)
	{
		FString RarityStr;
		switch (BaseData.Rarity)
		{
		case ECQBeastRarity::Common: RarityStr = TEXT("★"); break;
		case ECQBeastRarity::Uncommon: RarityStr = TEXT("★★"); break;
		case ECQBeastRarity::Rare: RarityStr = TEXT("★★★"); break;
		case ECQBeastRarity::Epic: RarityStr = TEXT("★★★★"); break;
		case ECQBeastRarity::Legendary: RarityStr = TEXT("★★★★★"); break;
		}
		Txt_BeastRarity->SetText(FText::FromString(RarityStr));
	}

	// 更新属性条（归一化到0-1范围，最大值假定为150）
	const float MaxStatValue = 150.0f;
	if (bDiscovered)
	{
		if (PB_HP) PB_HP->SetPercent(BaseData.BaseStats.MaxHP / MaxStatValue);
		if (PB_Attack) PB_Attack->SetPercent(BaseData.BaseStats.Attack / MaxStatValue);
		if (PB_Defense) PB_Defense->SetPercent(BaseData.BaseStats.Defense / MaxStatValue);
		if (PB_Speed) PB_Speed->SetPercent(BaseData.BaseStats.Speed / MaxStatValue);
	}
	else
	{
		if (PB_HP) PB_HP->SetPercent(0.0f);
		if (PB_Attack) PB_Attack->SetPercent(0.0f);
		if (PB_Defense) PB_Defense->SetPercent(0.0f);
		if (PB_Speed) PB_Speed->SetPercent(0.0f);
	}
}
