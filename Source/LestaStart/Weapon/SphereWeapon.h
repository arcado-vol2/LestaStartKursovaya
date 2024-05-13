// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChargeWeapon.h"
#include "Weapon.h"
#include "SphereWeapon.generated.h"

UCLASS()
class LESTASTART_API ASphereWeapon : public AChargeWeapon
{
	GENERATED_BODY()

public:
	ASphereWeapon();
protected:
	virtual void BeginPlay() override;
	virtual void FireEffect() override;
	virtual void Server_EndFire_Implementation() override;
	FCollisionShape CollisionSphere;
};
