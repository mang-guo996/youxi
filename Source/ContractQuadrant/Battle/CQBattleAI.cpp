// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Battle/CQBattleAI.h"
#include "Battle/CQDamageCalculation.h"
#include "ContractQuadrant.h"

FCQBattleAction UCQBattleAI::DecideAction(
	const FCQBeastInstanceData& AIBeast,
	const FCQBeastInstanceData& PlayerBeast,
	const TArray<FCQBeastInstanceData>& AIParty)
{
	FCQBattleAction Action;
	Action.bIsPlayerAction = false;
	Action.ActionSpeed = AIBeast.CurrentStats.Speed;

	// 难度1（简单）：随机选择技能
	if (DifficultyLevel <= 1)
	{
		Action.ActionType = ECQBattleActionType::Attack;
		if (AIBeast.LearnedSkills.Num() > 0)
		{
			// 随机选择有PP的技能
			TArray<int32> ValidSkills;
			for (int32 i = 0; i < AIBeast.LearnedSkills.Num(); ++i)
			{
				if (AIBeast.LearnedSkills[i].CurrentPP > 0)
				{
					ValidSkills.Add(i);
				}
			}
			if (ValidSkills.Num() > 0)
			{
				Action.SkillIndex = ValidSkills[FMath::RandRange(0, ValidSkills.Num() - 1)];
			}
		}
		return Action;
	}

	// 难度2+：策略性选择
	// 1. 检查是否应该切换晶兽（低HP且有更好的选择）
	if (DifficultyLevel >= 3 && ShouldSwitchBeast(AIBeast, PlayerBeast, AIParty))
	{
		int32 CurrentIndex = INDEX_NONE;
		for (int32 i = 0; i < AIParty.Num(); ++i)
		{
			if (AIParty[i].InstanceID == AIBeast.InstanceID)
			{
				CurrentIndex = i;
				break;
			}
		}

		int32 SwitchTarget = FindBestSwitchTarget(PlayerBeast, AIParty, CurrentIndex);
		if (SwitchTarget != INDEX_NONE)
		{
			Action.ActionType = ECQBattleActionType::SwitchBeast;
			Action.SwitchBeastIndex = SwitchTarget;
			return Action;
		}
	}

	// 2. 选择最佳技能
	Action.ActionType = ECQBattleActionType::Attack;
	Action.SkillIndex = SelectBestSkill(AIBeast, PlayerBeast);

	return Action;
}

int32 UCQBattleAI::SelectBestSkill(const FCQBeastInstanceData& AIBeast,
	const FCQBeastInstanceData& PlayerBeast)
{
	if (AIBeast.LearnedSkills.Num() == 0)
	{
		return 0;
	}

	int32 BestSkillIndex = 0;
	float BestScore = -1.0f;

	for (int32 i = 0; i < AIBeast.LearnedSkills.Num(); ++i)
	{
		const FCQSkillInstance& Skill = AIBeast.LearnedSkills[i];
		if (Skill.CurrentPP <= 0) continue;

		float Score = 0.0f;

		// 基础分数 = 技能威力（需要从DataTable读取，此处使用默认值）
		Score += 40.0f; // 默认威力

		// 属性克制加成
		// 注意：实际使用时需要获取技能属性和对方晶兽属性
		// 此处简化处理

		// 状态异常附加加成（普通难度以上考虑）
		if (DifficultyLevel >= 2)
		{
			// 如果对方没有状态异常，优先考虑带状态效果的技能
			if (PlayerBeast.CurrentStatusEffect == ECQStatusEffectType::None)
			{
				Score += 10.0f; // 状态技能加分
			}
		}

		// 低PP技能降分（保留后用）
		if (DifficultyLevel >= 2 && Skill.CurrentPP <= 2)
		{
			Score *= 0.5f;
		}

		// 添加少量随机性
		Score += FMath::FRandRange(0.0f, 10.0f);

		if (Score > BestScore)
		{
			BestScore = Score;
			BestSkillIndex = i;
		}
	}

	return BestSkillIndex;
}

bool UCQBattleAI::ShouldSwitchBeast(const FCQBeastInstanceData& AIBeast,
	const FCQBeastInstanceData& PlayerBeast,
	const TArray<FCQBeastInstanceData>& AIParty)
{
	// HP低于30%时考虑切换
	float HPPercent = static_cast<float>(AIBeast.CurrentStats.CurrentHP) /
		static_cast<float>(AIBeast.CurrentStats.MaxHP);

	if (HPPercent > 0.3f)
	{
		return false;
	}

	// 检查是否有可切换的晶兽
	for (const FCQBeastInstanceData& Beast : AIParty)
	{
		if (Beast.InstanceID != AIBeast.InstanceID && Beast.CanFight())
		{
			return true;
		}
	}

	return false;
}

int32 UCQBattleAI::FindBestSwitchTarget(const FCQBeastInstanceData& PlayerBeast,
	const TArray<FCQBeastInstanceData>& AIParty, int32 CurrentIndex)
{
	int32 BestIndex = INDEX_NONE;
	float BestScore = -1.0f;

	for (int32 i = 0; i < AIParty.Num(); ++i)
	{
		if (i == CurrentIndex || !AIParty[i].CanFight())
		{
			continue;
		}

		float Score = 0.0f;

		// HP越高越好
		Score += static_cast<float>(AIParty[i].CurrentStats.CurrentHP) /
			static_cast<float>(AIParty[i].CurrentStats.MaxHP) * 100.0f;

		// 属性克制加分（需要知道双方属性，此处简化）
		Score += FMath::FRandRange(0.0f, 20.0f);

		if (Score > BestScore)
		{
			BestScore = Score;
			BestIndex = i;
		}
	}

	return BestIndex;
}
