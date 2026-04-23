// Copyright (C) 2025 契约象限. All Rights Reserved.
// 可交互接口 - NPC、宝箱、告示牌等实现此接口

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CQInteractable.generated.h"

class ACQPlayerCharacter;

UINTERFACE(MinimalAPI, BlueprintType)
class UCQInteractable : public UInterface
{
	GENERATED_BODY()
};

class CONTRACTQUADRANT_API ICQInteractable
{
	GENERATED_BODY()

public:
	// 执行交互（当玩家按下交互键时调用）
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "交互")
	void Interact(ACQPlayerCharacter* Player);

	// 获取交互提示文本（显示在UI上）
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "交互")
	FText GetInteractText() const;

	// 是否可以交互（用于判断是否显示交互提示）
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "交互")
	bool CanInteract(ACQPlayerCharacter* Player) const;
};
