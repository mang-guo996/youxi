// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "UI/CQBattleWidget.h"
#include "Battle/CQBattleManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "ContractQuadrant.h"

void UCQBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 绑定按钮事件
	if (Btn_Attack) Btn_Attack->OnClicked.AddDynamic(this, &UCQBattleWidget::OnAttackClicked);
	if (Btn_Item) Btn_Item->OnClicked.AddDynamic(this, &UCQBattleWidget::OnItemClicked);
	if (Btn_Switch) Btn_Switch->OnClicked.AddDynamic(this, &UCQBattleWidget::OnSwitchClicked);
	if (Btn_Flee) Btn_Flee->OnClicked.AddDynamic(this, &UCQBattleWidget::OnFleeClicked);
	if (Btn_Capture) Btn_Capture->OnClicked.AddDynamic(this, &UCQBattleWidget::OnCaptureClicked);
	if (Btn_Skill1) Btn_Skill1->OnClicked.AddDynamic(this, &UCQBattleWidget::OnSkill1Clicked);
	if (Btn_Skill2) Btn_Skill2->OnClicked.AddDynamic(this, &UCQBattleWidget::OnSkill2Clicked);
	if (Btn_Skill3) Btn_Skill3->OnClicked.AddDynamic(this, &UCQBattleWidget::OnSkill3Clicked);
	if (Btn_Skill4) Btn_Skill4->OnClicked.AddDynamic(this, &UCQBattleWidget::OnSkill4Clicked);
	if (Btn_SkillBack) Btn_SkillBack->OnClicked.AddDynamic(this, &UCQBattleWidget::OnSkillBackClicked);

	ShowSkillPanel(false);
}

void UCQBattleWidget::BindToBattleManager(ACQBattleManager* BattleManager)
{
	BoundBattleManager = BattleManager;
	if (!BoundBattleManager) return;

	BoundBattleManager->OnBattlePhaseChanged.AddDynamic(this, &UCQBattleWidget::OnBattlePhaseChanged);
	BoundBattleManager->OnBattleMessage.AddDynamic(this, &UCQBattleWidget::OnBattleMessage);

	// 初始化显示
	UpdatePlayerBeastInfo(BoundBattleManager->GetPlayerActiveBeast());
	UpdateEnemyBeastInfo(BoundBattleManager->GetEnemyActiveBeast());

	// 设置捕获按钮可见性
	if (Btn_Capture)
	{
		Btn_Capture->SetVisibility(BoundBattleManager->IsWildBattle() ?
			ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UCQBattleWidget::UpdatePlayerBeastInfo(const FCQBeastInstanceData& BeastData)
{
	if (Txt_PlayerBeastName)
	{
		Txt_PlayerBeastName->SetText(FText::FromString(BeastData.BeastBaseID.ToString()));
	}
	if (Txt_PlayerBeastLevel)
	{
		Txt_PlayerBeastLevel->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), BeastData.Level)));
	}
	if (PB_PlayerHP)
	{
		float HPPercent = BeastData.CurrentStats.MaxHP > 0 ?
			static_cast<float>(BeastData.CurrentStats.CurrentHP) / BeastData.CurrentStats.MaxHP : 0.0f;
		PB_PlayerHP->SetPercent(HPPercent);
	}
	if (Txt_PlayerHP)
	{
		Txt_PlayerHP->SetText(FText::FromString(FString::Printf(
			TEXT("%d / %d"), BeastData.CurrentStats.CurrentHP, BeastData.CurrentStats.MaxHP)));
	}
}

void UCQBattleWidget::UpdateEnemyBeastInfo(const FCQBeastInstanceData& BeastData)
{
	if (Txt_EnemyBeastName)
	{
		Txt_EnemyBeastName->SetText(FText::FromString(BeastData.BeastBaseID.ToString()));
	}
	if (Txt_EnemyBeastLevel)
	{
		Txt_EnemyBeastLevel->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), BeastData.Level)));
	}
	if (PB_EnemyHP)
	{
		float HPPercent = BeastData.CurrentStats.MaxHP > 0 ?
			static_cast<float>(BeastData.CurrentStats.CurrentHP) / BeastData.CurrentStats.MaxHP : 0.0f;
		PB_EnemyHP->SetPercent(HPPercent);
	}
}

void UCQBattleWidget::AddBattleMessage(const FText& Message)
{
	BattleLogText += Message.ToString() + TEXT("\n");
	if (Txt_BattleLog)
	{
		Txt_BattleLog->SetText(FText::FromString(BattleLogText));
	}
}

void UCQBattleWidget::ShowActionMenu(bool bShow)
{
	if (ActionMenuPanel)
	{
		ActionMenuPanel->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UCQBattleWidget::ShowSkillPanel(bool bShow)
{
	if (SkillPanel)
	{
		SkillPanel->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	// 技能面板显示时隐藏行动菜单
	if (bShow)
	{
		ShowActionMenu(false);

		// 更新技能按钮文本
		if (BoundBattleManager)
		{
			TArray<FCQSkillInstance> Skills = BoundBattleManager->GetPlayerAvailableSkills();
			UButton* SkillButtons[] = { Btn_Skill1, Btn_Skill2, Btn_Skill3, Btn_Skill4 };

			for (int32 i = 0; i < 4; ++i)
			{
				if (SkillButtons[i])
				{
					if (i < Skills.Num())
					{
						SkillButtons[i]->SetVisibility(ESlateVisibility::Visible);
						SkillButtons[i]->SetIsEnabled(Skills[i].CurrentPP > 0);
					}
					else
					{
						SkillButtons[i]->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
			}
		}
	}
}

// ============================================================================
// 按钮回调
// ============================================================================

void UCQBattleWidget::OnAttackClicked() { ShowSkillPanel(true); }
void UCQBattleWidget::OnItemClicked() { UE_LOG(LogContractQuadrant, Log, TEXT("打开道具菜单")); }

void UCQBattleWidget::OnSwitchClicked()
{
	UE_LOG(LogContractQuadrant, Log, TEXT("打开切换晶兽菜单"));
}

void UCQBattleWidget::OnFleeClicked()
{
	if (BoundBattleManager)
	{
		FCQBattleAction FleeAction;
		FleeAction.ActionType = ECQBattleActionType::Flee;
		BoundBattleManager->PlayerSelectAction(FleeAction);
	}
}

void UCQBattleWidget::OnCaptureClicked()
{
	if (BoundBattleManager)
	{
		FCQBattleAction CaptureAction;
		CaptureAction.ActionType = ECQBattleActionType::Capture;
		CaptureAction.ItemID = FName("CrystalBall_Normal"); // 默认捕获道具
		BoundBattleManager->PlayerSelectAction(CaptureAction);
	}
}

void UCQBattleWidget::OnSkill1Clicked() { SubmitAttackAction(0); }
void UCQBattleWidget::OnSkill2Clicked() { SubmitAttackAction(1); }
void UCQBattleWidget::OnSkill3Clicked() { SubmitAttackAction(2); }
void UCQBattleWidget::OnSkill4Clicked() { SubmitAttackAction(3); }
void UCQBattleWidget::OnSkillBackClicked()
{
	ShowSkillPanel(false);
	ShowActionMenu(true);
}

void UCQBattleWidget::SubmitAttackAction(int32 SkillIndex)
{
	if (BoundBattleManager)
	{
		FCQBattleAction AttackAction;
		AttackAction.ActionType = ECQBattleActionType::Attack;
		AttackAction.SkillIndex = SkillIndex;
		BoundBattleManager->PlayerSelectAction(AttackAction);

		ShowSkillPanel(false);
	}
}

// ============================================================================
// 战斗事件响应
// ============================================================================

void UCQBattleWidget::OnBattlePhaseChanged(ECQBattlePhase NewPhase)
{
	switch (NewPhase)
	{
	case ECQBattlePhase::PlayerSelect:
		ShowActionMenu(true);
		break;
	case ECQBattlePhase::ActionExecution:
		ShowActionMenu(false);
		break;
	case ECQBattlePhase::BattleEnd:
		ShowActionMenu(false);
		break;
	default:
		break;
	}

	// 刷新晶兽信息
	if (BoundBattleManager)
	{
		UpdatePlayerBeastInfo(BoundBattleManager->GetPlayerActiveBeast());
		UpdateEnemyBeastInfo(BoundBattleManager->GetEnemyActiveBeast());
	}
}

void UCQBattleWidget::OnBattleMessage(const FText& Message)
{
	AddBattleMessage(Message);
}
