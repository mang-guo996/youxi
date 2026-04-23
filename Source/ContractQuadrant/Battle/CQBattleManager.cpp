// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Battle/CQBattleManager.h"
#include "Battle/CQDamageCalculation.h"
#include "Battle/CQStatusEffect.h"
#include "Battle/CQBattleAI.h"
#include "Engine/DataTable.h"
#include "ContractQuadrant.h"

ACQBattleManager::ACQBattleManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

// ============================================================================
// 战斗初始化
// ============================================================================

void ACQBattleManager::InitWildBattle(const TArray<FCQBeastInstanceData>& InPlayerParty,
	FName WildBeastID, int32 WildBeastLevel)
{
	PlayerParty = InPlayerParty;
	bIsWildBattle = true;
	FleeAttempts = 0;

	// 从DataTable获取野生晶兽基础数据并创建实例
	FCQBeastInstanceData WildBeast;
	WildBeast.BeastBaseID = WildBeastID;
	WildBeast.Level = WildBeastLevel;
	WildBeast.GenerateInstanceID();

	// 从DataTable读取基础数据，计算属性
	if (BeastDataTable)
	{
		FCQBeastBaseData* BaseData = BeastDataTable->FindRow<FCQBeastBaseData>(WildBeastID, TEXT("WildBattle"));
		if (BaseData)
		{
			// 计算等级对应属性
			float LevelMult = static_cast<float>(WildBeastLevel - 1);
			WildBeast.CurrentStats.MaxHP = FMath::RoundToInt(BaseData->BaseStats.MaxHP * (1.0f + BaseData->HPGrowthRate * LevelMult));
			WildBeast.CurrentStats.CurrentHP = WildBeast.CurrentStats.MaxHP;
			WildBeast.CurrentStats.Attack = FMath::RoundToInt(BaseData->BaseStats.Attack * (1.0f + BaseData->AttackGrowthRate * LevelMult));
			WildBeast.CurrentStats.Defense = FMath::RoundToInt(BaseData->BaseStats.Defense * (1.0f + BaseData->DefenseGrowthRate * LevelMult));
			WildBeast.CurrentStats.Speed = FMath::RoundToInt(BaseData->BaseStats.Speed * (1.0f + BaseData->SpeedGrowthRate * LevelMult));
			WildBeast.CurrentStats.SpecialAttack = FMath::RoundToInt(BaseData->BaseStats.SpecialAttack * (1.0f + BaseData->SpAttackGrowthRate * LevelMult));
			WildBeast.CurrentStats.SpecialDefense = FMath::RoundToInt(BaseData->BaseStats.SpecialDefense * (1.0f + BaseData->SpDefenseGrowthRate * LevelMult));

			// 初始化技能
			for (const FCQLearnableSkill& Learnable : BaseData->LearnableSkills)
			{
				if (Learnable.LearnLevel <= WildBeastLevel && WildBeast.LearnedSkills.Num() < 4)
				{
					FCQSkillInstance Skill;
					Skill.SkillID = Learnable.SkillID;
					Skill.CurrentPP = 20;
					Skill.MaxPP = 20;
					WildBeast.LearnedSkills.Add(Skill);
				}
			}
		}
	}

	EnemyParty.Empty();
	EnemyParty.Add(WildBeast);

	// 初始化战斗AI
	BattleAI = NewObject<UCQBattleAI>(this);

	// 初始化回合信息
	TurnInfo.TurnNumber = 0;
	TurnInfo.PlayerActiveBeastIndex = 0;
	TurnInfo.EnemyActiveBeastIndex = 0;

	SetPhase(ECQBattlePhase::TurnStart);
	BroadcastMessage(FText::FromString(FString::Printf(
		TEXT("野生的 %s 出现了！"), *WildBeastID.ToString())));

	StartTurn();
}

void ACQBattleManager::InitTrainerBattle(const TArray<FCQBeastInstanceData>& InPlayerParty,
	const TArray<FName>& EnemyBeastIDs, const TArray<int32>& EnemyBeastLevels)
{
	PlayerParty = InPlayerParty;
	bIsWildBattle = false;
	FleeAttempts = 0;

	EnemyParty.Empty();
	for (int32 i = 0; i < EnemyBeastIDs.Num(); ++i)
	{
		FCQBeastInstanceData EnemyBeast;
		EnemyBeast.BeastBaseID = EnemyBeastIDs[i];
		EnemyBeast.Level = (i < EnemyBeastLevels.Num()) ? EnemyBeastLevels[i] : 1;
		EnemyBeast.GenerateInstanceID();

		// 从DataTable初始化（同上简化逻辑）
		if (BeastDataTable)
		{
			FCQBeastBaseData* BaseData = BeastDataTable->FindRow<FCQBeastBaseData>(EnemyBeastIDs[i], TEXT("TrainerBattle"));
			if (BaseData)
			{
				float LevelMult = static_cast<float>(EnemyBeast.Level - 1);
				EnemyBeast.CurrentStats.MaxHP = FMath::RoundToInt(BaseData->BaseStats.MaxHP * (1.0f + BaseData->HPGrowthRate * LevelMult));
				EnemyBeast.CurrentStats.CurrentHP = EnemyBeast.CurrentStats.MaxHP;
				EnemyBeast.CurrentStats.Attack = FMath::RoundToInt(BaseData->BaseStats.Attack * (1.0f + BaseData->AttackGrowthRate * LevelMult));
				EnemyBeast.CurrentStats.Defense = FMath::RoundToInt(BaseData->BaseStats.Defense * (1.0f + BaseData->DefenseGrowthRate * LevelMult));
				EnemyBeast.CurrentStats.Speed = FMath::RoundToInt(BaseData->BaseStats.Speed * (1.0f + BaseData->SpeedGrowthRate * LevelMult));
				EnemyBeast.CurrentStats.SpecialAttack = FMath::RoundToInt(BaseData->BaseStats.SpecialAttack * (1.0f + BaseData->SpAttackGrowthRate * LevelMult));
				EnemyBeast.CurrentStats.SpecialDefense = FMath::RoundToInt(BaseData->BaseStats.SpecialDefense * (1.0f + BaseData->SpDefenseGrowthRate * LevelMult));

				for (const FCQLearnableSkill& Learnable : BaseData->LearnableSkills)
				{
					if (Learnable.LearnLevel <= EnemyBeast.Level && EnemyBeast.LearnedSkills.Num() < 4)
					{
						FCQSkillInstance Skill;
						Skill.SkillID = Learnable.SkillID;
						Skill.CurrentPP = 20;
						Skill.MaxPP = 20;
						EnemyBeast.LearnedSkills.Add(Skill);
					}
				}
			}
		}

		EnemyParty.Add(EnemyBeast);
	}

	BattleAI = NewObject<UCQBattleAI>(this);

	TurnInfo.TurnNumber = 0;
	TurnInfo.PlayerActiveBeastIndex = 0;
	TurnInfo.EnemyActiveBeastIndex = 0;

	SetPhase(ECQBattlePhase::TurnStart);
	BroadcastMessage(FText::FromString(TEXT("训练师发起了挑战！")));

	StartTurn();
}

// ============================================================================
// 战斗控制
// ============================================================================

void ACQBattleManager::PlayerSelectAction(const FCQBattleAction& Action)
{
	if (CurrentPhase != ECQBattlePhase::PlayerSelect)
	{
		return;
	}

	PlayerAction = Action;
	PlayerAction.bIsPlayerAction = true;

	// 获取玩家晶兽速度
	const FCQBeastInstanceData& PlayerBeast = PlayerParty[TurnInfo.PlayerActiveBeastIndex];
	PlayerAction.ActionSpeed = PlayerBeast.CurrentStats.Speed;

	// AI决定行动
	if (BattleAI)
	{
		const FCQBeastInstanceData& EnemyBeast = EnemyParty[TurnInfo.EnemyActiveBeastIndex];
		EnemyAction = BattleAI->DecideAction(EnemyBeast, PlayerBeast, EnemyParty);
		EnemyAction.bIsPlayerAction = false;
		EnemyAction.ActionSpeed = EnemyBeast.CurrentStats.Speed;
	}

	// 进入执行阶段
	SetPhase(ECQBattlePhase::ActionExecution);
	ExecuteTurn();
}

void ACQBattleManager::ExecuteTurn()
{
	TurnInfo.TurnLog.Empty();

	// 确定行动顺序（速度高者先行动，逃跑/切换优先）
	bool bPlayerFirst = true;

	// 逃跑和切换优先级最高
	if (PlayerAction.ActionType == ECQBattleActionType::SwitchBeast ||
		PlayerAction.ActionType == ECQBattleActionType::Flee)
	{
		bPlayerFirst = true;
	}
	else if (EnemyAction.ActionType == ECQBattleActionType::SwitchBeast)
	{
		bPlayerFirst = false;
	}
	else
	{
		// 按速度排序
		bPlayerFirst = PlayerAction.ActionSpeed >= EnemyAction.ActionSpeed;
		// 速度相同随机
		if (PlayerAction.ActionSpeed == EnemyAction.ActionSpeed)
		{
			bPlayerFirst = FMath::RandBool();
		}
	}

	// 执行行动
	if (bPlayerFirst)
	{
		ExecuteAction(PlayerAction, true);
		CheckBattleEnd();
		if (CurrentPhase != ECQBattlePhase::BattleEnd)
		{
			ExecuteAction(EnemyAction, false);
			CheckBattleEnd();
		}
	}
	else
	{
		ExecuteAction(EnemyAction, false);
		CheckBattleEnd();
		if (CurrentPhase != ECQBattlePhase::BattleEnd)
		{
			ExecuteAction(PlayerAction, true);
			CheckBattleEnd();
		}
	}

	// 回合结束处理
	if (CurrentPhase != ECQBattlePhase::BattleEnd)
	{
		ProcessTurnEndEffects();
		CheckBattleEnd();

		if (CurrentPhase != ECQBattlePhase::BattleEnd)
		{
			StartTurn();
		}
	}
}

void ACQBattleManager::SwitchPlayerBeast(int32 PartyIndex)
{
	if (!PlayerParty.IsValidIndex(PartyIndex) || !PlayerParty[PartyIndex].CanFight())
	{
		return;
	}

	TurnInfo.PlayerActiveBeastIndex = PartyIndex;
	OnTurnInfoUpdated.Broadcast(TurnInfo);

	BroadcastMessage(FText::FromString(FString::Printf(
		TEXT("切换为 %s！"), *PlayerParty[PartyIndex].BeastBaseID.ToString())));
}

bool ACQBattleManager::AttemptFlee()
{
	if (!bIsWildBattle)
	{
		BroadcastMessage(FText::FromString(TEXT("无法从训练师战斗中逃跑！")));
		return false;
	}

	FleeAttempts++;
	int32 PlayerSpeed = PlayerParty[TurnInfo.PlayerActiveBeastIndex].CurrentStats.Speed;
	int32 EnemySpeed = EnemyParty[TurnInfo.EnemyActiveBeastIndex].CurrentStats.Speed;

	bool bFled = UCQDamageCalculation::RollFleeSuccess(PlayerSpeed, EnemySpeed, FleeAttempts);

	if (bFled)
	{
		BroadcastMessage(FText::FromString(TEXT("成功逃跑了！")));
		EndBattle(ECQBattleResult::PlayerFled);
	}
	else
	{
		BroadcastMessage(FText::FromString(TEXT("逃跑失败！")));
	}

	return bFled;
}

bool ACQBattleManager::AttemptCapture(FName CaptureItemID)
{
	if (!bIsWildBattle)
	{
		BroadcastMessage(FText::FromString(TEXT("无法捕获训练师的晶兽！")));
		return false;
	}

	// 获取捕获道具加成
	float CaptureBonus = 1.0f;
	if (ItemDataTable)
	{
		FCQItemData* ItemData = ItemDataTable->FindRow<FCQItemData>(CaptureItemID, TEXT("Capture"));
		if (ItemData && ItemData->bIsCaptureItem)
		{
			CaptureBonus = ItemData->CaptureRateBonus;
		}
	}

	const FCQBeastInstanceData& WildBeast = EnemyParty[TurnInfo.EnemyActiveBeastIndex];

	// 计算捕获概率
	float MaxHP = static_cast<float>(WildBeast.CurrentStats.MaxHP);
	float CurrentHP = static_cast<float>(WildBeast.CurrentStats.CurrentHP);
	float HPFactor = (3.0f * MaxHP - 2.0f * CurrentHP) / (3.0f * MaxHP);

	float StatusBonus = 1.0f;
	if (WildBeast.CurrentStatusEffect == ECQStatusEffectType::Sleep ||
		WildBeast.CurrentStatusEffect == ECQStatusEffectType::Freeze)
	{
		StatusBonus = 2.0f;
	}
	else if (WildBeast.CurrentStatusEffect != ECQStatusEffectType::None)
	{
		StatusBonus = 1.5f;
	}

	float FinalRate = FMath::Clamp(HPFactor * StatusBonus * CaptureBonus * 0.5f, 0.05f, 0.95f);
	bool bCaptured = FMath::FRandRange(0.0f, 1.0f) < FinalRate;

	BroadcastMessage(FText::FromString(TEXT("投出了晶能球...")));

	if (bCaptured)
	{
		BroadcastMessage(FText::FromString(TEXT("捕获成功！")));
		EndBattle(ECQBattleResult::Captured);
	}
	else
	{
		BroadcastMessage(FText::FromString(TEXT("晶兽挣脱了！")));
	}

	return bCaptured;
}

// ============================================================================
// 查询接口
// ============================================================================

FCQBeastInstanceData ACQBattleManager::GetPlayerActiveBeast() const
{
	if (PlayerParty.IsValidIndex(TurnInfo.PlayerActiveBeastIndex))
	{
		return PlayerParty[TurnInfo.PlayerActiveBeastIndex];
	}
	return FCQBeastInstanceData();
}

FCQBeastInstanceData ACQBattleManager::GetEnemyActiveBeast() const
{
	if (EnemyParty.IsValidIndex(TurnInfo.EnemyActiveBeastIndex))
	{
		return EnemyParty[TurnInfo.EnemyActiveBeastIndex];
	}
	return FCQBeastInstanceData();
}

TArray<FCQSkillInstance> ACQBattleManager::GetPlayerAvailableSkills() const
{
	if (PlayerParty.IsValidIndex(TurnInfo.PlayerActiveBeastIndex))
	{
		return PlayerParty[TurnInfo.PlayerActiveBeastIndex].LearnedSkills;
	}
	return TArray<FCQSkillInstance>();
}

// ============================================================================
// 内部方法
// ============================================================================

void ACQBattleManager::SetPhase(ECQBattlePhase NewPhase)
{
	CurrentPhase = NewPhase;
	OnBattlePhaseChanged.Broadcast(NewPhase);
}

void ACQBattleManager::StartTurn()
{
	TurnInfo.TurnNumber++;
	TurnInfo.TurnLog.Empty();

	UE_LOG(LogContractQuadrant, Log, TEXT("===== 第 %d 回合 ====="), TurnInfo.TurnNumber);

	// 处理回合开始的状态异常
	TArray<FText> PlayerStatusMessages;
	bool bPlayerCanAct = UCQStatusEffect::ProcessStatusEffect(
		PlayerParty[TurnInfo.PlayerActiveBeastIndex], PlayerStatusMessages);

	for (const FText& Msg : PlayerStatusMessages)
	{
		BroadcastMessage(Msg);
	}

	TArray<FText> EnemyStatusMessages;
	bool bEnemyCanAct = UCQStatusEffect::ProcessStatusEffect(
		EnemyParty[TurnInfo.EnemyActiveBeastIndex], EnemyStatusMessages);

	for (const FText& Msg : EnemyStatusMessages)
	{
		BroadcastMessage(Msg);
	}

	// 检查状态异常造成的击败
	CheckBattleEnd();
	if (CurrentPhase == ECQBattlePhase::BattleEnd)
	{
		return;
	}

	// 进入玩家选择阶段
	SetPhase(ECQBattlePhase::PlayerSelect);
	OnTurnInfoUpdated.Broadcast(TurnInfo);
}

void ACQBattleManager::ExecuteAction(const FCQBattleAction& Action, bool bIsPlayerTurn)
{
	switch (Action.ActionType)
	{
	case ECQBattleActionType::Attack:
		ExecuteAttack(bIsPlayerTurn, Action.SkillIndex);
		break;

	case ECQBattleActionType::SwitchBeast:
		if (bIsPlayerTurn)
		{
			SwitchPlayerBeast(Action.SwitchBeastIndex);
		}
		break;

	case ECQBattleActionType::Flee:
		if (bIsPlayerTurn)
		{
			AttemptFlee();
		}
		break;

	case ECQBattleActionType::Capture:
		if (bIsPlayerTurn)
		{
			AttemptCapture(Action.ItemID);
		}
		break;

	case ECQBattleActionType::UseItem:
		// 使用道具逻辑
		if (bIsPlayerTurn)
		{
			BroadcastMessage(FText::FromString(TEXT("使用了道具！")));
		}
		break;
	}
}

void ACQBattleManager::ExecuteAttack(bool bIsPlayerAttack, int32 SkillIndex)
{
	FCQBeastInstanceData& Attacker = bIsPlayerAttack ?
		PlayerParty[TurnInfo.PlayerActiveBeastIndex] :
		EnemyParty[TurnInfo.EnemyActiveBeastIndex];

	FCQBeastInstanceData& Defender = bIsPlayerAttack ?
		EnemyParty[TurnInfo.EnemyActiveBeastIndex] :
		PlayerParty[TurnInfo.PlayerActiveBeastIndex];

	if (!Attacker.CanFight() || !Attacker.LearnedSkills.IsValidIndex(SkillIndex))
	{
		return;
	}

	FCQSkillInstance& SkillInst = Attacker.LearnedSkills[SkillIndex];

	// 检查PP
	if (SkillInst.CurrentPP <= 0)
	{
		BroadcastMessage(FText::FromString(TEXT("技能PP不足！")));
		return;
	}

	// 消耗PP
	SkillInst.CurrentPP--;

	// 获取技能数据
	FCQBeastSkillData SkillData = GetSkillData(SkillInst.SkillID);

	// 计算伤害
	FCQDamageResult DamageResult = UCQDamageCalculation::CalculateDamage(
		Attacker, Defender, SkillData, ElementCounterTable);

	// 构建消息
	FString AttackMsg = FString::Printf(TEXT("%s 使用了 %s！"),
		*Attacker.BeastBaseID.ToString(), *SkillInst.SkillID.ToString());
	BroadcastMessage(FText::FromString(AttackMsg));

	if (!DamageResult.bHit)
	{
		BroadcastMessage(FText::FromString(TEXT("攻击没有命中！")));
		return;
	}

	// 应用伤害
	Defender.CurrentStats.CurrentHP = FMath::Max(0,
		Defender.CurrentStats.CurrentHP - DamageResult.FinalDamage);

	BroadcastMessage(FText::FromString(FString::Printf(
		TEXT("造成了 %d 点伤害！"), DamageResult.FinalDamage)));

	// 暴击提示
	if (DamageResult.bIsCritical)
	{
		BroadcastMessage(FText::FromString(TEXT("暴击！")));
	}

	// 属性克制提示
	if (!DamageResult.EffectivenessText.IsEmpty())
	{
		BroadcastMessage(DamageResult.EffectivenessText);
	}

	// 状态异常附加
	if (DamageResult.bAppliedStatus)
	{
		UCQStatusEffect::ApplyStatusEffect(Defender, DamageResult.AppliedStatusType);
		FText StatusName = UCQStatusEffect::GetStatusEffectName(DamageResult.AppliedStatusType);
		BroadcastMessage(FText::Format(
			FText::FromString(TEXT("对方陷入了{0}状态！")), StatusName));
	}

	OnDamageDealt.Broadcast(DamageResult);
}

void ACQBattleManager::CheckBattleEnd()
{
	// 检查敌方是否全灭
	bool bAllEnemyDefeated = true;
	for (const FCQBeastInstanceData& Beast : EnemyParty)
	{
		if (Beast.CanFight())
		{
			bAllEnemyDefeated = false;
			break;
		}
	}

	if (bAllEnemyDefeated)
	{
		BroadcastMessage(FText::FromString(TEXT("战斗胜利！")));
		EndBattle(ECQBattleResult::PlayerWin);
		return;
	}

	// 检查敌方当前晶兽是否需要换人
	if (!EnemyParty[TurnInfo.EnemyActiveBeastIndex].CanFight())
	{
		for (int32 i = 0; i < EnemyParty.Num(); ++i)
		{
			if (EnemyParty[i].CanFight())
			{
				TurnInfo.EnemyActiveBeastIndex = i;
				BroadcastMessage(FText::FromString(FString::Printf(
					TEXT("对方派出了 %s！"), *EnemyParty[i].BeastBaseID.ToString())));
				break;
			}
		}
	}

	// 检查玩家是否全灭
	bool bAllPlayerDefeated = true;
	for (const FCQBeastInstanceData& Beast : PlayerParty)
	{
		if (Beast.CanFight())
		{
			bAllPlayerDefeated = false;
			break;
		}
	}

	if (bAllPlayerDefeated)
	{
		BroadcastMessage(FText::FromString(TEXT("所有晶兽都倒下了...")));
		EndBattle(ECQBattleResult::PlayerLose);
		return;
	}

	// 检查玩家当前晶兽是否需要换人
	if (!PlayerParty[TurnInfo.PlayerActiveBeastIndex].CanFight())
	{
		// 需要玩家选择下一只晶兽（通过UI触发SwitchPlayerBeast）
		BroadcastMessage(FText::FromString(TEXT("请选择下一只出战的晶兽！")));
	}
}

void ACQBattleManager::EndBattle(ECQBattleResult Result)
{
	SetPhase(ECQBattlePhase::BattleEnd);

	if (Result == ECQBattleResult::PlayerWin)
	{
		// 分配经验值
		int32 TotalExp = 0;
		for (const FCQBeastInstanceData& Enemy : EnemyParty)
		{
			TotalExp += 50 * Enemy.Level; // 简化的经验计算
		}

		BroadcastMessage(FText::FromString(FString::Printf(
			TEXT("获得了 %d 点经验值！"), TotalExp)));
	}

	OnBattleEnded.Broadcast(Result);
	UE_LOG(LogContractQuadrant, Log, TEXT("战斗结束: 结果=%d"), static_cast<int32>(Result));
}

void ACQBattleManager::ProcessTurnEndEffects()
{
	// 回合结束时的状态异常持续伤害已在StartTurn中处理
	// 此处可添加其他回合结束效果（如天气、场地效果等）
}

void ACQBattleManager::BroadcastMessage(const FText& Message)
{
	TurnInfo.TurnLog.Add(Message);
	OnBattleMessage.Broadcast(Message);
	UE_LOG(LogContractQuadrant, Log, TEXT("[战斗] %s"), *Message.ToString());
}

FCQBeastSkillData ACQBattleManager::GetSkillData(FName SkillID) const
{
	if (SkillDataTable)
	{
		FCQBeastSkillData* Data = SkillDataTable->FindRow<FCQBeastSkillData>(SkillID, TEXT("SkillLookup"));
		if (Data)
		{
			return *Data;
		}
	}

	// 返回默认技能数据
	FCQBeastSkillData DefaultSkill;
	DefaultSkill.SkillID = SkillID;
	DefaultSkill.SkillName = FText::FromString(TEXT("普通攻击"));
	DefaultSkill.Power = 40;
	DefaultSkill.Accuracy = 100;
	return DefaultSkill;
}
