// Copyright (C) 2025 契约象限. All Rights Reserved.
// 野生晶兽生成器

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrystalBeast/CQBeastDataTypes.h"
#include "CQWildBeastSpawner.generated.h"

class UBoxComponent;
class UDataTable;

UCLASS()
class CONTRACTQUADRANT_API ACQWildBeastSpawner : public AActor
{
	GENERATED_BODY()

public:
	ACQWildBeastSpawner();

protected:
	virtual void BeginPlay() override;

public:
	// 触发区域
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "生成器")
	TObjectPtr<UBoxComponent> TriggerVolume;

	// 此区域的晶兽生成表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "生成器")
	TObjectPtr<UDataTable> SpawnTable;

	// 所属区域
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "生成器")
	ECQWorldArea Area = ECQWorldArea::CrystalTown;

	// 遭遇概率（每次进入触发区域时的遇敌概率，0-1）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "生成器", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float EncounterRate = 0.15f;

	// 最短遭遇间隔（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "生成器")
	float MinEncounterInterval = 5.0f;

protected:
	// 重叠事件
	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 随机选择一只野生晶兽
	bool SelectRandomBeast(FName& OutBeastID, int32& OutLevel) const;

	// 上次遭遇时间
	float LastEncounterTime = 0.0f;
};
