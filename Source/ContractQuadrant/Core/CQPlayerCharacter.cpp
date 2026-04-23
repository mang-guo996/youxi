// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Core/CQPlayerCharacter.h"
#include "CrystalBeast/CQBeastManagerComponent.h"
#include "Interfaces/CQInteractable.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ContractQuadrant.h"

ACQPlayerCharacter::ACQPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 胶囊体设置
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// 不随控制器旋转（使用移动方向旋转）
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 弹簧臂组件
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = CameraArmLength;
	SpringArm->SocketOffset = CameraOffset;
	SpringArm->bUsePawnControlRotation = true;				// 跟随控制器旋转
	SpringArm->bEnableCameraLag = true;						// 开启摄像机延迟（更平滑）
	SpringArm->CameraLagSpeed = 10.0f;
	SpringArm->bDoCollisionTest = true;						// 防止摄像机穿墙

	// 摄像机组件
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 角色移动组件设置
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 角色朝移动方向旋转
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// 晶兽管理组件
	BeastManager = CreateDefaultSubobject<UCQBeastManagerComponent>(TEXT("BeastManager"));
}

void ACQPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 应用移动速度
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	UE_LOG(LogContractQuadrant, Log, TEXT("玩家角色初始化完成"));
}

void ACQPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AActor* ACQPlayerCharacter::GetNearestInteractable() const
{
	// 球形扫描检测附近可交互对象
	TArray<FHitResult> HitResults;
	FVector ScanCenter = GetActorLocation();

	FCollisionShape SphereShape;
	SphereShape.SetSphere(InteractionScanRadius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		ScanCenter,
		ScanCenter,
		FQuat::Identity,
		ECC_Visibility,
		SphereShape,
		QueryParams
	);

	float ClosestDistance = InteractionScanRadius;
	AActor* ClosestInteractable = nullptr;

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor->Implements<UCQInteractable>())
			{
				float Distance = FVector::Dist(ScanCenter, HitActor->GetActorLocation());
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestInteractable = HitActor;
				}
			}
		}
	}

	return ClosestInteractable;
}
