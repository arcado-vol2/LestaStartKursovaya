// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeapon::AWeapon()
{
	NetUpdateFrequency = 120.f;
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	CurrentOwner = nullptr;
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if(!CurrentOwner)
		Mesh->SetVisibility(false);
	if(HasAuthority())
	{
		CurrentClip = ClipSize;
		CurrentBelt = BeltSize;
	}
}


void AWeapon::Tick(float DeltaSeconds)
{
	if (WeaponState == EWeaponState::Firing)
	{
		Fire(DeltaSeconds);
	}
	if (IsFiringVisual)
	{
		FireEffect();
	}
		
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, WeaponState);
	DOREPLIFETIME(AWeapon, IsFiringVisual);
	DOREPLIFETIME(AWeapon, BeltSize);
	DOREPLIFETIME(AWeapon, ClipSize);
	DOREPLIFETIME(AWeapon, CurrentClip);
	DOREPLIFETIME(AWeapon, CurrentBelt);
	DOREPLIFETIME(AWeapon, ReloadTime);
	DOREPLIFETIME(AWeapon, Damage);
	DOREPLIFETIME(AWeapon, Range);
	DOREPLIFETIME(AWeapon, AmmoPerSecond);
}


void AWeapon::StartFire()
{
	Server_StartFire();
	if (WeaponState == EWeaponState::Idle)
	{
		IsFiringVisual = true;
	}
	
}

void AWeapon::Server_StartFire_Implementation()
{
	if (WeaponState == EWeaponState::Idle)
	{
		WeaponState = EWeaponState::Firing;
		IsFiringVisual = true;
	}

}

void AWeapon::EndFire()
{
	Server_EndFire();
	
}


void AWeapon::Server_EndFire_Implementation()
{
	IsFiringVisual = false;
	WeaponState = EWeaponState::Idle;
	OnEndFire();
	
}

void AWeapon::Reload()
{
	Server_Reload();
}

void AWeapon::Server_Reload_Implementation()
{
	WeaponState = EWeaponState::Reloading;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		const float TakenAmmo = ClipSize - CurrentClip;
		if (TakenAmmo > CurrentBelt)
		{
			CurrentClip += CurrentBelt;
			CurrentBelt = 0;
		}
		else
		{
			CurrentBelt -= TakenAmmo;
			CurrentClip += TakenAmmo;
		}
		WeaponState = EWeaponState::Idle;
	}, ReloadTime, false);
	OnAmmoChangeDelegate.Broadcast(CurrentClip, CurrentBelt);
}


void AWeapon::Fire(const float DeltaTime)
{
	CurrentClip = FMath::Max(CurrentClip - AmmoPerSecond * DeltaTime, 0);
	OnAmmoChangeDelegate.Broadcast(CurrentClip, CurrentBelt);
}


void AWeapon::FireEffect()
{
	
}

FIntPoint AWeapon::GetCurrentAmmoInfo()
{
	return FIntPoint(FMath::TruncToInt(CurrentClip), FMath::TruncToInt(CurrentBelt));
}

EWeaponState AWeapon::GetWeaponState()
{
	return WeaponState;
}

void AWeapon::OnRep_CurrentBelt()
{
	OnAmmoChangeDelegate.Broadcast(CurrentClip, CurrentBelt);
}

void AWeapon::OnRep_CurrentClip()
{
	OnAmmoChangeDelegate.Broadcast(CurrentClip, CurrentBelt);
}

EWeaponType AWeapon::GetWeaponType()
{
	return WeaponType;
}

void AWeapon::OnEndFire()
{
	
}

