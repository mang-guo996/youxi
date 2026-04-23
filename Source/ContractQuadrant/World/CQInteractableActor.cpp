// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "World/CQInteractableActor.h"
#include "Core/CQPlayerCharacter.h"
#include "Core/CQGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "ContractQuadrant.h"

ACQInteractableActor::ACQInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void ACQInteractableActor::Interact_Implementation(ACQPlayerCharacter* Player)
{
	if (bOneTimeUse && bHasBeenUsed)
	{
		return;
	}

	// 给予奖励
	UCQGameInstance* GI = Cast<UCQGameInstance>(GetGameInstance());
	if (GI)
	{
		for (const auto& Pair : RewardItems)
		{
			GI->AddItem(Pair.Key, Pair.Value);
			UE_LOG(LogContractQuadrant, Log, TEXT("获得物品: %s x%d"), *Pair.Key.ToString(), Pair.Value);
		}

		if (RewardGold > 0)
		{
			GI->AddGold(RewardGold);
			UE_LOG(LogContractQuadrant, Log, TEXT("获得金币: %d"), RewardGold);
		}
	}

	if (bOneTimeUse)
	{
		bHasBeenUsed = true;
	}
}

FText ACQInteractableActor::GetInteractText_Implementation() const
{
	if (bOneTimeUse && bHasBeenUsed)
	{
		return FText::FromString(TEXT("（已使用）"));
	}
	return InteractPromptText;
}

bool ACQInteractableActor::CanInteract_Implementation(ACQPlayerCharacter* Player) const
{
	return !(bOneTimeUse && bHasBeenUsed);
}
