// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "World/CQWildBeastSpawner.h"
#include "Core/CQPlayerCharacter.h"
#include "Core/CQGameMode.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "ContractQuadrant.h"

ACQWildBeastSpawner::ACQWildBeastSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
	TriggerVolume->SetGenerateOverlapEvents(true);
	RootComponent = TriggerVolume;
}

void ACQWildBeastSpawner::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ACQWildBeastSpawner::OnTriggerOverlap);
}

void ACQWildBeastSpawner::OnTriggerOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// 只响应玩家角色
	ACQPlayerCharacter* Player = Cast<ACQPlayerCharacter>(OtherActor);
	if (!Player)
	{
		return;
	}

	// 检查遭遇间隔
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastEncounterTime < MinEncounterInterval)
	{
		return;
	}

	// 检查是否已在战斗中
	ACQGameMode* GameMode = Cast<ACQGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GameMode || GameMode->GetCurrentGamePlayMode() != ECQGamePlayMode::Exploration)
	{
		return;
	}

	// 随机判定遭遇
	float Roll = FMath::FRandRange(0.0f, 1.0f);
	if (Roll > EncounterRate)
	{
		return;
	}

	// 选择野生晶兽
	FName BeastID;
	int32 BeastLevel;
	if (SelectRandomBeast(BeastID, BeastLevel))
	{
		LastEncounterTime = CurrentTime;
		GameMode->StartWildBattle(BeastID, BeastLevel);

		UE_LOG(LogContractQuadrant, Log, TEXT("遭遇野生晶兽: %s Lv.%d"), *BeastID.ToString(), BeastLevel);
	}
}

bool ACQWildBeastSpawner::SelectRandomBeast(FName& OutBeastID, int32& OutLevel) const
{
	if (!SpawnTable)
	{
		return false;
	}

	TArray<FCQWildBeastEntry*> AllEntries;
	SpawnTable->GetAllRows<FCQWildBeastEntry>(TEXT("WildSpawn"), AllEntries);

	if (AllEntries.Num() == 0)
	{
		return false;
	}

	// 按权重随机选择
	float TotalWeight = 0.0f;
	for (const FCQWildBeastEntry* Entry : AllEntries)
	{
		if (Entry->Area == Area)
		{
			TotalWeight += Entry->SpawnWeight;
		}
	}

	if (TotalWeight <= 0.0f)
	{
		return false;
	}

	float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);
	float Accumulated = 0.0f;

	for (const FCQWildBeastEntry* Entry : AllEntries)
	{
		if (Entry->Area != Area) continue;

		Accumulated += Entry->SpawnWeight;
		if (RandomWeight <= Accumulated)
		{
			OutBeastID = Entry->BeastID;
			OutLevel = FMath::RandRange(Entry->MinLevel, Entry->MaxLevel);
			return true;
		}
	}

	return false;
}
