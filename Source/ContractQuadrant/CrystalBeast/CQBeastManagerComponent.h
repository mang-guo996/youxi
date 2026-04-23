// Copyright (C) 2025 契约象限. All Rights Reserved.
// 晶兽管理组件 - 挂载在玩家角色上，管理队伍和仓库

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrystalBeast/CQBeastDataTypes.h"
#include "CQBeastManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeastCaptured, const FCQBeastInstanceData&, NewBeast);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeamChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONTRACTQUADRANT_API UCQBeastManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCQBeastManagerComponent();

	// ========== 队伍管理 ==========

	// 添加晶兽到队伍（满则放入仓库）
	UFUNCTION(BlueprintCallable, Category = "晶兽管理")
	bool AddBeastToParty(const FCQBeastInstanceData& BeastData);

	// 从队伍中移除晶兽（移到仓库）
	UFUNCTION(BlueprintCallable, Category = "晶兽管理")
	bool RemoveBeastFromParty(int32 PartyIndex);

	// 交换队伍中晶兽的位置
	UFUNCTION(BlueprintCallable, Category = "晶兽管理")
	void SwapPartyPositions(int32 IndexA, int32 IndexB);

	// 从仓库中取出晶兽到队伍
	UFUNCTION(BlueprintCallable, Category = "晶兽管理")
	bool WithdrawBeastFromStorage(int32 StorageIndex);

	// 获取队伍中的晶兽数据
	UFUNCTION(BlueprintCallable, Category = "晶兽管理")
	FCQBeastInstanceData GetPartyBeast(int32 Index) const;

	// 获取队伍数量
	UFUNCTION(BlueprintCallable, Category = "晶兽管理")
	int32 GetPartyCount() const { return PartyBeasts.Num(); }

	// 获取第一只可战斗的晶兽索引
	UFUNCTION(BlueprintCallable, Category = "晶兽管理")
	int32 GetFirstFightableBeastIndex() const;

	// 是否全队覆灭
	UFUNCTION(BlueprintCallable, Category = "晶兽管理")
	bool IsPartyWipedOut() const;

	// 全队恢复
	UFUNCTION(BlueprintCallable, Category = "晶兽管理")
	void HealAllPartyBeasts();

	// ========== 捕获系统 ==========

	// 尝试捕获野生晶兽
	UFUNCTION(BlueprintCallable, Category = "捕获")
	bool AttemptCapture(const FCQBeastInstanceData& WildBeast, float CaptureRateBonus = 1.0f);

	// ========== 图鉴 ==========

	// 记录发现晶兽
	UFUNCTION(BlueprintCallable, Category = "图鉴")
	void RegisterDiscovery(FName BeastID);

	// 检查是否已发现
	UFUNCTION(BlueprintCallable, Category = "图鉴")
	bool HasDiscovered(FName BeastID) const;

	// 已发现晶兽数量
	UFUNCTION(BlueprintCallable, Category = "图鉴")
	int32 GetDiscoveredCount() const { return DiscoveredBeastIDs.Num(); }

	// ========== 委托 ==========

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnBeastCaptured OnBeastCaptured;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnTeamChanged OnTeamChanged;

	// ========== 数据 ==========

	// 队伍晶兽（最多6只）
	UPROPERTY(BlueprintReadOnly, Category = "晶兽")
	TArray<FCQBeastInstanceData> PartyBeasts;

	// 仓库晶兽
	UPROPERTY(BlueprintReadOnly, Category = "晶兽")
	TArray<FCQBeastInstanceData> StorageBeasts;

	// 已发现晶兽ID
	UPROPERTY(BlueprintReadOnly, Category = "图鉴")
	TArray<FName> DiscoveredBeastIDs;

	// 队伍最大数量
	static constexpr int32 MaxPartySize = 6;
};
