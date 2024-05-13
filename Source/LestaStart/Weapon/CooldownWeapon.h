// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "CooldownWeapon.generated.h"

UCLASS()
//not used now, but can in future
class LESTASTART_API ACooldownWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	ACooldownWeapon();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Server_StartFire_Implementation() override;
	UPROPERTY(Replicated, EditDefaultsOnly)
	float CooldownTime = 1.3;
};
