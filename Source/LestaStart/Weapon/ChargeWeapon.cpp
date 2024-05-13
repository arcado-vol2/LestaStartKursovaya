// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeWeapon.h"
#include "Net/UnrealNetwork.h"

AChargeWeapon::AChargeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponType = EWeaponType::Charge;
}

void AChargeWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentCharge = 0;
}

void AChargeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChargeWeapon, CurrentCharge);
	DOREPLIFETIME(AChargeWeapon, MaxCharge);
	DOREPLIFETIME(AChargeWeapon, ChargePerSecond);
}


void AChargeWeapon::Fire(const float DeltaTime)
{
	if (!HasAuthority() || CurrentClip == 0)
	{
		return;
	}
	CurrentCharge = FMath::Min(CurrentCharge + ChargePerSecond * DeltaTime, MaxCharge);
	if (CurrentCharge < MaxCharge)
	{
		Super::Fire(DeltaTime);
	}
}

void AChargeWeapon::Server_EndFire_Implementation()
{
	Super::Server_EndFire_Implementation();
	CurrentCharge = 0;
}

void AChargeWeapon::OnRep_CurrentCharge()
{
	OnChargeChangedDelegate.Broadcast(CurrentCharge, MaxCharge);
}
