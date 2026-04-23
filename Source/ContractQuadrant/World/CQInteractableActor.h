// Copyright (C) 2025 契约象限. All Rights Reserved.
// 可交互物体基类（宝箱、告示牌等）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CQInteractable.h"
#include "CQInteractableActor.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;

UCLASS()
class CONTRACTQUADRANT_API ACQInteractableActor : public AActor, public ICQInteractable
{
	GENERATED_BODY()

public:
	ACQInteractableActor();

	// ICQInteractable 接口
	virtual void Interact_Implementation(ACQPlayerCharacter* Player) override;
	virtual FText GetInteractText_Implementation() const override;
	virtual bool CanInteract_Implementation(ACQPlayerCharacter* Player) const override;

	// 外观Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "外观")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	// 交互提示文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "交互")
	FText InteractPromptText = FText::FromString(TEXT("按E交互"));

	// 是否已被使用（一次性交互物）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "交互")
	bool bHasBeenUsed = false;

	// 是否为一次性交互
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "交互")
	bool bOneTimeUse = true;

	// 交互时给予的物品
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "奖励")
	TMap<FName, int32> RewardItems;

	// 交互时给予的金币
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "奖励")
	int32 RewardGold = 0;
};
