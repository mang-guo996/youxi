// Copyright (C) 2025 契约象限. All Rights Reserved.
// NPC角色基类

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CQInteractable.h"
#include "CQNPCCharacter.generated.h"

class UCQDialogueComponent;

UCLASS()
class CONTRACTQUADRANT_API ACQNPCCharacter : public ACharacter, public ICQInteractable
{
	GENERATED_BODY()

public:
	ACQNPCCharacter();

protected:
	virtual void BeginPlay() override;

public:
	// ========== ICQInteractable 接口 ==========
	virtual void Interact_Implementation(ACQPlayerCharacter* Player) override;
	virtual FText GetInteractText_Implementation() const override;
	virtual bool CanInteract_Implementation(ACQPlayerCharacter* Player) const override;

	// ========== 组件 ==========

	// 对话组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<UCQDialogueComponent> DialogueComponent;

	// ========== NPC配置 ==========

	// NPC唯一ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FName NPCID;

	// NPC显示名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FText NPCDisplayName;

	// 交互提示文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FText InteractionPrompt = FText::FromString(TEXT("按E对话"));

	// 是否为任务NPC（显示头顶标记）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	bool bIsQuestNPC = false;

	// 头顶任务标记组件（蓝图中设置Mesh/Widget）
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NPC")
	TObjectPtr<USceneComponent> QuestMarkerRoot;

	// 更新任务标记显示
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void UpdateQuestMarker();
};
