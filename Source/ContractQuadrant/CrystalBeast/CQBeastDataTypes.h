// Copyright (C) 2025 契约象限. All Rights Reserved.
// 晶兽数据类型 - 所有晶兽相关的数据结构

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Core/CQTypes.h"
#include "CQBeastDataTypes.generated.h"

// ============================================================================
// 技能数据（DataTable行）
// ============================================================================
USTRUCT(BlueprintType)
struct FCQBeastSkillData : public FTableRowBase
{
	GENERATED_BODY()

	// 技能ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	FName SkillID;

	// 技能名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	FText SkillName;

	// 技能描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	FText Description;

	// 技能属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	ECQElementType Element = ECQElementType::None;

	// 技能威力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	int32 Power = 40;

	// 命中率 (0-100)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	int32 Accuracy = 100;

	// 最大使用次数 (PP)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	int32 MaxPP = 20;

	// 是否为特殊攻击（使用特攻/特防计算）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	bool bIsSpecialAttack = false;

	// 附加状态异常类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	ECQStatusEffectType StatusEffect = ECQStatusEffectType::None;

	// 附加状态异常概率 (0-100)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	int32 StatusEffectChance = 0;

	// 技能特效引用（Niagara粒子系统）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	TSoftObjectPtr<UObject> SkillVFX;

	// 技能音效引用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	TSoftObjectPtr<USoundBase> SkillSFX;
};

// ============================================================================
// 晶兽可学习技能条目（等级 → 技能）
// ============================================================================
USTRUCT(BlueprintType)
struct FCQLearnableSkill
{
	GENERATED_BODY()

	// 学习等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能学习")
	int32 LearnLevel = 1;

	// 技能ID（对应DataTable中的行名）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能学习")
	FName SkillID;
};

// ============================================================================
// 晶兽基础数据（DataTable行）- 种族数据，不随实例变化
// ============================================================================
USTRUCT(BlueprintType)
struct FCQBeastBaseData : public FTableRowBase
{
	GENERATED_BODY()

	// 晶兽ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	FName BeastID;

	// 晶兽名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	FText BeastName;

	// 晶兽描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	FText Description;

	// 属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	ECQElementType Element = ECQElementType::Fire;

	// 稀有度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	ECQBeastRarity Rarity = ECQBeastRarity::Common;

	// 基础属性值（1级时的属性）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	FCQAttributeSet BaseStats;

	// 属性成长率（每升一级增长量，百分比）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	float HPGrowthRate = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	float AttackGrowthRate = 0.04f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	float DefenseGrowthRate = 0.04f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	float SpeedGrowthRate = 0.03f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	float SpAttackGrowthRate = 0.04f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	float SpDefenseGrowthRate = 0.04f;

	// 基础捕获率 (0-255, 越高越容易捕获)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "捕获")
	int32 BaseCaptureRate = 128;

	// 基础经验值产出（被击败时给予的经验）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "经验")
	int32 BaseExpYield = 50;

	// 可学习技能列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	TArray<FCQLearnableSkill> LearnableSkills;

	// 最大等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "等级")
	int32 MaxLevel = 50;

	// 进化目标（空则不进化）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "进化")
	FName EvolutionTargetID;

	// 进化所需等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "进化")
	int32 EvolutionLevel = 0;

	// 模型引用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "视觉")
	TSoftClassPtr<AActor> BeastBlueprintClass;

	// 图鉴图标
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "视觉")
	TSoftObjectPtr<UTexture2D> PortraitIcon;
};

// ============================================================================
// 晶兽技能实例数据（含当前PP）
// ============================================================================
USTRUCT(BlueprintType)
struct FCQSkillInstance
{
	GENERATED_BODY()

	// 技能ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	FName SkillID;

	// 当前PP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	int32 CurrentPP = 20;

	// 最大PP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	int32 MaxPP = 20;
};

// ============================================================================
// 晶兽实例数据 - 每只具体的晶兽的运行时数据
// ============================================================================
USTRUCT(BlueprintType)
struct FCQBeastInstanceData
{
	GENERATED_BODY()

	// 对应的基础数据ID（DataTable行名）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	FName BeastBaseID;

	// 自定义昵称（空则使用默认名称）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	FText Nickname;

	// 当前等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "等级")
	int32 Level = 1;

	// 当前经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "等级")
	int32 CurrentExp = 0;

	// 升到下一级所需经验
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "等级")
	int32 ExpToNextLevel = 100;

	// 当前计算后的属性值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	FCQAttributeSet CurrentStats;

	// 已学会的技能（最多4个）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能")
	TArray<FCQSkillInstance> LearnedSkills;

	// 当前状态异常
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "状态")
	ECQStatusEffectType CurrentStatusEffect = ECQStatusEffectType::None;

	// 状态异常剩余回合数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "状态")
	int32 StatusEffectTurnsRemaining = 0;

	// 唯一实例ID（用于区分同种晶兽）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	FGuid InstanceID;

	// 是否可以战斗（HP > 0）
	bool CanFight() const { return CurrentStats.CurrentHP > 0; }

	// 生成唯一ID
	void GenerateInstanceID() { InstanceID = FGuid::NewGuid(); }
};

// ============================================================================
// 野生晶兽生成表项
// ============================================================================
USTRUCT(BlueprintType)
struct FCQWildBeastEntry : public FTableRowBase
{
	GENERATED_BODY()

	// 晶兽基础ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "生成")
	FName BeastID;

	// 出现概率（权重）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "生成")
	float SpawnWeight = 1.0f;

	// 最低等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "生成")
	int32 MinLevel = 1;

	// 最高等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "生成")
	int32 MaxLevel = 5;

	// 所在区域
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "生成")
	ECQWorldArea Area = ECQWorldArea::CrystalTown;
};
