// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Battle/CQStatusEffect.h"
#include "ContractQuadrant.h"

bool UCQStatusEffect::ProcessStatusEffect(FCQBeastInstanceData& BeastData, TArray<FText>& OutMessages)
{
	if (BeastData.CurrentStatusEffect == ECQStatusEffectType::None)
	{
		return true; // 无状态异常，可以行动
	}

	bool bCanAct = true;

	switch (BeastData.CurrentStatusEffect)
	{
	case ECQStatusEffectType::Poison:
		{
			// 中毒：每回合损失最大HP的1/8
			int32 PoisonDamage = GetStatusDamagePerTurn(BeastData, ECQStatusEffectType::Poison);
			BeastData.CurrentStats.CurrentHP = FMath::Max(0, BeastData.CurrentStats.CurrentHP - PoisonDamage);
			OutMessages.Add(FText::FromString(FString::Printf(TEXT("中毒造成了 %d 点伤害！"), PoisonDamage)));
			bCanAct = true; // 中毒不影响行动
		}
		break;

	case ECQStatusEffectType::Burn:
		{
			// 灼烧：每回合损失最大HP的1/16，物理攻击减半
			int32 BurnDamage = GetStatusDamagePerTurn(BeastData, ECQStatusEffectType::Burn);
			BeastData.CurrentStats.CurrentHP = FMath::Max(0, BeastData.CurrentStats.CurrentHP - BurnDamage);
			OutMessages.Add(FText::FromString(FString::Printf(TEXT("灼烧造成了 %d 点伤害！"), BurnDamage)));
			bCanAct = true;
		}
		break;

	case ECQStatusEffectType::Freeze:
		{
			// 冰冻：每回合20%概率解除，冰冻期间无法行动
			float ThawChance = 0.2f;
			if (FMath::FRandRange(0.0f, 1.0f) < ThawChance)
			{
				RemoveStatusEffect(BeastData);
				OutMessages.Add(FText::FromString(TEXT("冰冻解除了！")));
				bCanAct = true;
			}
			else
			{
				OutMessages.Add(FText::FromString(TEXT("被冻住了，无法行动！")));
				bCanAct = false;
			}
		}
		break;

	case ECQStatusEffectType::Paralysis:
		{
			// 麻痹：25%概率无法行动，速度减半
			float ParalyzeChance = 0.25f;
			if (FMath::FRandRange(0.0f, 1.0f) < ParalyzeChance)
			{
				OutMessages.Add(FText::FromString(TEXT("身体麻痹了，无法行动！")));
				bCanAct = false;
			}
			else
			{
				bCanAct = true;
			}
		}
		break;

	case ECQStatusEffectType::Sleep:
		{
			// 睡眠：持续期间无法行动，每回合减少剩余回合数
			if (BeastData.StatusEffectTurnsRemaining > 0)
			{
				OutMessages.Add(FText::FromString(TEXT("还在沉睡中...")));
				bCanAct = false;
			}
			else
			{
				RemoveStatusEffect(BeastData);
				OutMessages.Add(FText::FromString(TEXT("醒过来了！")));
				bCanAct = true;
			}
		}
		break;

	default:
		bCanAct = true;
		break;
	}

	// 减少状态持续回合数
	if (BeastData.StatusEffectTurnsRemaining > 0)
	{
		BeastData.StatusEffectTurnsRemaining--;
		if (BeastData.StatusEffectTurnsRemaining <= 0 &&
			BeastData.CurrentStatusEffect != ECQStatusEffectType::Freeze) // 冰冻用概率解除
		{
			FText StatusName = GetStatusEffectName(BeastData.CurrentStatusEffect);
			RemoveStatusEffect(BeastData);
			OutMessages.Add(FText::Format(
				FText::FromString(TEXT("{0} 状态解除了！")), StatusName));
		}
	}

	return bCanAct;
}

bool UCQStatusEffect::ApplyStatusEffect(FCQBeastInstanceData& BeastData,
	ECQStatusEffectType StatusType, int32 Duration)
{
	// 已有状态异常时不可叠加
	if (BeastData.CurrentStatusEffect != ECQStatusEffectType::None)
	{
		return false;
	}

	BeastData.CurrentStatusEffect = StatusType;
	BeastData.StatusEffectTurnsRemaining = Duration;

	UE_LOG(LogContractQuadrant, Log, TEXT("附加状态异常: %s, 持续 %d 回合"),
		*GetStatusEffectName(StatusType).ToString(), Duration);

	return true;
}

void UCQStatusEffect::RemoveStatusEffect(FCQBeastInstanceData& BeastData)
{
	BeastData.CurrentStatusEffect = ECQStatusEffectType::None;
	BeastData.StatusEffectTurnsRemaining = 0;
}

FText UCQStatusEffect::GetStatusEffectName(ECQStatusEffectType StatusType)
{
	switch (StatusType)
	{
	case ECQStatusEffectType::Poison:   return FText::FromString(TEXT("中毒"));
	case ECQStatusEffectType::Burn:     return FText::FromString(TEXT("灼烧"));
	case ECQStatusEffectType::Freeze:   return FText::FromString(TEXT("冰冻"));
	case ECQStatusEffectType::Paralysis: return FText::FromString(TEXT("麻痹"));
	case ECQStatusEffectType::Sleep:    return FText::FromString(TEXT("睡眠"));
	default: return FText::FromString(TEXT("无"));
	}
}

int32 UCQStatusEffect::GetStatusDamagePerTurn(const FCQBeastInstanceData& BeastData,
	ECQStatusEffectType StatusType)
{
	switch (StatusType)
	{
	case ECQStatusEffectType::Poison:
		return FMath::Max(1, BeastData.CurrentStats.MaxHP / 8);
	case ECQStatusEffectType::Burn:
		return FMath::Max(1, BeastData.CurrentStats.MaxHP / 16);
	default:
		return 0;
	}
}
