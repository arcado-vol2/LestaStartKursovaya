// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownWeapon.h"


ACooldownWeapon::ACooldownWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACooldownWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void ACooldownWeapon::Server_StartFire_Implementation()
{
	WeaponState = EWeaponState::Cooldown;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		WeaponState = EWeaponState::Idle;
	}, CooldownTime, false);
}

