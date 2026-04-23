// Copyright (C) 2025 契约象限. All Rights Reserved.
// 全局类型定义 - 包含游戏中使用的所有枚举、结构体

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CQTypes.generated.h"

// ============================================================================
// 属性元素类型（五行属性系统）
// 火→草→水→火 形成基础克制环
// 光↔暗 互相克制
// ============================================================================
UENUM(BlueprintType)
enum class ECQElementType : uint8
{
	None	UMETA(DisplayName = "无"),
	Fire	UMETA(DisplayName = "火"),
	Water	UMETA(DisplayName = "水"),
	Grass	UMETA(DisplayName = "草"),
	Light	UMETA(DisplayName = "光"),
	Dark	UMETA(DisplayName = "暗")
};

// ============================================================================
// 晶兽稀有度
// ============================================================================
UENUM(BlueprintType)
enum class ECQBeastRarity : uint8
{
	Common		UMETA(DisplayName = "普通"),
	Uncommon	UMETA(DisplayName = "稀有"),
	Rare		UMETA(DisplayName = "珍贵"),
	Epic		UMETA(DisplayName = "史诗"),
	Legendary	UMETA(DisplayName = "传说")
};

// ============================================================================
// 战斗状态
// ============================================================================
UENUM(BlueprintType)
enum class ECQBattleState : uint8
{
	None			UMETA(DisplayName = "无"),
	Initializing	UMETA(DisplayName = "初始化"),
	TurnStart		UMETA(DisplayName = "回合开始"),
	ActionSelect	UMETA(DisplayName = "选择行动"),
	ActionExecute	UMETA(DisplayName = "执行行动"),
	TurnEnd			UMETA(DisplayName = "回合结束"),
	BattleWon		UMETA(DisplayName = "战斗胜利"),
	BattleLost		UMETA(DisplayName = "战斗失败"),
	BattleFled		UMETA(DisplayName = "逃跑成功"),
	BeastCaptured	UMETA(DisplayName = "捕获成功")
};

// ============================================================================
// 任务状态
// ============================================================================
UENUM(BlueprintType)
enum class ECQQuestState : uint8
{
	Locked		UMETA(DisplayName = "未解锁"),
	Available	UMETA(DisplayName = "可接取"),
	Active		UMETA(DisplayName = "进行中"),
	Completed	UMETA(DisplayName = "已完成"),
	Failed		UMETA(DisplayName = "已失败")
};

// ============================================================================
// 任务目标类型
// ============================================================================
UENUM(BlueprintType)
enum class ECQObjectiveType : uint8
{
	TalkToNPC		UMETA(DisplayName = "与NPC对话"),
	DefeatBeast		UMETA(DisplayName = "击败晶兽"),
	CaptureBeast	UMETA(DisplayName = "捕获晶兽"),
	CollectItem		UMETA(DisplayName = "收集物品"),
	ReachLocation	UMETA(DisplayName = "到达地点"),
	WinBattle		UMETA(DisplayName = "赢得战斗")
};

// ============================================================================
// 战斗行动类型
// ============================================================================
UENUM(BlueprintType)
enum class ECQBattleActionType : uint8
{
	Attack		UMETA(DisplayName = "攻击"),
	UseItem		UMETA(DisplayName = "使用道具"),
	SwitchBeast	UMETA(DisplayName = "切换晶兽"),
	Flee		UMETA(DisplayName = "逃跑"),
	Capture		UMETA(DisplayName = "捕获")
};

// ============================================================================
// 状态异常类型
// ============================================================================
UENUM(BlueprintType)
enum class ECQStatusEffectType : uint8
{
	None		UMETA(DisplayName = "无"),
	Poison		UMETA(DisplayName = "中毒"),
	Burn		UMETA(DisplayName = "灼烧"),
	Freeze		UMETA(DisplayName = "冰冻"),
	Paralysis	UMETA(DisplayName = "麻痹"),
	Sleep		UMETA(DisplayName = "睡眠")
};

// ============================================================================
// 游戏模式枚举（探索/战斗切换）
// ============================================================================
UENUM(BlueprintType)
enum class ECQGamePlayMode : uint8
{
	Exploration	UMETA(DisplayName = "探索模式"),
	Battle		UMETA(DisplayName = "战斗模式"),
	Dialogue	UMETA(DisplayName = "对话模式"),
	Menu		UMETA(DisplayName = "菜单模式")
};

// ============================================================================
// 场景区域类型
// ============================================================================
UENUM(BlueprintType)
enum class ECQWorldArea : uint8
{
	CrystalTown		UMETA(DisplayName = "晶石小镇"),
	PhantomForest	UMETA(DisplayName = "幽光森林"),
	MoltenCanyon	UMETA(DisplayName = "熔烬峡谷")
};

// ============================================================================
// 基础属性集 - 用于晶兽和角色的核心数值
// ============================================================================
USTRUCT(BlueprintType)
struct FCQAttributeSet
{
	GENERATED_BODY()

	// 最大生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	int32 MaxHP = 100;

	// 当前生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	int32 CurrentHP = 100;

	// 攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	int32 Attack = 50;

	// 防御力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	int32 Defense = 50;

	// 速度（决定行动顺序）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	int32 Speed = 50;

	// 特攻
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	int32 SpecialAttack = 50;

	// 特防
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	int32 SpecialDefense = 50;
};

// ============================================================================
// 属性克制矩阵行 - DataTable 用
// ============================================================================
USTRUCT(BlueprintType)
struct FCQElementCounterRow : public FTableRowBase
{
	GENERATED_BODY()

	// 攻击方属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性克制")
	ECQElementType AttackElement = ECQElementType::None;

	// 防御方属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性克制")
	ECQElementType DefenseElement = ECQElementType::None;

	// 伤害倍率 (2.0=克制, 1.0=普通, 0.5=被克制)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性克制")
	float DamageMultiplier = 1.0f;
};

// ============================================================================
// 物品数据结构
// ============================================================================
USTRUCT(BlueprintType)
struct FCQItemData : public FTableRowBase
{
	GENERATED_BODY()

	// 物品ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品")
	FName ItemID;

	// 物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品")
	FText ItemName;

	// 物品描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品")
	FText Description;

	// 物品图标
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品")
	TSoftObjectPtr<UTexture2D> Icon;

	// 是否可在战斗中使用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品")
	bool bUsableInBattle = false;

	// 回复HP量（0表示非回复道具）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品")
	int32 HealAmount = 0;

	// 是否为捕获道具
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品")
	bool bIsCaptureItem = false;

	// 捕获倍率（仅捕获道具有效）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品")
	float CaptureRateBonus = 1.0f;

	// 最大堆叠数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品")
	int32 MaxStackCount = 99;
};

// ============================================================================
// 背包物品实例（含数量）
// ============================================================================
USTRUCT(BlueprintType)
struct FCQInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "背包")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "背包")
	int32 Count = 0;
};
