// Copyright (C) 2025 契约象限. All Rights Reserved.
// 可存储接口 - 需要持久化的对象实现此接口

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CQSaveable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UCQSaveable : public UInterface
{
	GENERATED_BODY()
};

class CONTRACTQUADRANT_API ICQSaveable
{
	GENERATED_BODY()

public:
	// 获取需要保存的数据（序列化为字符串）
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "存档")
	FString GetSaveData() const;

	// 从存档数据恢复状态
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "存档")
	void LoadSaveData(const FString& SaveData);

	// 获取存储唯一标识
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "存档")
	FName GetSaveID() const;
};
