// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "CrystalBeast/CQCrystalBeast.h"
#include "ContractQuadrant.h"

ACQCrystalBeast::ACQCrystalBeast()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACQCrystalBeast::BeginPlay()
{
	Super::BeginPlay();
}

// ============================================================================
// 数据初始化
// ============================================================================

void ACQCrystalBeast::InitializeFromBaseData(const FCQBeastBaseData& BaseData, int32 Level)
{
	CachedBaseData = BaseData;

	BeastData.BeastBaseID = BaseData.BeastID;
	BeastData.Level = FMath::Clamp(Level, 1, BaseData.MaxLevel);
	BeastData.CurrentExp = 0;
	BeastData.ExpToNextLevel = CalculateExpForLevel(BeastData.Level + 1);
	BeastData.GenerateInstanceID();

	// 根据等级计算属性
	CalculateStatsForLevel(BeastData.Level);

	// 初始化技能（根据等级学习对应技能）
	BeastData.LearnedSkills.Empty();
	for (const FCQLearnableSkill& Learnable : BaseData.LearnableSkills)
	{
		if (Learnable.LearnLevel <= Level && BeastData.LearnedSkills.Num() < 4)
		{
			FCQSkillInstance SkillInst;
			SkillInst.SkillID = Learnable.SkillID;
			// PP值将从技能DataTable中读取，此处使用默认值
			SkillInst.CurrentPP = 20;
			SkillInst.MaxPP = 20;
			BeastData.LearnedSkills.Add(SkillInst);
		}
	}

	bIsInitialized = true;
	UE_LOG(LogContractQuadrant, Log, TEXT("晶兽初始化: %s Lv.%d"), *BaseData.BeastID.ToString(), Level);
}

void ACQCrystalBeast::InitializeFromInstanceData(const FCQBeastInstanceData& InstanceData)
{
	BeastData = InstanceData;
	bIsInitialized = true;
}

FText ACQCrystalBeast::GetDisplayName() const
{
	if (!BeastData.Nickname.IsEmpty())
	{
		return BeastData.Nickname;
	}
	return CachedBaseData.BeastName;
}

// ============================================================================
// 战斗相关
// ============================================================================

int32 ACQCrystalBeast::UseSkill(int32 SkillIndex, ACQCrystalBeast* Target)
{
	if (SkillIndex < 0 || SkillIndex >= BeastData.LearnedSkills.Num())
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("无效的技能索引: %d"), SkillIndex);
		return 0;
	}

	FCQSkillInstance& Skill = BeastData.LearnedSkills[SkillIndex];

	// 检查PP
	if (Skill.CurrentPP <= 0)
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("技能PP不足: %s"), *Skill.SkillID.ToString());
		return 0;
	}

	// 消耗PP
	Skill.CurrentPP--;

	// 伤害计算在BattleManager中统一处理，此处返回技能索引供外部使用
	return SkillIndex;
}

void ACQCrystalBeast::TakeBattleDamage_Implementation(int32 DamageAmount, AActor* DamageSource)
{
	int32 OldHP = BeastData.CurrentStats.CurrentHP;
	BeastData.CurrentStats.CurrentHP = FMath::Max(0, BeastData.CurrentStats.CurrentHP - DamageAmount);

	OnHealthChanged.Broadcast(BeastData.CurrentStats.CurrentHP, BeastData.CurrentStats.MaxHP);

	UE_LOG(LogContractQuadrant, Log, TEXT("晶兽受到伤害: %d (HP: %d -> %d)"),
		DamageAmount, OldHP, BeastData.CurrentStats.CurrentHP);

	if (BeastData.CurrentStats.CurrentHP <= 0)
	{
		OnDefeated.Broadcast(this);
	}
}

int32 ACQCrystalBeast::GetCurrentHealth_Implementation() const
{
	return BeastData.CurrentStats.CurrentHP;
}

int32 ACQCrystalBeast::GetMaxHealth_Implementation() const
{
	return BeastData.CurrentStats.MaxHP;
}

bool ACQCrystalBeast::IsDefeated_Implementation() const
{
	return BeastData.CurrentStats.CurrentHP <= 0;
}

void ACQCrystalBeast::HealHealth_Implementation(int32 HealAmount)
{
	BeastData.CurrentStats.CurrentHP = FMath::Min(
		BeastData.CurrentStats.MaxHP,
		BeastData.CurrentStats.CurrentHP + HealAmount);

	OnHealthChanged.Broadcast(BeastData.CurrentStats.CurrentHP, BeastData.CurrentStats.MaxHP);
}

// ============================================================================
// 成长系统
// ============================================================================

void ACQCrystalBeast::GainExperience(int32 ExpAmount)
{
	if (BeastData.Level >= CachedBaseData.MaxLevel)
	{
		return; // 已满级
	}

	BeastData.CurrentExp += ExpAmount;
	UE_LOG(LogContractQuadrant, Log, TEXT("晶兽获得经验: +%d (当前: %d/%d)"),
		ExpAmount, BeastData.CurrentExp, BeastData.ExpToNextLevel);

	CheckLevelUp();
}

void ACQCrystalBeast::FullRestore()
{
	BeastData.CurrentStats.CurrentHP = BeastData.CurrentStats.MaxHP;
	BeastData.CurrentStatusEffect = ECQStatusEffectType::None;
	BeastData.StatusEffectTurnsRemaining = 0;

	// 恢复所有技能PP
	for (FCQSkillInstance& Skill : BeastData.LearnedSkills)
	{
		Skill.CurrentPP = Skill.MaxPP;
	}

	OnHealthChanged.Broadcast(BeastData.CurrentStats.CurrentHP, BeastData.CurrentStats.MaxHP);
}

// ============================================================================
// 内部方法
// ============================================================================

void ACQCrystalBeast::CalculateStatsForLevel(int32 Level)
{
	// 属性计算公式: BaseStat * (1 + GrowthRate * (Level - 1))
	const FCQAttributeSet& Base = CachedBaseData.BaseStats;
	float LevelMultiplier = static_cast<float>(Level - 1);

	BeastData.CurrentStats.MaxHP = FMath::RoundToInt(Base.MaxHP * (1.0f + CachedBaseData.HPGrowthRate * LevelMultiplier));
	BeastData.CurrentStats.CurrentHP = BeastData.CurrentStats.MaxHP; // 初始化时满血
	BeastData.CurrentStats.Attack = FMath::RoundToInt(Base.Attack * (1.0f + CachedBaseData.AttackGrowthRate * LevelMultiplier));
	BeastData.CurrentStats.Defense = FMath::RoundToInt(Base.Defense * (1.0f + CachedBaseData.DefenseGrowthRate * LevelMultiplier));
	BeastData.CurrentStats.Speed = FMath::RoundToInt(Base.Speed * (1.0f + CachedBaseData.SpeedGrowthRate * LevelMultiplier));
	BeastData.CurrentStats.SpecialAttack = FMath::RoundToInt(Base.SpecialAttack * (1.0f + CachedBaseData.SpAttackGrowthRate * LevelMultiplier));
	BeastData.CurrentStats.SpecialDefense = FMath::RoundToInt(Base.SpecialDefense * (1.0f + CachedBaseData.SpDefenseGrowthRate * LevelMultiplier));
}

void ACQCrystalBeast::CheckLevelUp()
{
	while (BeastData.CurrentExp >= BeastData.ExpToNextLevel && BeastData.Level < CachedBaseData.MaxLevel)
	{
		BeastData.CurrentExp -= BeastData.ExpToNextLevel;
		BeastData.Level++;

		// 重新计算属性
		int32 OldMaxHP = BeastData.CurrentStats.MaxHP;
		CalculateStatsForLevel(BeastData.Level);

		// 升级时恢复相应HP增量
		BeastData.CurrentStats.CurrentHP += (BeastData.CurrentStats.MaxHP - OldMaxHP);

		BeastData.ExpToNextLevel = CalculateExpForLevel(BeastData.Level + 1);

		// 检查是否学习新技能
		for (const FCQLearnableSkill& Learnable : CachedBaseData.LearnableSkills)
		{
			if (Learnable.LearnLevel == BeastData.Level && BeastData.LearnedSkills.Num() < 4)
			{
				FCQSkillInstance NewSkill;
				NewSkill.SkillID = Learnable.SkillID;
				NewSkill.CurrentPP = 20;
				NewSkill.MaxPP = 20;
				BeastData.LearnedSkills.Add(NewSkill);

				UE_LOG(LogContractQuadrant, Log, TEXT("晶兽学会新技能: %s"), *Learnable.SkillID.ToString());
			}
		}

		OnLevelUp.Broadcast(this, BeastData.Level);
		UE_LOG(LogContractQuadrant, Log, TEXT("晶兽升级! Lv.%d"), BeastData.Level);
	}
}

int32 ACQCrystalBeast::CalculateExpForLevel(int32 Level) const
{
	// 经验曲线: Level^3 * 0.8 + Level * 20
	// 这是一个较为平缓的成长曲线，适合RPG游戏
	return FMath::RoundToInt(FMath::Pow(static_cast<float>(Level), 3.0f) * 0.8f + Level * 20.0f);
}
