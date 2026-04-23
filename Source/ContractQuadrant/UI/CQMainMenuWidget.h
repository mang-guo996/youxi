// Copyright (C) 2025 契约象限. All Rights Reserved.
// 主菜单Widget

#pragma once

#include "CoreMinimal.h"
#include "UI/CQWidgetBase.h"
#include "CQMainMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;

UCLASS()
class CONTRACTQUADRANT_API UCQMainMenuWidget : public UCQWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	// ========== UI绑定 ==========

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_NewGame;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Continue;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_Settings;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_QuitGame;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_GameTitle;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> SaveSlotContainer;

	// ========== 按钮回调 ==========

	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnContinueClicked();

	UFUNCTION()
	void OnSettingsClicked();

	UFUNCTION()
	void OnQuitGameClicked();

	// 加载指定存档
	UFUNCTION(BlueprintCallable, Category = "菜单")
	void LoadSaveSlot(int32 SlotIndex);

	// 刷新存档槽位显示
	UFUNCTION(BlueprintCallable, Category = "菜单")
	void RefreshSaveSlots();
};
