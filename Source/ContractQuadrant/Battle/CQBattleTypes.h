// Copyright (C) 2025 契约象限. All Rights Reserved.
// 战斗类型定义

#pragma once

#include "CoreMinimal.h"
#include "Core/CQTypes.h"
#include "CrystalBeast/CQBeastDataTypes.h"
#include "CQBattleTypes.generated.h"

// ============================================================================
// 战斗阶段
// ============================================================================
UENUM(BlueprintType)
enum class ECQBattlePhase : uint8
{
	Initialization	UMETA(DisplayName = "初始化"),
	TurnStart		UMETA(DisplayName = "回合开始"),
	PlayerSelect	UMETA(DisplayName = "玩家选择"),
	ActionExecution	UMETA(DisplayName = "行动执行"),
	TurnEnd			UMETA(DisplayName = "回合结束"),
	BattleEnd		UMETA(DisplayName = "战斗结束")
};

// ============================================================================
// 战斗结果
// ============================================================================
UENUM(BlueprintType)
enum class ECQBattleResult : uint8
{
	None		UMETA(DisplayName = "无"),
	PlayerWin	UMETA(DisplayName = "玩家胜利"),
	PlayerLose	UMETA(DisplayName = "玩家失败"),
	PlayerFled	UMETA(DisplayName = "逃跑成功"),
	Captured	UMETA(DisplayName = "捕获成功")
};

// ============================================================================
// 战斗行动数据
// ============================================================================
USTRUCT(BlueprintType)
struct FCQBattleAction
{
	GENERATED_BODY()

	// 行动类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "战斗")
	ECQBattleActionType ActionType = ECQBattleActionType::Attack;

	// 使用的技能索引（仅攻击时有效）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "战斗")
	int32 SkillIndex = 0;

	// 使用的道具ID（仅使用道具时有效）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "战斗")
	FName ItemID;

	// 切换到的晶兽索引（仅切换晶兽时有效）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "战斗")
	int32 SwitchBeastIndex = 0;

	// 行动方是否为玩家
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "战斗")
	bool bIsPlayerAction = true;

	// 行动方的速度（用于排序）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "战斗")
	int32 ActionSpeed = 0;
};

// ============================================================================
// 战斗伤害结果
// ============================================================================
USTRUCT(BlueprintType)
struct FCQDamageResult
{
	GENERATED_BODY()

	// 最终伤害值
	UPROPERTY(BlueprintReadOnly, Category = "伤害")
	int32 FinalDamage = 0;

	// 是否暴击
	UPROPERTY(BlueprintReadOnly, Category = "伤害")
	bool bIsCritical = false;

	// 属性克制倍率
	UPROPERTY(BlueprintReadOnly, Category = "伤害")
	float TypeMultiplier = 1.0f;

	// 是否命中
	UPROPERTY(BlueprintReadOnly, Category = "伤害")
	bool bHit = true;

	// 是否附加了状态异常
	UPROPERTY(BlueprintReadOnly, Category = "伤害")
	bool bAppliedStatus = false;

	// 附加的状态异常类型
	UPROPERTY(BlueprintReadOnly, Category = "伤害")
	ECQStatusEffectType AppliedStatusType = ECQStatusEffectType::None;

	// 克制描述文本（"效果绝佳！"/"效果不太好..."）
	UPROPERTY(BlueprintReadOnly, Category = "伤害")
	FText EffectivenessText;
};

// ============================================================================
// 战斗回合信息
// ============================================================================
USTRUCT(BlueprintType)
struct FCQBattleTurnInfo
{
	GENERATED_BODY()

	// 当前回合数
	UPROPERTY(BlueprintReadOnly, Category = "回合")
	int32 TurnNumber = 0;

	// 玩家当前出战晶兽索引
	UPROPERTY(BlueprintReadOnly, Category = "回合")
	int32 PlayerActiveBeastIndex = 0;

	// 敌方当前出战晶兽索引
	UPROPERTY(BlueprintReadOnly, Category = "回合")
	int32 EnemyActiveBeastIndex = 0;

	// 本回合战斗日志
	UPROPERTY(BlueprintReadOnly, Category = "回合")
	TArray<FText> TurnLog;
};
