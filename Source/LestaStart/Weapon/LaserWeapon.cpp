#include "LaserWeapon.h"

#include "LestaStart/Core/HealthComponent.h"
#include "LestaStart/Core/LestaCharacter.h"
#include "Camera/CameraComponent.h"

ALaserWeapon::ALaserWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponType = EWeaponType::Laser;
}


void ALaserWeapon::Fire(const float DeltaTime)
{
	if (!HasAuthority() || CurrentClip == 0)
	{
		return;
	}
	Super::Fire(DeltaTime);
	FHitResult OutHit;
	//Firing from camera
	const FVector& Start = CurrentOwner->GetCamera()->GetComponentLocation();
	const FVector End = Start + CurrentOwner->GetControlRotation().Vector() * Range;
	const FName ProfileName = UCollisionProfile::BlockAllDynamic_ProfileName;
	const FCollisionQueryParams QueryParams(TEXT("WeaponFire"), false, GetOwner());
	GetWorld()->LineTraceSingleByProfile(OutHit, Start, End, ProfileName, QueryParams);
	if (OutHit.IsValidBlockingHit())
	{
		if (AActor* HitTarget = OutHit.GetActor())
		{
			if (UHealthComponent* HealthComponent = HitTarget->FindComponentByClass<UHealthComponent>())
			{
				HealthComponent->TakeDamage(this, Damage * DeltaTime, CurrentOwner->GetController());
			}
		}
	}
}

void ALaserWeapon::FireEffect()
{
	if (!CurrentOwner || CurrentClip == 0 || (HasAuthority() && GetNetMode() == NM_DedicatedServer ))
	{
		return;
	}
	//Firing from muzzle
	const FVector& Start = Mesh->GetSocketLocation(FName("Muzzle"));
	FVector End = Start + CurrentOwner->GetBaseAimRotation().Vector() * Range;
	FHitResult OutHit;
	const FName ProfileName = UCollisionProfile::BlockAllDynamic_ProfileName;
	const FCollisionQueryParams QueryParams(TEXT("WeaponFire"), false, GetOwner());
	GetWorld()->LineTraceSingleByProfile(OutHit, Start, End, ProfileName, QueryParams);
	if (OutHit.IsValidBlockingHit())
	{
		End = OutHit.ImpactPoint;
	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.0f);
}

