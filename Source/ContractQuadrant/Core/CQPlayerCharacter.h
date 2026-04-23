// Copyright (C) 2025 契约象限. All Rights Reserved.
// 玩家角色 - 第三人称角色，带晶兽管理组件

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CQPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCQBeastManagerComponent;

UCLASS()
class CONTRACTQUADRANT_API ACQPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACQPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ========== 组件 ==========

	// 弹簧臂（第三人称摄像机挂载）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "摄像机")
	TObjectPtr<USpringArmComponent> SpringArm;

	// 第三人称摄像机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "摄像机")
	TObjectPtr<UCameraComponent> FollowCamera;

	// 晶兽管理组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "晶兽")
	TObjectPtr<UCQBeastManagerComponent> BeastManager;

	// ========== 摄像机设置 ==========

	// 弹簧臂目标长度
	UPROPERTY(EditDefaultsOnly, Category = "摄像机")
	float CameraArmLength = 400.0f;

	// 弹簧臂偏移
	UPROPERTY(EditDefaultsOnly, Category = "摄像机")
	FVector CameraOffset = FVector(0.0f, 0.0f, 80.0f);

	// ========== 移动参数 ==========

	// 行走速度
	UPROPERTY(EditDefaultsOnly, Category = "移动")
	float WalkSpeed = 400.0f;

	// 跑步速度
	UPROPERTY(EditDefaultsOnly, Category = "移动")
	float RunSpeed = 600.0f;

	// ========== 交互 ==========

	// 获取当前可交互对象
	UFUNCTION(BlueprintCallable, Category = "交互")
	AActor* GetNearestInteractable() const;

	// 交互检测范围
	UPROPERTY(EditDefaultsOnly, Category = "交互")
	float InteractionScanRadius = 200.0f;
};
