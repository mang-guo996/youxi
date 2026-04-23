// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "UI/CQHUDWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void UCQHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HideInteractPrompt();

	if (Txt_Notification)
	{
		Txt_Notification->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCQHUDWidget::UpdateQuestHint(const FText& QuestName, const FText& ObjectiveText)
{
	if (Txt_CurrentQuestName) Txt_CurrentQuestName->SetText(QuestName);
	if (Txt_CurrentObjective) Txt_CurrentObjective->SetText(ObjectiveText);
}

void UCQHUDWidget::UpdateAreaName(const FText& AreaName)
{
	if (Txt_AreaName) Txt_AreaName->SetText(AreaName);
}

void UCQHUDWidget::UpdatePartyStatus(int32 PartySize, int32 AliveBeastCount)
{
	if (Txt_PartyStatus)
	{
		Txt_PartyStatus->SetText(FText::FromString(
			FString::Printf(TEXT("晶兽: %d/%d"), AliveBeastCount, PartySize)));
	}
}

void UCQHUDWidget::ShowInteractPrompt(const FText& PromptText)
{
	if (Txt_InteractPrompt)
	{
		Txt_InteractPrompt->SetText(PromptText);
		Txt_InteractPrompt->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCQHUDWidget::HideInteractPrompt()
{
	if (Txt_InteractPrompt)
	{
		Txt_InteractPrompt->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCQHUDWidget::ShowNotification(const FText& Message, float Duration)
{
	if (Txt_Notification)
	{
		Txt_Notification->SetText(Message);
		Txt_Notification->SetVisibility(ESlateVisibility::Visible);
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(NotificationTimerHandle);
		World->GetTimerManager().SetTimer(
			NotificationTimerHandle,
			this,
			&UCQHUDWidget::HideNotification,
			Duration,
			false
		);
	}
}

void UCQHUDWidget::HideNotification()
{
	if (Txt_Notification)
	{
		Txt_Notification->SetVisibility(ESlateVisibility::Collapsed);
	}
}
