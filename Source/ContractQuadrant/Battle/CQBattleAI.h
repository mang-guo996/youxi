// Copyright (C) 2025 契约象限. All Rights Reserved.
// 战斗AI - 敌方行动决策系统

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Battle/CQBattleTypes.h"
#include "CQBattleAI.generated.h"

UCLASS(BlueprintType)
class CONTRACTQUADRANT_API UCQBattleAI : public UObject
{
	GENERATED_BODY()

public:
	// 决定AI行动
	UFUNCTION(BlueprintCallable, Category = "战斗AI")
	FCQBattleAction DecideAction(
		const FCQBeastInstanceData& AIBeast,
		const FCQBeastInstanceData& PlayerBeast,
		const TArray<FCQBeastInstanceData>& AIParty);

	// AI难度等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "战斗AI")
	int32 DifficultyLevel = 1; // 1=简单, 2=普通, 3=困难

private:
	// 选择最佳技能
	int32 SelectBestSkill(const FCQBeastInstanceData& AIBeast, const FCQBeastInstanceData& PlayerBeast);

	// 评估是否应该切换晶兽
	bool ShouldSwitchBeast(const FCQBeastInstanceData& AIBeast,
		const FCQBeastInstanceData& PlayerBeast,
		const TArray<FCQBeastInstanceData>& AIParty);

	// 找到最佳切换目标
	int32 FindBestSwitchTarget(const FCQBeastInstanceData& PlayerBeast,
		const TArray<FCQBeastInstanceData>& AIParty, int32 CurrentIndex);
};
