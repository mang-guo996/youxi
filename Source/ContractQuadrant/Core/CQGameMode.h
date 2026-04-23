// Copyright (C) 2025 契约象限. All Rights Reserved.
// 主游戏模式 - 管理关卡逻辑和游戏模式切换

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/CQTypes.h"
#include "CQGameMode.generated.h"

class ACQBattleManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePlayModeChanged, ECQGamePlayMode, NewMode);

UCLASS()
class CONTRACTQUADRANT_API ACQGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACQGameMode();

	virtual void BeginPlay() override;

	// ========== 游戏模式切换 ==========

	// 切换游戏模式（探索/战斗/对话/菜单）
	UFUNCTION(BlueprintCallable, Category = "游戏模式")
	void SetGamePlayMode(ECQGamePlayMode NewMode);

	// 获取当前游戏模式
	UFUNCTION(BlueprintCallable, Category = "游戏模式")
	ECQGamePlayMode GetCurrentGamePlayMode() const { return CurrentGamePlayMode; }

	// ========== 战斗触发 ==========

	// 开始野生晶兽战斗
	UFUNCTION(BlueprintCallable, Category = "战斗")
	void StartWildBattle(FName WildBeastID, int32 BeastLevel);

	// 开始NPC训练师战斗
	UFUNCTION(BlueprintCallable, Category = "战斗")
	void StartTrainerBattle(const TArray<FName>& EnemyBeastIDs, const TArray<int32>& EnemyBeastLevels);

	// 结束战斗，返回探索模式
	UFUNCTION(BlueprintCallable, Category = "战斗")
	void EndBattle();

	// ========== 场景管理 ==========

	// 切换场景
	UFUNCTION(BlueprintCallable, Category = "场景")
	void TravelToArea(ECQWorldArea TargetArea);

	// ========== 委托 ==========

	// 游戏模式变更通知
	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnGamePlayModeChanged OnGamePlayModeChanged;

protected:
	// 当前游戏模式
	UPROPERTY(BlueprintReadOnly, Category = "游戏模式")
	ECQGamePlayMode CurrentGamePlayMode = ECQGamePlayMode::Exploration;

	// 战斗管理器类（蓝图中指定）
	UPROPERTY(EditDefaultsOnly, Category = "战斗")
	TSubclassOf<ACQBattleManager> BattleManagerClass;

	// 当前战斗管理器实例
	UPROPERTY()
	TObjectPtr<ACQBattleManager> CurrentBattleManager;

	// 场景地图映射
	UPROPERTY(EditDefaultsOnly, Category = "场景")
	TMap<ECQWorldArea, TSoftObjectPtr<UWorld>> AreaMaps;
};
