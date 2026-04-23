// Copyright (C) 2025 契约象限. All Rights Reserved.
// 传送点

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CQInteractable.h"
#include "Core/CQTypes.h"
#include "CQTeleportPoint.generated.h"

class UBoxComponent;

UCLASS()
class CONTRACTQUADRANT_API ACQTeleportPoint : public AActor, public ICQInteractable
{
	GENERATED_BODY()

public:
	ACQTeleportPoint();

	// ICQInteractable 接口
	virtual void Interact_Implementation(ACQPlayerCharacter* Player) override;
	virtual FText GetInteractText_Implementation() const override;
	virtual bool CanInteract_Implementation(ACQPlayerCharacter* Player) const override;

	// 传送点ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "传送")
	FName TeleportPointID;

	// 传送点显示名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "传送")
	FText TeleportPointName;

	// 目标区域
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "传送")
	ECQWorldArea TargetArea = ECQWorldArea::CrystalTown;

	// 触发区域
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "传送")
	TObjectPtr<UBoxComponent> TriggerBox;

	// 是否需要解锁
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "传送")
	bool bRequiresUnlock = true;

protected:
	UFUNCTION()
	void OnPlayerEnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
