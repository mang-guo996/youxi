// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Battle/CQDamageCalculation.h"
#include "Engine/DataTable.h"
#include "ContractQuadrant.h"

FCQDamageResult UCQDamageCalculation::CalculateDamage(
	const FCQBeastInstanceData& Attacker,
	const FCQBeastInstanceData& Defender,
	const FCQBeastSkillData& Skill,
	UDataTable* ElementCounterTable)
{
	FCQDamageResult Result;

	// 1. 命中判定
	if (!RollAccuracy(Skill.Accuracy))
	{
		Result.bHit = false;
		Result.FinalDamage = 0;
		return Result;
	}
	Result.bHit = true;

	// 2. 获取攻击和防御数值
	int32 AttackStat, DefenseStat;
	if (Skill.bIsSpecialAttack)
	{
		AttackStat = Attacker.CurrentStats.SpecialAttack;
		DefenseStat = Defender.CurrentStats.SpecialDefense;
	}
	else
	{
		AttackStat = Attacker.CurrentStats.Attack;
		DefenseStat = Defender.CurrentStats.Defense;
	}

	// 防御不能为0
	DefenseStat = FMath::Max(1, DefenseStat);

	// 3. 基础伤害公式
	// Damage = ((2 * Level / 5 + 2) * Power * Atk / Def) / 50 + 2
	float Level = static_cast<float>(Attacker.Level);
	float Power = static_cast<float>(Skill.Power);
	float Atk = static_cast<float>(AttackStat);
	float Def = static_cast<float>(DefenseStat);

	float BaseDamage = ((2.0f * Level / 5.0f + 2.0f) * Power * Atk / Def) / 50.0f + 2.0f;

	// 4. 属性克制倍率
	if (ElementCounterTable)
	{
		Result.TypeMultiplier = GetElementMultiplier(Skill.Element, 
			// 需要从外部获取防御方属性，这里使用参数传递
			// 此处简化处理，实际使用时从BeastBaseData获取
			ECQElementType::None, ElementCounterTable);
	}
	else
	{
		// 使用硬编码的默认克制关系
		Result.TypeMultiplier = GetDefaultElementMultiplier(Skill.Element, ECQElementType::None);
	}

	BaseDamage *= Result.TypeMultiplier;

	// 5. 同属性加成（STAB: Same Type Attack Bonus）
	// 如果技能属性与使用者属性相同，伤害 * 1.5
	// 注意：此处需要从外部传入攻击者属性，简化为通过Skill.Element判断
	float StabBonus = 1.0f; // 实际使用时在BattleManager中判断
	BaseDamage *= StabBonus;

	// 6. 暴击判定
	Result.bIsCritical = RollCritical();
	if (Result.bIsCritical)
	{
		BaseDamage *= 1.5f;
	}

	// 7. 随机浮动 (0.85 ~ 1.0)
	float RandomFactor = FMath::FRandRange(0.85f, 1.0f);
	BaseDamage *= RandomFactor;

	// 8. 最终伤害（最少1点）
	Result.FinalDamage = FMath::Max(1, FMath::RoundToInt(BaseDamage));

	// 9. 状态异常附加判定
	if (Skill.StatusEffect != ECQStatusEffectType::None && Skill.StatusEffectChance > 0)
	{
		int32 StatusRoll = FMath::RandRange(1, 100);
		if (StatusRoll <= Skill.StatusEffectChance)
		{
			Result.bAppliedStatus = true;
			Result.AppliedStatusType = Skill.StatusEffect;
		}
	}

	// 10. 设置克制描述
	Result.EffectivenessText = GetEffectivenessText(Result.TypeMultiplier);

	return Result;
}

float UCQDamageCalculation::GetElementMultiplier(
	ECQElementType AttackElement,
	ECQElementType DefenseElement,
	UDataTable* ElementCounterTable)
{
	if (!ElementCounterTable)
	{
		return GetDefaultElementMultiplier(AttackElement, DefenseElement);
	}

	// 从DataTable查表
	TArray<FCQElementCounterRow*> AllRows;
	ElementCounterTable->GetAllRows<FCQElementCounterRow>(TEXT("ElementCounter"), AllRows);

	for (const FCQElementCounterRow* Row : AllRows)
	{
		if (Row->AttackElement == AttackElement && Row->DefenseElement == DefenseElement)
		{
			return Row->DamageMultiplier;
		}
	}

	return 1.0f; // 未找到则返回普通倍率
}

float UCQDamageCalculation::GetDefaultElementMultiplier(
	ECQElementType AttackElement, ECQElementType DefenseElement)
{
	// 硬编码的属性克制关系
	// 火→草 2.0x | 火←水 0.5x
	// 水→火 2.0x | 水←草 0.5x
	// 草→水 2.0x | 草←火 0.5x
	// 光↔暗 互克 2.0x

	if (AttackElement == ECQElementType::None || DefenseElement == ECQElementType::None)
	{
		return 1.0f;
	}

	// 火克草
	if (AttackElement == ECQElementType::Fire && DefenseElement == ECQElementType::Grass) return 2.0f;
	if (AttackElement == ECQElementType::Fire && DefenseElement == ECQElementType::Water) return 0.5f;

	// 水克火
	if (AttackElement == ECQElementType::Water && DefenseElement == ECQElementType::Fire) return 2.0f;
	if (AttackElement == ECQElementType::Water && DefenseElement == ECQElementType::Grass) return 0.5f;

	// 草克水
	if (AttackElement == ECQElementType::Grass && DefenseElement == ECQElementType::Water) return 2.0f;
	if (AttackElement == ECQElementType::Grass && DefenseElement == ECQElementType::Fire) return 0.5f;

	// 光暗互克
	if (AttackElement == ECQElementType::Light && DefenseElement == ECQElementType::Dark) return 2.0f;
	if (AttackElement == ECQElementType::Dark && DefenseElement == ECQElementType::Light) return 2.0f;

	// 同属性抵抗
	if (AttackElement == DefenseElement) return 0.5f;

	return 1.0f;
}

bool UCQDamageCalculation::RollAccuracy(int32 Accuracy)
{
	int32 Roll = FMath::RandRange(1, 100);
	return Roll <= Accuracy;
}

bool UCQDamageCalculation::RollCritical(float CritRate)
{
	float Roll = FMath::FRandRange(0.0f, 1.0f);
	return Roll < CritRate;
}

FText UCQDamageCalculation::GetEffectivenessText(float Multiplier)
{
	if (Multiplier >= 2.0f)
	{
		return FText::FromString(TEXT("效果绝佳！"));
	}
	else if (Multiplier > 1.0f)
	{
		return FText::FromString(TEXT("效果不错！"));
	}
	else if (Multiplier < 0.5f)
	{
		return FText::FromString(TEXT("几乎没有效果..."));
	}
	else if (Multiplier < 1.0f)
	{
		return FText::FromString(TEXT("效果不太好..."));
	}
	return FText::GetEmpty();
}

bool UCQDamageCalculation::RollFleeSuccess(int32 PlayerSpeed, int32 EnemySpeed, int32 FleeAttempts)
{
	// 逃跑公式: FleeChance = (PlayerSpeed * 128 / EnemySpeed + 30 * FleeAttempts) / 256
	if (EnemySpeed <= 0) return true;

	float FleeChance = (static_cast<float>(PlayerSpeed) * 128.0f /
		static_cast<float>(EnemySpeed) + 30.0f * FleeAttempts) / 256.0f;

	FleeChance = FMath::Clamp(FleeChance, 0.1f, 1.0f);

	float Roll = FMath::FRandRange(0.0f, 1.0f);
	return Roll < FleeChance;
}
