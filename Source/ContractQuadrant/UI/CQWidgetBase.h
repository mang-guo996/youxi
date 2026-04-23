// Copyright (C) 2025 契约象限. All Rights Reserved.
// Widget基类 - 通用UI行为

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CQWidgetBase.generated.h"

UCLASS()
class CONTRACTQUADRANT_API UCQWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// 显示Widget（带动画）
	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual void ShowWidget();

	// 隐藏Widget（带动画）
	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual void HideWidget();

	// 切换显示/隐藏
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleWidget();

	// 是否正在显示
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsShowing() const { return bIsShowing; }

protected:
	virtual void NativeConstruct() override;

	// 显示/隐藏动画
	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> ShowAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> HideAnimation;

	bool bIsShowing = false;
};
