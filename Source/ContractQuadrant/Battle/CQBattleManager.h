// Copyright (C) 2025 契约象限. All Rights Reserved.
// 战斗管理器 - 回合制战斗的核心控制器

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Battle/CQBattleTypes.h"
#include "CQBattleManager.generated.h"

class UDataTable;
class ACQCrystalBeast;
class UCQBattleAI;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattlePhaseChanged, ECQBattlePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleEnded, ECQBattleResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleMessage, const FText&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageDealt, const FCQDamageResult&, DamageResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnInfoUpdated, const FCQBattleTurnInfo&, TurnInfo);

UCLASS()
class CONTRACTQUADRANT_API ACQBattleManager : public AActor
{
	GENERATED_BODY()

public:
	ACQBattleManager();

	// ========== 战斗初始化 ==========

	// 初始化野生晶兽战斗
	UFUNCTION(BlueprintCallable, Category = "战斗")
	void InitWildBattle(const TArray<FCQBeastInstanceData>& PlayerParty,
		FName WildBeastID, int32 WildBeastLevel);

	// 初始化训练师对战
	UFUNCTION(BlueprintCallable, Category = "战斗")
	void InitTrainerBattle(const TArray<FCQBeastInstanceData>& PlayerParty,
		const TArray<FName>& EnemyBeastIDs, const TArray<int32>& EnemyBeastLevels);

	// ========== 战斗控制 ==========

	// 玩家选择行动
	UFUNCTION(BlueprintCallable, Category = "战斗")
	void PlayerSelectAction(const FCQBattleAction& Action);

	// 执行回合
	UFUNCTION(BlueprintCallable, Category = "战斗")
	void ExecuteTurn();

	// 切换玩家出战晶兽
	UFUNCTION(BlueprintCallable, Category = "战斗")
	void SwitchPlayerBeast(int32 PartyIndex);

	// 尝试逃跑
	UFUNCTION(BlueprintCallable, Category = "战斗")
	bool AttemptFlee();

	// 尝试捕获
	UFUNCTION(BlueprintCallable, Category = "战斗")
	bool AttemptCapture(FName CaptureItemID);

	// ========== 查询接口 ==========

	// 获取当前战斗阶段
	UFUNCTION(BlueprintCallable, Category = "战斗")
	ECQBattlePhase GetCurrentPhase() const { return CurrentPhase; }

	// 获取回合信息
	UFUNCTION(BlueprintCallable, Category = "战斗")
	FCQBattleTurnInfo GetTurnInfo() const { return TurnInfo; }

	// 获取玩家当前出战晶兽数据
	UFUNCTION(BlueprintCallable, Category = "战斗")
	FCQBeastInstanceData GetPlayerActiveBeast() const;

	// 获取敌方当前出战晶兽数据
	UFUNCTION(BlueprintCallable, Category = "战斗")
	FCQBeastInstanceData GetEnemyActiveBeast() const;

	// 获取可用技能列表
	UFUNCTION(BlueprintCallable, Category = "战斗")
	TArray<FCQSkillInstance> GetPlayerAvailableSkills() const;

	// 是否为野生战斗（可捕获）
	UFUNCTION(BlueprintCallable, Category = "战斗")
	bool IsWildBattle() const { return bIsWildBattle; }

	// ========== 委托 ==========

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnBattlePhaseChanged OnBattlePhaseChanged;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnBattleEnded OnBattleEnded;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnBattleMessage OnBattleMessage;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnDamageDealt OnDamageDealt;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnTurnInfoUpdated OnTurnInfoUpdated;

	// ========== 配置 ==========

	// 晶兽基础数据表
	UPROPERTY(EditDefaultsOnly, Category = "数据")
	TObjectPtr<UDataTable> BeastDataTable;

	// 技能数据表
	UPROPERTY(EditDefaultsOnly, Category = "数据")
	TObjectPtr<UDataTable> SkillDataTable;

	// 属性克制数据表
	UPROPERTY(EditDefaultsOnly, Category = "数据")
	TObjectPtr<UDataTable> ElementCounterTable;

	// 物品数据表
	UPROPERTY(EditDefaultsOnly, Category = "数据")
	TObjectPtr<UDataTable> ItemDataTable;

protected:
	// 当前战斗阶段
	ECQBattlePhase CurrentPhase = ECQBattlePhase::Initialization;

	// 回合信息
	FCQBattleTurnInfo TurnInfo;

	// 玩家队伍数据
	TArray<FCQBeastInstanceData> PlayerParty;

	// 敌方队伍数据
	TArray<FCQBeastInstanceData> EnemyParty;

	// 是否为野生战斗
	bool bIsWildBattle = true;

	// 逃跑尝试次数
	int32 FleeAttempts = 0;

	// 玩家本回合行动
	FCQBattleAction PlayerAction;

	// AI本回合行动
	FCQBattleAction EnemyAction;

	// 战斗AI
	UPROPERTY()
	TObjectPtr<UCQBattleAI> BattleAI;

	// ========== 内部方法 ==========

	void SetPhase(ECQBattlePhase NewPhase);
	void StartTurn();
	void ProcessActions();
	void ExecuteAction(const FCQBattleAction& Action, bool bIsPlayerTurn);
	void ExecuteAttack(bool bIsPlayerAttack, int32 SkillIndex);
	void CheckBattleEnd();
	void EndBattle(ECQBattleResult Result);
	void ProcessTurnEndEffects();
	void BroadcastMessage(const FText& Message);

	// 获取技能数据
	FCQBeastSkillData GetSkillData(FName SkillID) const;
};
