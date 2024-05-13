// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentry.h"

#include "SentryController.h"
#include "Components/ArrowComponent.h"
#include "LestaStart/Core/HealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASentry::ASentry()
{
	NetUpdateFrequency = 60.f;
	
	bReplicates = true;
	
	PrimaryActorTick.bCanEverTick = true;

	//Ideally, here you need to work with a skeleton mesh and an animator
	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Body);
	Head->SetIsReplicated(true);
	
	Muzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzle"));
	Muzzle ->SetupAttachment(Head);
	Muzzle->SetIsReplicated(true);
	
	FaceDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Face direction"));
	FaceDirection->SetupAttachment(Muzzle);
	FaceDirection->SetIsReplicated(true);
	
	CurrentState = ESentryStates::Idle;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
}

void ASentry::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	Head->SetIsReplicated(true);
	Muzzle->SetIsReplicated(true);
	FaceDirection->SetIsReplicated(true);
}


void ASentry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (CurrentState)
	{
	case ESentryStates::Combat:
		if (CurrentTarget)
		{
			if (HasAuthority())
			{
				if (RotateTo(DeltaTime, CurrentTarget->GetActorLocation()))
				{
					Fire(DeltaTime);
					IsFiringVisual = true;
				}
				else
				{
					IsFiringVisual = false;
				}
			}
			//Since it is replicated
			if (IsFiringVisual)
			{
				FireEffect();
			}
		}
		break;

	case ESentryStates::Idle:
		IdleRotate(DeltaTime);
		break;

	default:
		break;
	}

}

void ASentry::Fire(float DeltaTime)
{
	if (!HasAuthority())
	{
		return;
	}
	FHitResult OutHit;
	const FVector& Start = FaceDirection->GetComponentLocation();
	const FVector End = Start + FaceDirection->GetComponentRotation().Vector() * FireRange;
	const FName ProfileName = UCollisionProfile::BlockAllDynamic_ProfileName;
	const FCollisionQueryParams QueryParams(TEXT("SentryFire"), false, this);
	GetWorld()->LineTraceSingleByProfile(OutHit, Start, End, ProfileName, QueryParams);
	if (OutHit.IsValidBlockingHit())
	{
		if (UHealthComponent* TargetHealthComponent = OutHit.GetActor()->FindComponentByClass<UHealthComponent>())
		{
			TargetHealthComponent->TakeDamage(this, Damage * DeltaTime, this->GetController());
		}
		
	}
	//Remove the current target from available ones if it is dead or invulnerable
	if (CurrentTarget->FindComponentByClass<UHealthComponent>()->GetCurrentGameplayStatus() != EGameplayActorStatus::Alive)
	{
		if (ASentryController* SentryController = Cast<ASentryController>(GetController()))
		{
			SentryController->RemoveTarget(CurrentTarget);
		}
	}
}

void ASentry::IdleRotate(float DeltaTime)
{
	if (!HasAuthority())
	{
		return;
	}
	Muzzle->SetRelativeRotation(FMath::RInterpTo(Muzzle->GetRelativeRotation(), FRotator::ZeroRotator, DeltaTime, BattleRotationSpeed));
	FRotator HeadRotation = Head->GetRelativeRotation();
	float DeltaRotation = IdleRotationSpeed * DeltaTime * CurrentRotationDirection;
	float NewYaw = FMath::ClampAngle(HeadRotation.Yaw + DeltaRotation, -MaxIdleRotationAngle, MaxIdleRotationAngle);
	Head->SetRelativeRotation(FRotator(0.0f, NewYaw, 0.0f));
	if (FMath::IsNearlyEqual(NewYaw, MaxIdleRotationAngle, RotationTrashHold) || FMath::IsNearlyEqual(NewYaw, -MaxIdleRotationAngle, RotationTrashHold))
	{
		CurrentRotationDirection *= -1.0f;
	}
}


bool ASentry::RotateTo(float DeltaTime, const FVector Target)
{
	if (!HasAuthority())
	{
		return false;
	}
	FVector Direction = Target - GetActorLocation();
	if (!Direction.IsNearlyZero())
	{
		FRotator NewRotation = Direction.Rotation();
		FRotator StartHeadRotation = Head->GetComponentRotation();
		FRotator InterpolatedHeadRotation = FMath::RInterpTo(StartHeadRotation, FRotator(0, NewRotation.Yaw, 0), DeltaTime, BattleRotationSpeed);
		Head->SetWorldRotation(InterpolatedHeadRotation);
		
		FRotator StartMuzzleRotation = Muzzle->GetRelativeRotation();
		FRotator InterpolatedMuzzleRotation = FMath::RInterpTo(StartMuzzleRotation, FRotator(NewRotation.Pitch, 0, 0), DeltaTime, BattleRotationSpeed);
		Muzzle->SetRelativeRotation(InterpolatedMuzzleRotation);
		
		if ((FRotator(InterpolatedMuzzleRotation.Pitch,InterpolatedHeadRotation.Yaw,NewRotation.Roll) - NewRotation).IsNearlyZero(RotationTrashHold))
		{
			return true;
		}
	}
	return false;
	
}

void ASentry::FireEffect()
{
	if ((HasAuthority() && GetNetMode() == NM_DedicatedServer ))
	{
		return;
	}
	FHitResult OutHit;
	const FVector& Start = FaceDirection->GetComponentLocation();
	FVector End = Start + FaceDirection->GetComponentRotation().Vector() * FireRange;
	const FName ProfileName = UCollisionProfile::Pawn_ProfileName;
	const FCollisionQueryParams QueryParams(TEXT("SentryFireVisual"), false, this);
	GetWorld()->LineTraceSingleByProfile(OutHit, Start, End, ProfileName, QueryParams);
	if (OutHit.IsValidBlockingHit())
	{
		End = OutHit.ImpactPoint;
	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.0f);
}


void ASentry::SetCurrentState(const ESentryStates NewState)
{
	CurrentState = NewState;
}

void ASentry::SetCurrentTarget(AActor* NewTarget)
{
	CurrentTarget = NewTarget;
}

AActor* ASentry::GetCurrentTarget()
{
	return  CurrentTarget;
}


void ASentry::SetRotationAngle(float NewAngle)
{
	MaxIdleRotationAngle = NewAngle;
}

void ASentry::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASentry, CurrentState);
	DOREPLIFETIME(ASentry, CurrentTarget);
	DOREPLIFETIME(ASentry, IsFiringVisual);
}