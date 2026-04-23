// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "CrystalBeast/CQBeastManagerComponent.h"
#include "ContractQuadrant.h"

UCQBeastManagerComponent::UCQBeastManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// ============================================================================
// 队伍管理
// ============================================================================

bool UCQBeastManagerComponent::AddBeastToParty(const FCQBeastInstanceData& BeastData)
{
	// 记录发现
	RegisterDiscovery(BeastData.BeastBaseID);

	if (PartyBeasts.Num() < MaxPartySize)
	{
		FCQBeastInstanceData NewBeast = BeastData;
		if (!NewBeast.InstanceID.IsValid())
		{
			NewBeast.GenerateInstanceID();
		}
		PartyBeasts.Add(NewBeast);
		OnTeamChanged.Broadcast();

		UE_LOG(LogContractQuadrant, Log, TEXT("晶兽加入队伍: %s (队伍: %d/%d)"),
			*BeastData.BeastBaseID.ToString(), PartyBeasts.Num(), MaxPartySize);
		return true;
	}
	else
	{
		// 队伍已满，放入仓库
		FCQBeastInstanceData NewBeast = BeastData;
		if (!NewBeast.InstanceID.IsValid())
		{
			NewBeast.GenerateInstanceID();
		}
		StorageBeasts.Add(NewBeast);

		UE_LOG(LogContractQuadrant, Log, TEXT("队伍已满，晶兽存入仓库: %s"), *BeastData.BeastBaseID.ToString());
		return true;
	}
}

bool UCQBeastManagerComponent::RemoveBeastFromParty(int32 PartyIndex)
{
	// 至少保留一只晶兽在队伍中
	if (PartyBeasts.Num() <= 1)
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("无法移除最后一只队伍晶兽"));
		return false;
	}

	if (!PartyBeasts.IsValidIndex(PartyIndex))
	{
		return false;
	}

	StorageBeasts.Add(PartyBeasts[PartyIndex]);
	PartyBeasts.RemoveAt(PartyIndex);
	OnTeamChanged.Broadcast();

	return true;
}

void UCQBeastManagerComponent::SwapPartyPositions(int32 IndexA, int32 IndexB)
{
	if (PartyBeasts.IsValidIndex(IndexA) && PartyBeasts.IsValidIndex(IndexB))
	{
		PartyBeasts.Swap(IndexA, IndexB);
		OnTeamChanged.Broadcast();
	}
}

bool UCQBeastManagerComponent::WithdrawBeastFromStorage(int32 StorageIndex)
{
	if (!StorageBeasts.IsValidIndex(StorageIndex))
	{
		return false;
	}

	if (PartyBeasts.Num() >= MaxPartySize)
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("队伍已满，无法取出晶兽"));
		return false;
	}

	PartyBeasts.Add(StorageBeasts[StorageIndex]);
	StorageBeasts.RemoveAt(StorageIndex);
	OnTeamChanged.Broadcast();

	return true;
}

FCQBeastInstanceData UCQBeastManagerComponent::GetPartyBeast(int32 Index) const
{
	if (PartyBeasts.IsValidIndex(Index))
	{
		return PartyBeasts[Index];
	}
	return FCQBeastInstanceData();
}

int32 UCQBeastManagerComponent::GetFirstFightableBeastIndex() const
{
	for (int32 i = 0; i < PartyBeasts.Num(); ++i)
	{
		if (PartyBeasts[i].CanFight())
		{
			return i;
		}
	}
	return -1; // 全队覆灭
}

bool UCQBeastManagerComponent::IsPartyWipedOut() const
{
	for (const FCQBeastInstanceData& Beast : PartyBeasts)
	{
		if (Beast.CanFight())
		{
			return false;
		}
	}
	return true;
}

void UCQBeastManagerComponent::HealAllPartyBeasts()
{
	for (FCQBeastInstanceData& Beast : PartyBeasts)
	{
		Beast.CurrentStats.CurrentHP = Beast.CurrentStats.MaxHP;
		Beast.CurrentStatusEffect = ECQStatusEffectType::None;
		Beast.StatusEffectTurnsRemaining = 0;

		// 恢复所有技能PP
		for (FCQSkillInstance& Skill : Beast.LearnedSkills)
		{
			Skill.CurrentPP = Skill.MaxPP;
		}
	}

	UE_LOG(LogContractQuadrant, Log, TEXT("全队晶兽已恢复"));
}

// ============================================================================
// 捕获系统
// ============================================================================

bool UCQBeastManagerComponent::AttemptCapture(const FCQBeastInstanceData& WildBeast, float CaptureRateBonus)
{
	// 捕获概率计算公式（参考经典宝可梦公式简化版）：
	// CaptureRate = (BaseCaptureRate * HPFactor * StatusBonus * ItemBonus) / 255
	// HPFactor = (3 * MaxHP - 2 * CurrentHP) / (3 * MaxHP)
	// StatusBonus = 状态异常加成（睡眠/冰冻 = 2.0, 其他 = 1.5, 无 = 1.0）

	float MaxHP = static_cast<float>(WildBeast.CurrentStats.MaxHP);
	float CurrentHP = static_cast<float>(WildBeast.CurrentStats.CurrentHP);

	// HP因子：HP越低越容易捕获
	float HPFactor = (3.0f * MaxHP - 2.0f * CurrentHP) / (3.0f * MaxHP);
	HPFactor = FMath::Clamp(HPFactor, 0.1f, 1.0f);

	// 状态异常加成
	float StatusBonus = 1.0f;
	switch (WildBeast.CurrentStatusEffect)
	{
	case ECQStatusEffectType::Sleep:
	case ECQStatusEffectType::Freeze:
		StatusBonus = 2.0f;
		break;
	case ECQStatusEffectType::Poison:
	case ECQStatusEffectType::Burn:
	case ECQStatusEffectType::Paralysis:
		StatusBonus = 1.5f;
		break;
	default:
		StatusBonus = 1.0f;
		break;
	}

	// 基础捕获率（此处使用一个默认值，实际应从DataTable读取）
	float BaseCaptureRate = 128.0f; // 0-255

	// 最终捕获概率
	float FinalRate = (BaseCaptureRate * HPFactor * StatusBonus * CaptureRateBonus) / 255.0f;
	FinalRate = FMath::Clamp(FinalRate, 0.05f, 1.0f); // 最少5%概率

	// 随机判定
	float Roll = FMath::FRandRange(0.0f, 1.0f);
	bool bCaptured = Roll < FinalRate;

	UE_LOG(LogContractQuadrant, Log, TEXT("捕获尝试: 概率=%.2f%%, 投掷=%.2f%%, 结果=%s"),
		FinalRate * 100.0f, Roll * 100.0f, bCaptured ? TEXT("成功") : TEXT("失败"));

	if (bCaptured)
	{
		// 捕获成功
		FCQBeastInstanceData CapturedBeast = WildBeast;
		AddBeastToParty(CapturedBeast);
		OnBeastCaptured.Broadcast(CapturedBeast);
	}

	return bCaptured;
}

// ============================================================================
// 图鉴
// ============================================================================

void UCQBeastManagerComponent::RegisterDiscovery(FName BeastID)
{
	if (!DiscoveredBeastIDs.Contains(BeastID))
	{
		DiscoveredBeastIDs.Add(BeastID);
		UE_LOG(LogContractQuadrant, Log, TEXT("发现新晶兽: %s (已发现: %d)"),
			*BeastID.ToString(), DiscoveredBeastIDs.Num());
	}
}

bool UCQBeastManagerComponent::HasDiscovered(FName BeastID) const
{
	return DiscoveredBeastIDs.Contains(BeastID);
}
