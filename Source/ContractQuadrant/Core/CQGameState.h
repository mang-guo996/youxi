// Copyright (C) 2025 契约象限. All Rights Reserved.
// 游戏状态 - 运行时全局状态追踪

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Core/CQTypes.h"
#include "CQGameState.generated.h"

UCLASS()
class CONTRACTQUADRANT_API ACQGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACQGameState();

	// 游戏运行时间（秒）
	UPROPERTY(BlueprintReadOnly, Category = "游戏状态")
	float TotalPlayTime = 0.0f;

	// 当前游戏模式
	UPROPERTY(BlueprintReadOnly, Category = "游戏状态")
	ECQGamePlayMode CurrentPlayMode = ECQGamePlayMode::Exploration;

	// 击败晶兽总数
	UPROPERTY(BlueprintReadOnly, Category = "统计")
	int32 TotalBeastsDefeated = 0;

	// 捕获晶兽总数
	UPROPERTY(BlueprintReadOnly, Category = "统计")
	int32 TotalBeastsCaptured = 0;

	// 完成任务总数
	UPROPERTY(BlueprintReadOnly, Category = "统计")
	int32 TotalQuestsCompleted = 0;

	virtual void Tick(float DeltaSeconds) override;
};
