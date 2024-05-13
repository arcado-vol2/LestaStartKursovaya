// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "LaserWeapon.generated.h"

UCLASS()
class LESTASTART_API ALaserWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	ALaserWeapon();

protected:
	virtual void Fire(const float DeltaTime) override;
	virtual void FireEffect() override;
};
