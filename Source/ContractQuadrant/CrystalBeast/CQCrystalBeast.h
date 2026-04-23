// Copyright (C) 2025 契约象限. All Rights Reserved.
// 晶兽Actor - 世界中可见的晶兽实体

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CrystalBeast/CQBeastDataTypes.h"
#include "Interfaces/CQDamageable.h"
#include "CQCrystalBeast.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeastHealthChanged, int32, CurrentHP, int32, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeastDefeated, ACQCrystalBeast*, DefeatedBeast);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeastLevelUp, ACQCrystalBeast*, Beast, int32, NewLevel);

UCLASS()
class CONTRACTQUADRANT_API ACQCrystalBeast : public ACharacter, public ICQDamageable
{
	GENERATED_BODY()

public:
	ACQCrystalBeast();

protected:
	virtual void BeginPlay() override;

public:
	// ========== 数据初始化 ==========

	// 通过基础数据和等级初始化晶兽（用于野生晶兽生成）
	UFUNCTION(BlueprintCallable, Category = "晶兽")
	void InitializeFromBaseData(const FCQBeastBaseData& BaseData, int32 Level);

	// 通过实例数据初始化（用于加载已有晶兽）
	UFUNCTION(BlueprintCallable, Category = "晶兽")
	void InitializeFromInstanceData(const FCQBeastInstanceData& InstanceData);

	// 获取实例数据
	UFUNCTION(BlueprintCallable, Category = "晶兽")
	FCQBeastInstanceData GetInstanceData() const { return BeastData; }

	// 获取显示名称
	UFUNCTION(BlueprintCallable, Category = "晶兽")
	FText GetDisplayName() const;

	// ========== 战斗相关 ==========

	// 使用技能
	UFUNCTION(BlueprintCallable, Category = "战斗")
	int32 UseSkill(int32 SkillIndex, ACQCrystalBeast* Target);

	// ICQDamageable 接口实现
	virtual void TakeBattleDamage_Implementation(int32 DamageAmount, AActor* DamageSource) override;
	virtual int32 GetCurrentHealth_Implementation() const override;
	virtual int32 GetMaxHealth_Implementation() const override;
	virtual bool IsDefeated_Implementation() const override;
	virtual void HealHealth_Implementation(int32 HealAmount) override;

	// ========== 成长系统 ==========

	// 获得经验值
	UFUNCTION(BlueprintCallable, Category = "成长")
	void GainExperience(int32 ExpAmount);

	// 全恢复（回复所有HP和PP）
	UFUNCTION(BlueprintCallable, Category = "回复")
	void FullRestore();

	// ========== 委托 ==========

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnBeastHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnBeastDefeated OnDefeated;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnBeastLevelUp OnLevelUp;

	// ========== 数据 ==========

	// 晶兽实例数据
	UPROPERTY(BlueprintReadOnly, Category = "晶兽")
	FCQBeastInstanceData BeastData;

	// 晶兽基础数据引用（从DataTable读取后缓存）
	UPROPERTY(BlueprintReadOnly, Category = "晶兽")
	FCQBeastBaseData CachedBaseData;

	// 是否已初始化
	UPROPERTY(BlueprintReadOnly, Category = "晶兽")
	bool bIsInitialized = false;

protected:
	// 根据等级计算属性
	void CalculateStatsForLevel(int32 Level);

	// 检查升级
	void CheckLevelUp();

	// 计算升级所需经验
	int32 CalculateExpForLevel(int32 Level) const;
};
