// Copyright (C) 2025 契约象限. All Rights Reserved.
// 探索模式HUD

#pragma once

#include "CoreMinimal.h"
#include "UI/CQWidgetBase.h"
#include "CQHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class CONTRACTQUADRANT_API UCQHUDWidget : public UCQWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 更新当前任务提示
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateQuestHint(const FText& QuestName, const FText& ObjectiveText);

	// 更新区域名称
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateAreaName(const FText& AreaName);

	// 更新队伍状态
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdatePartyStatus(int32 PartySize, int32 AliveBeastCount);

	// 显示交互提示
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowInteractPrompt(const FText& PromptText);

	// 隐藏交互提示
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideInteractPrompt();

	// 显示通知消息（带自动消失）
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowNotification(const FText& Message, float Duration = 3.0f);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_CurrentQuestName;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_CurrentObjective;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_AreaName;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_PartyStatus;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_InteractPrompt;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_Notification;

	FTimerHandle NotificationTimerHandle;

	UFUNCTION()
	void HideNotification();
};
