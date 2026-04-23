// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "World/CQTeleportPoint.h"
#include "Core/CQPlayerCharacter.h"
#include "Core/CQGameInstance.h"
#include "Core/CQGameMode.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ContractQuadrant.h"

ACQTeleportPoint::ACQTeleportPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = TriggerBox;
}

void ACQTeleportPoint::Interact_Implementation(ACQPlayerCharacter* Player)
{
	if (!Player) return;

	UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
	if (!GI) return;

	// 解锁传送点
	if (bRequiresUnlock && !GI->UnlockedTeleportPoints.Contains(TeleportPointID))
	{
		GI->UnlockedTeleportPoints.Add(TeleportPointID);
		UE_LOG(LogContractQuadrant, Log, TEXT("解锁传送点: %s"), *TeleportPointName.ToString());
	}

	// 传送到目标区域
	ACQGameMode* GameMode = Cast<ACQGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->TravelToArea(TargetArea);
	}
}

FText ACQTeleportPoint::GetInteractText_Implementation() const
{
	return FText::FromString(FString::Printf(TEXT("传送至 %s"), *TeleportPointName.ToString()));
}

bool ACQTeleportPoint::CanInteract_Implementation(ACQPlayerCharacter* Player) const
{
	if (!bRequiresUnlock) return true;

	UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
	if (!GI) return false;

	return GI->UnlockedTeleportPoints.Contains(TeleportPointID);
}

void ACQTeleportPoint::OnPlayerEnterTrigger(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ACQPlayerCharacter* Player = Cast<ACQPlayerCharacter>(OtherActor);
	if (Player)
	{
		// 自动解锁传送点
		UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
		if (GI && !GI->UnlockedTeleportPoints.Contains(TeleportPointID))
		{
			GI->UnlockedTeleportPoints.Add(TeleportPointID);
			UE_LOG(LogContractQuadrant, Log, TEXT("发现传送点: %s"), *TeleportPointName.ToString());
		}
	}
}
