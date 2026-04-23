// Copyright (C) 2025 契约象限. All Rights Reserved.
// 伤害计算系统 - 属性克制查表、伤害公式

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Battle/CQBattleTypes.h"
#include "CQDamageCalculation.generated.h"

class UDataTable;

UCLASS(BlueprintType)
class CONTRACTQUADRANT_API UCQDamageCalculation : public UObject
{
	GENERATED_BODY()

public:
	// 计算技能伤害
	UFUNCTION(BlueprintCallable, Category = "伤害计算")
	static FCQDamageResult CalculateDamage(
		const FCQBeastInstanceData& Attacker,
		const FCQBeastInstanceData& Defender,
		const FCQBeastSkillData& Skill,
		UDataTable* ElementCounterTable);

	// 获取属性克制倍率
	UFUNCTION(BlueprintCallable, Category = "伤害计算")
	static float GetElementMultiplier(
		ECQElementType AttackElement,
		ECQElementType DefenseElement,
		UDataTable* ElementCounterTable);

	// 判定命中
	UFUNCTION(BlueprintCallable, Category = "伤害计算")
	static bool RollAccuracy(int32 Accuracy);

	// 判定暴击
	UFUNCTION(BlueprintCallable, Category = "伤害计算")
	static bool RollCritical(float CritRate = 0.0625f);

	// 获取克制描述文本
	UFUNCTION(BlueprintCallable, Category = "伤害计算")
	static FText GetEffectivenessText(float Multiplier);

	// 计算逃跑成功率
	UFUNCTION(BlueprintCallable, Category = "伤害计算")
	static bool RollFleeSuccess(int32 PlayerSpeed, int32 EnemySpeed, int32 FleeAttempts);

	// 默认属性克制倍率（无DataTable时使用硬编码值）
	static float GetDefaultElementMultiplier(ECQElementType AttackElement, ECQElementType DefenseElement);
};
