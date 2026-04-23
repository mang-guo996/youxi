// Copyright (C) 2025 契约象限. All Rights Reserved.
// 世界管理器 - 场景加载与区域管理

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/CQTypes.h"
#include "CQWorldManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAreaChanged, ECQWorldArea, NewArea);

UCLASS()
class CONTRACTQUADRANT_API ACQWorldManager : public AActor
{
	GENERATED_BODY()

public:
	ACQWorldManager();

	// 获取区域显示名称
	UFUNCTION(BlueprintCallable, Category = "世界")
	static FText GetAreaDisplayName(ECQWorldArea Area);

	// 获取区域描述
	UFUNCTION(BlueprintCallable, Category = "世界")
	static FText GetAreaDescription(ECQWorldArea Area);

	// 区域变更事件
	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnAreaChanged OnAreaChanged;
};
