// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Core/CQSaveGame.h"

UCQSaveGame::UCQSaveGame()
{
	PlayerName = TEXT("训练师");
	PlayerGold = 500;
	CurrentArea = ECQWorldArea::CrystalTown;
	PlayerLocation = FVector::ZeroVector;
	PlayerRotation = FRotator::ZeroRotator;
	SaveDateTime = FDateTime::Now();
	PlayTimeSeconds = 0.0f;
	SaveVersion = TEXT("1.0.0");
}
