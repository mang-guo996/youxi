// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "UI/CQMainMenuWidget.h"
#include "Core/CQGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ContractQuadrant.h"

void UCQMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_NewGame)
	{
		Btn_NewGame->OnClicked.AddDynamic(this, &UCQMainMenuWidget::OnNewGameClicked);
	}
	if (Btn_Continue)
	{
		Btn_Continue->OnClicked.AddDynamic(this, &UCQMainMenuWidget::OnContinueClicked);
	}
	if (Btn_Settings)
	{
		Btn_Settings->OnClicked.AddDynamic(this, &UCQMainMenuWidget::OnSettingsClicked);
	}
	if (Btn_QuitGame)
	{
		Btn_QuitGame->OnClicked.AddDynamic(this, &UCQMainMenuWidget::OnQuitGameClicked);
	}

	if (Txt_GameTitle)
	{
		Txt_GameTitle->SetText(FText::FromString(TEXT("契约象限")));
	}

	RefreshSaveSlots();
}

void UCQMainMenuWidget::OnNewGameClicked()
{
	UE_LOG(LogContractQuadrant, Log, TEXT("开始新游戏"));

	// 重置GameInstance数据
	UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->PlayerName = TEXT("训练师");
		GI->PlayerGold = 500;
		GI->PartyBeasts.Empty();
		GI->Inventory.Empty();
		GI->CurrentArea = ECQWorldArea::CrystalTown;
	}

	// 打开初始场景
	UGameplayStatics::OpenLevel(this, FName("CrystalTown"));
}

void UCQMainMenuWidget::OnContinueClicked()
{
	UE_LOG(LogContractQuadrant, Log, TEXT("继续游戏"));
	// 显示存档选择UI
	// RefreshSaveSlots();
}

void UCQMainMenuWidget::OnSettingsClicked()
{
	UE_LOG(LogContractQuadrant, Log, TEXT("打开设置"));
	// TODO: 显示设置面板
}

void UCQMainMenuWidget::OnQuitGameClicked()
{
	UE_LOG(LogContractQuadrant, Log, TEXT("退出游戏"));
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UCQMainMenuWidget::LoadSaveSlot(int32 SlotIndex)
{
	UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
	if (GI && GI->LoadGame(SlotIndex))
	{
		// 根据存档区域打开对应关卡
		FString LevelName;
		switch (GI->CurrentArea)
		{
		case ECQWorldArea::CrystalTown:
			LevelName = TEXT("CrystalTown");
			break;
		case ECQWorldArea::PhantomForest:
			LevelName = TEXT("PhantomForest");
			break;
		case ECQWorldArea::MoltenCanyon:
			LevelName = TEXT("MoltenCanyon");
			break;
		}
		UGameplayStatics::OpenLevel(this, FName(*LevelName));
	}
}

void UCQMainMenuWidget::RefreshSaveSlots()
{
	UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
	if (!GI) return;

	// 检查各存档槽位
	for (int32 i = 0; i < UCQGameInstance::MaxSaveSlots; ++i)
	{
		bool bExists = GI->DoesSaveExist(i);
		UE_LOG(LogContractQuadrant, Log, TEXT("存档槽位 %d: %s"), i, bExists ? TEXT("有数据") : TEXT("空"));
	}

	// 如果没有任何存档，隐藏继续按钮
	bool bHasAnySave = false;
	for (int32 i = 0; i < UCQGameInstance::MaxSaveSlots; ++i)
	{
		if (GI->DoesSaveExist(i))
		{
			bHasAnySave = true;
			break;
		}
	}

	if (Btn_Continue)
	{
		Btn_Continue->SetIsEnabled(bHasAnySave);
	}
}
