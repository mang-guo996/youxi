// Copyright (C) 2025 契约象限. All Rights Reserved.
// 玩家控制器 - Enhanced Input 集成、交互检测

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/CQTypes.h"
#include "CQPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class CONTRACTQUADRANT_API ACQPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACQPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	// ========== Enhanced Input 配置 ==========

	// 探索模式输入映射上下文
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "输入")
	TObjectPtr<UInputMappingContext> ExplorationMappingContext;

	// 战斗模式输入映射上下文
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "输入")
	TObjectPtr<UInputMappingContext> BattleMappingContext;

	// UI/菜单输入映射上下文
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "输入")
	TObjectPtr<UInputMappingContext> MenuMappingContext;

	// ========== 输入动作 ==========

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "输入|动作")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "输入|动作")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "输入|动作")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "输入|动作")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "输入|动作")
	TObjectPtr<UInputAction> OpenMenuAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "输入|动作")
	TObjectPtr<UInputAction> OpenBeastMenuAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "输入|动作")
	TObjectPtr<UInputAction> OpenQuestLogAction;

	// ========== 交互系统 ==========

	// 执行交互射线检测
	UFUNCTION(BlueprintCallable, Category = "交互")
	void PerformInteraction();

	// 切换输入上下文（根据游戏模式）
	UFUNCTION(BlueprintCallable, Category = "输入")
	void SwitchInputContext(ECQGamePlayMode NewMode);

	// 交互检测距离
	UPROPERTY(EditDefaultsOnly, Category = "交互")
	float InteractionDistance = 300.0f;

protected:
	// 输入回调函数
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleJump();
	void HandleStopJumping();
	void HandleInteract();
	void HandleOpenMenu();
	void HandleOpenBeastMenu();
	void HandleOpenQuestLog();
};
