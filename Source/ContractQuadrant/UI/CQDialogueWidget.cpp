// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "UI/CQDialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "TimerManager.h"
#include "ContractQuadrant.h"

void UCQDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Continue)
	{
		Btn_Continue->OnClicked.AddDynamic(this, &UCQDialogueWidget::OnContinueClicked);
	}
}

void UCQDialogueWidget::UpdateDialogueDisplay(const FCQDialogueNode& DialogueNode)
{
	// 更新说话人名称
	if (Txt_SpeakerName)
	{
		Txt_SpeakerName->SetText(DialogueNode.SpeakerName);
	}

	// 更新头像
	if (Img_SpeakerPortrait && DialogueNode.SpeakerPortrait.IsValid())
	{
		// 异步加载头像纹理
		// Img_SpeakerPortrait->SetBrushFromTexture(LoadedTexture);
	}

	// 启动打字机效果
	FullDialogueText = DialogueNode.DialogueText.ToString();
	CurrentCharIndex = 0;

	if (Txt_DialogueText)
	{
		Txt_DialogueText->SetText(FText::GetEmpty());
	}

	// 清除之前的选项
	ClearOptions();

	// 隐藏继续按钮，直到打字完成
	if (Btn_Continue)
	{
		Btn_Continue->SetVisibility(ESlateVisibility::Hidden);
	}

	// 开始打字机定时器
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TypewriterTimerHandle);
		World->GetTimerManager().SetTimer(
			TypewriterTimerHandle,
			this,
			&UCQDialogueWidget::TypewriterTick,
			TypewriterSpeed,
			true
		);
	}
}

void UCQDialogueWidget::ShowOptions(const TArray<FCQDialogueOption>& Options)
{
	ClearOptions();

	// 如果有选项，隐藏继续按钮
	if (Btn_Continue && Options.Num() > 0)
	{
		Btn_Continue->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 选项按钮需要在蓝图中动态创建
	// 此处提供选项数据，具体UI创建由蓝图完成
	UE_LOG(LogContractQuadrant, Log, TEXT("显示 %d 个对话选项"), Options.Num());
}

void UCQDialogueWidget::ClearOptions()
{
	if (OptionsContainer)
	{
		OptionsContainer->ClearChildren();
	}
}

void UCQDialogueWidget::OnContinueClicked()
{
	// 如果打字机还在进行中,点击则直接显示全部文本
	if (CurrentCharIndex < FullDialogueText.Len())
	{
		FinishTypewriter();
	}
	else
	{
		// 通知对话组件推进
		// 由蓝图或外部系统处理
	}
}

void UCQDialogueWidget::TypewriterTick()
{
	if (CurrentCharIndex < FullDialogueText.Len())
	{
		CurrentCharIndex++;
		FString DisplayText = FullDialogueText.Left(CurrentCharIndex);

		if (Txt_DialogueText)
		{
			Txt_DialogueText->SetText(FText::FromString(DisplayText));
		}
	}
	else
	{
		FinishTypewriter();
	}
}

void UCQDialogueWidget::FinishTypewriter()
{
	// 停止定时器
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TypewriterTimerHandle);
	}

	// 显示完整文本
	CurrentCharIndex = FullDialogueText.Len();
	if (Txt_DialogueText)
	{
		Txt_DialogueText->SetText(FText::FromString(FullDialogueText));
	}

	// 显示继续按钮
	if (Btn_Continue)
	{
		Btn_Continue->SetVisibility(ESlateVisibility::Visible);
	}
}
