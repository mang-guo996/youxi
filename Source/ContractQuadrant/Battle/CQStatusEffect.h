// Copyright (C) 2025 契约象限. All Rights Reserved.
// 状态异常系统

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/CQTypes.h"
#include "CrystalBeast/CQBeastDataTypes.h"
#include "CQStatusEffect.generated.h"

UCLASS(BlueprintType)
class CONTRACTQUADRANT_API UCQStatusEffect : public UObject
{
	GENERATED_BODY()

public:
	// 在回合开始时处理状态异常效果
	// 返回值：该晶兽本回合是否可以行动
	UFUNCTION(BlueprintCallable, Category = "状态异常")
	static bool ProcessStatusEffect(UPARAM(ref) FCQBeastInstanceData& BeastData, TArray<FText>& OutMessages);

	// 尝试附加状态异常
	UFUNCTION(BlueprintCallable, Category = "状态异常")
	static bool ApplyStatusEffect(UPARAM(ref) FCQBeastInstanceData& BeastData, 
		ECQStatusEffectType StatusType, int32 Duration = 3);

	// 移除状态异常
	UFUNCTION(BlueprintCallable, Category = "状态异常")
	static void RemoveStatusEffect(UPARAM(ref) FCQBeastInstanceData& BeastData);

	// 获取状态异常名称
	UFUNCTION(BlueprintCallable, Category = "状态异常")
	static FText GetStatusEffectName(ECQStatusEffectType StatusType);

	// 获取每回合伤害量（中毒/灼烧用）
	UFUNCTION(BlueprintCallable, Category = "状态异常")
	static int32 GetStatusDamagePerTurn(const FCQBeastInstanceData& BeastData, ECQStatusEffectType StatusType);
};
