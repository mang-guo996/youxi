// Copyright (C) 2025 契约象限. All Rights Reserved.
// 对话界面Widget

#pragma once

#include "CoreMinimal.h"
#include "UI/CQWidgetBase.h"
#include "Quest/CQDialogueTypes.h"
#include "CQDialogueWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;
class UVerticalBox;

UCLASS()
class CONTRACTQUADRANT_API UCQDialogueWidget : public UCQWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 更新对话内容
	UFUNCTION(BlueprintCallable, Category = "对话UI")
	void UpdateDialogueDisplay(const FCQDialogueNode& DialogueNode);

	// 显示选项
	UFUNCTION(BlueprintCallable, Category = "对话UI")
	void ShowOptions(const TArray<FCQDialogueOption>& Options);

	// 清除选项
	UFUNCTION(BlueprintCallable, Category = "对话UI")
	void ClearOptions();

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_SpeakerName;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_DialogueText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Img_SpeakerPortrait;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> OptionsContainer;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Continue;

	// 继续按钮回调
	UFUNCTION()
	void OnContinueClicked();

	// 打字机效果相关
	UPROPERTY(EditDefaultsOnly, Category = "打字机")
	float TypewriterSpeed = 0.03f;

	FString FullDialogueText;
	int32 CurrentCharIndex = 0;
	FTimerHandle TypewriterTimerHandle;

	void TypewriterTick();
	void FinishTypewriter();
};
