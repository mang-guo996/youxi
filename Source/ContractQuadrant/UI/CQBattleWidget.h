// Copyright (C) 2025 契约象限. All Rights Reserved.
// 战斗界面Widget

#pragma once

#include "CoreMinimal.h"
#include "UI/CQWidgetBase.h"
#include "Battle/CQBattleTypes.h"
#include "CQBattleWidget.generated.h"

class UButton;
class UTextBlock;
class UProgressBar;
class UVerticalBox;
class UHorizontalBox;
class ACQBattleManager;

UCLASS()
class CONTRACTQUADRANT_API UCQBattleWidget : public UCQWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 绑定战斗管理器事件
	UFUNCTION(BlueprintCallable, Category = "战斗UI")
	void BindToBattleManager(ACQBattleManager* BattleManager);

	// 更新玩家晶兽显示
	UFUNCTION(BlueprintCallable, Category = "战斗UI")
	void UpdatePlayerBeastInfo(const FCQBeastInstanceData& BeastData);

	// 更新敌方晶兽显示
	UFUNCTION(BlueprintCallable, Category = "战斗UI")
	void UpdateEnemyBeastInfo(const FCQBeastInstanceData& BeastData);

	// 添加战斗日志消息
	UFUNCTION(BlueprintCallable, Category = "战斗UI")
	void AddBattleMessage(const FText& Message);

	// 显示/隐藏行动菜单
	UFUNCTION(BlueprintCallable, Category = "战斗UI")
	void ShowActionMenu(bool bShow);

	// 显示/隐藏技能选择面板
	UFUNCTION(BlueprintCallable, Category = "战斗UI")
	void ShowSkillPanel(bool bShow);

protected:
	// ========== 玩家晶兽信息 ==========
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_PlayerBeastName;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_PlayerBeastLevel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> PB_PlayerHP;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_PlayerHP;

	// ========== 敌方晶兽信息 ==========
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_EnemyBeastName;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_EnemyBeastLevel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> PB_EnemyHP;

	// ========== 行动菜单 ==========
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> ActionMenuPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Attack;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Item;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Switch;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Flee;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Capture;

	// ========== 技能面板 ==========
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> SkillPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Skill1;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Skill2;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Skill3;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Skill4;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_SkillBack;

	// ========== 战斗日志 ==========
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_BattleLog;

	// ========== 回调 ==========
	UFUNCTION() void OnAttackClicked();
	UFUNCTION() void OnItemClicked();
	UFUNCTION() void OnSwitchClicked();
	UFUNCTION() void OnFleeClicked();
	UFUNCTION() void OnCaptureClicked();
	UFUNCTION() void OnSkill1Clicked();
	UFUNCTION() void OnSkill2Clicked();
	UFUNCTION() void OnSkill3Clicked();
	UFUNCTION() void OnSkill4Clicked();
	UFUNCTION() void OnSkillBackClicked();

	// 选择技能后提交行动
	void SubmitAttackAction(int32 SkillIndex);

	// 战斗事件响应
	UFUNCTION()
	void OnBattlePhaseChanged(ECQBattlePhase NewPhase);

	UFUNCTION()
	void OnBattleMessage(const FText& Message);

	UPROPERTY()
	TObjectPtr<ACQBattleManager> BoundBattleManager;

	// 战斗日志文本累积
	FString BattleLogText;
};
