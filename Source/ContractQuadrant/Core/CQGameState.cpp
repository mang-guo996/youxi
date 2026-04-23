// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Core/CQGameState.h"

ACQGameState::ACQGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACQGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TotalPlayTime += DeltaSeconds;
}
