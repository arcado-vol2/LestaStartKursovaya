// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereWeapon.h"
#include "LestaStart/Core/HealthComponent.h"
#include "LestaStart/Core/LestaCharacter.h"

ASphereWeapon::ASphereWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ASphereWeapon::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere = FCollisionShape::MakeSphere(Range);
}


void ASphereWeapon::FireEffect()
{
	if (!CurrentOwner || CurrentClip == 0 || (HasAuthority() && GetNetMode() == NM_DedicatedServer ))
	{
		return;
	}
	const FVector Center = CurrentOwner->GetActorLocation();
	DrawDebugSphere(GetWorld(), Center, Range, 128, FColor::Red, false, 0.0f);
}

void ASphereWeapon::Server_EndFire_Implementation()
{
	TArray<FHitResult> OutHits;
	const FCollisionQueryParams QueryParams(TEXT("WeaponFire"), false, GetOwner());
	const FName ProfileName = UCollisionProfile::BlockAllDynamic_ProfileName;
	FVector StartLocation = CurrentOwner->GetActorLocation();
	if (GetWorld()->SweepMultiByProfile(OutHits, StartLocation, StartLocation, FQuat::Identity, ProfileName, CollisionSphere, QueryParams))
	{
		std::unordered_set<AActor*> DamagedActors;
		for (const FHitResult& Hit : OutHits)
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (!DamagedActors.count(HitActor) && HitActor->GetDistanceTo(CurrentOwner) <= Range )
				{
					if (UHealthComponent* HealthComponent = Hit.GetActor()->FindComponentByClass<UHealthComponent>())
					{
						HealthComponent->TakeDamage(this, Damage * CurrentCharge, CurrentOwner->GetController());
						DamagedActors.insert(HitActor);
					}
				}
			}
			
		}
	}
	Super::Server_EndFire_Implementation();
}
