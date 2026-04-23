// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Core/CQPlayerController.h"
#include "Core/CQPlayerCharacter.h"
#include "Interfaces/CQInteractable.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ContractQuadrant.h"

ACQPlayerController::ACQPlayerController()
{
}

void ACQPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 添加默认输入映射上下文（探索模式）
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (ExplorationMappingContext)
		{
			Subsystem->AddMappingContext(ExplorationMappingContext, 0);
		}
	}
}

void ACQPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInput)
	{
		UE_LOG(LogContractQuadrant, Error, TEXT("无法获取EnhancedInputComponent"));
		return;
	}

	// 绑定移动
	if (MoveAction)
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACQPlayerController::HandleMove);
	}

	// 绑定视角
	if (LookAction)
	{
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACQPlayerController::HandleLook);
	}

	// 绑定跳跃
	if (JumpAction)
	{
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACQPlayerController::HandleJump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACQPlayerController::HandleStopJumping);
	}

	// 绑定交互
	if (InteractAction)
	{
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ACQPlayerController::HandleInteract);
	}

	// 绑定菜单
	if (OpenMenuAction)
	{
		EnhancedInput->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &ACQPlayerController::HandleOpenMenu);
	}

	// 绑定晶兽菜单
	if (OpenBeastMenuAction)
	{
		EnhancedInput->BindAction(OpenBeastMenuAction, ETriggerEvent::Started, this, &ACQPlayerController::HandleOpenBeastMenu);
	}

	// 绑定任务日志
	if (OpenQuestLogAction)
	{
		EnhancedInput->BindAction(OpenQuestLogAction, ETriggerEvent::Started, this, &ACQPlayerController::HandleOpenQuestLog);
	}
}

// ============================================================================
// 输入处理
// ============================================================================

void ACQPlayerController::HandleMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	ACQPlayerCharacter* PlayerChar = Cast<ACQPlayerCharacter>(GetPawn());
	if (PlayerChar)
	{
		// 获取控制器的前方和右方方向
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 添加移动输入
		PlayerChar->AddMovementInput(ForwardDirection, MovementVector.Y);
		PlayerChar->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACQPlayerController::HandleLook(const FInputActionValue& Value)
{
	FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (LookAxisValue != FVector2D::ZeroVector)
	{
		AddYawInput(LookAxisValue.X);
		AddPitchInput(LookAxisValue.Y);
	}
}

void ACQPlayerController::HandleJump()
{
	ACQPlayerCharacter* PlayerChar = Cast<ACQPlayerCharacter>(GetPawn());
	if (PlayerChar)
	{
		PlayerChar->Jump();
	}
}

void ACQPlayerController::HandleStopJumping()
{
	ACQPlayerCharacter* PlayerChar = Cast<ACQPlayerCharacter>(GetPawn());
	if (PlayerChar)
	{
		PlayerChar->StopJumping();
	}
}

void ACQPlayerController::HandleInteract()
{
	PerformInteraction();
}

void ACQPlayerController::HandleOpenMenu()
{
	// 由蓝图或UI系统处理菜单开关
	UE_LOG(LogContractQuadrant, Log, TEXT("打开主菜单"));
}

void ACQPlayerController::HandleOpenBeastMenu()
{
	UE_LOG(LogContractQuadrant, Log, TEXT("打开晶兽菜单"));
}

void ACQPlayerController::HandleOpenQuestLog()
{
	UE_LOG(LogContractQuadrant, Log, TEXT("打开任务日志"));
}

// ============================================================================
// 交互系统
// ============================================================================

void ACQPlayerController::PerformInteraction()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	// 射线检测
	FVector Start = ControlledPawn->GetActorLocation();
	FVector End = Start + ControlledPawn->GetActorForwardVector() * InteractionDistance;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ControlledPawn);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);

	if (bHit && HitResult.GetActor())
	{
		// 检查是否实现了可交互接口
		if (HitResult.GetActor()->Implements<UCQInteractable>())
		{
			ICQInteractable::Execute_Interact(
				HitResult.GetActor(),
				Cast<ACQPlayerCharacter>(ControlledPawn));

			UE_LOG(LogContractQuadrant, Log, TEXT("与 %s 交互"), *HitResult.GetActor()->GetName());
		}
	}
}

void ACQPlayerController::SwitchInputContext(ECQGamePlayMode NewMode)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!Subsystem)
	{
		return;
	}

	// 清除所有映射上下文
	Subsystem->ClearAllMappings();

	// 根据模式添加对应的输入上下文
	switch (NewMode)
	{
	case ECQGamePlayMode::Exploration:
		if (ExplorationMappingContext)
		{
			Subsystem->AddMappingContext(ExplorationMappingContext, 0);
		}
		break;

	case ECQGamePlayMode::Battle:
		if (BattleMappingContext)
		{
			Subsystem->AddMappingContext(BattleMappingContext, 0);
		}
		break;

	case ECQGamePlayMode::Menu:
	case ECQGamePlayMode::Dialogue:
		if (MenuMappingContext)
		{
			Subsystem->AddMappingContext(MenuMappingContext, 0);
		}
		break;
	}
}
