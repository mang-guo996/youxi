// Copyright (C) 2025 契约象限. All Rights Reserved.
// 可受伤接口 - 战斗中的晶兽实现此接口

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CQDamageable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UCQDamageable : public UInterface
{
	GENERATED_BODY()
};

class CONTRACTQUADRANT_API ICQDamageable
{
	GENERATED_BODY()

public:
	// 受到战斗伤害
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "战斗")
	void TakeBattleDamage(int32 DamageAmount, AActor* DamageSource);

	// 获取当前生命值
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "战斗")
	int32 GetCurrentHealth() const;

	// 获取最大生命值
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "战斗")
	int32 GetMaxHealth() const;

	// 是否已被击败
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "战斗")
	bool IsDefeated() const;

	// 回复生命值
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "战斗")
	void HealHealth(int32 HealAmount);
};
