// Copyright (C) 2025 契约象限. All Rights Reserved.
// NPC AI控制器

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CQAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBehaviorTree;

UCLASS()
class CONTRACTQUADRANT_API ACQAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACQAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	// 行为树组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;

	// 黑板组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	// 使用的行为树资源
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	// ========== 黑板键名 ==========
	static const FName BB_PlayerActor;       // 玩家Actor引用
	static const FName BB_PlayerDistance;     // 玩家距离
	static const FName BB_IsPlayerNearby;    // 玩家是否在附近
	static const FName BB_IsTalking;         // 是否在对话
	static const FName BB_PatrolTarget;      // 巡逻目标点
	static const FName BB_HasQuestForPlayer; // 是否有可给玩家的任务

	// 设置黑板值的便捷方法
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetIsTalking(bool bTalking);
};
