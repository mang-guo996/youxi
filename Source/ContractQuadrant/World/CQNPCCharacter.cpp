// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "World/CQNPCCharacter.h"
#include "Quest/CQDialogueComponent.h"
#include "Core/CQPlayerCharacter.h"
#include "AI/CQAIController.h"
#include "ContractQuadrant.h"

ACQNPCCharacter::ACQNPCCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// 对话组件
	DialogueComponent = CreateDefaultSubobject<UCQDialogueComponent>(TEXT("DialogueComponent"));

	// 任务标记根节点
	QuestMarkerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("QuestMarkerRoot"));
	QuestMarkerRoot->SetupAttachment(RootComponent);
	QuestMarkerRoot->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));

	// 设置AI控制器类
	AIControllerClass = ACQAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACQNPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 设置对话组件的NPC名称
	if (DialogueComponent)
	{
		DialogueComponent->NPCName = NPCDisplayName;
	}

	UpdateQuestMarker();
}

// ============================================================================
// 可交互接口实现
// ============================================================================

void ACQNPCCharacter::Interact_Implementation(ACQPlayerCharacter* Player)
{
	if (!Player || !DialogueComponent)
	{
		return;
	}

	// 通知AI控制器进入对话状态
	ACQAIController* NPCAI = Cast<ACQAIController>(GetController());
	if (NPCAI)
	{
		NPCAI->SetIsTalking(true);
	}

	// 面向玩家
	FVector Direction = Player->GetActorLocation() - GetActorLocation();
	Direction.Z = 0;
	if (!Direction.IsNearlyZero())
	{
		SetActorRotation(Direction.Rotation());
	}

	// 开始默认对话
	DialogueComponent->StartDialogue(DialogueComponent->DefaultDialogueGroupID);

	UE_LOG(LogContractQuadrant, Log, TEXT("与NPC交互: %s (%s)"),
		*NPCDisplayName.ToString(), *NPCID.ToString());
}

FText ACQNPCCharacter::GetInteractText_Implementation() const
{
	return InteractionPrompt;
}

bool ACQNPCCharacter::CanInteract_Implementation(ACQPlayerCharacter* Player) const
{
	// 正在对话中不可再次交互
	return DialogueComponent && !DialogueComponent->IsInDialogue();
}

void ACQNPCCharacter::UpdateQuestMarker()
{
	if (!bIsQuestNPC || !QuestMarkerRoot)
	{
		return;
	}

	// 任务标记的具体显示由蓝图处理（材质、动画等）
	// 此处仅控制可见性
	QuestMarkerRoot->SetVisibility(bIsQuestNPC, true);
}
