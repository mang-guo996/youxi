// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "UI/CQWidgetBase.h"
#include "Animation/WidgetAnimation.h"

void UCQWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCQWidgetBase::ShowWidget()
{
	SetVisibility(ESlateVisibility::Visible);
	bIsShowing = true;

	if (ShowAnimation)
	{
		PlayAnimation(ShowAnimation);
	}
}

void UCQWidgetBase::HideWidget()
{
	bIsShowing = false;

	if (HideAnimation)
	{
		PlayAnimation(HideAnimation);
		// 动画结束后设置不可见（可通过蓝图绑定动画结束事件）
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCQWidgetBase::ToggleWidget()
{
	if (bIsShowing)
	{
		HideWidget();
	}
	else
	{
		ShowWidget();
	}
}
