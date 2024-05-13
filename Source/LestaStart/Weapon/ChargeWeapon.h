// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ChargeWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChargeChangedDelegate, float, NewCharge, float, MaxCharge);

UCLASS()
class LESTASTART_API AChargeWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AChargeWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stats")
	float MaxCharge = 50;
	UPROPERTY(ReplicatedUsing=OnRep_CurrentCharge, VisibleInstanceOnly, Category = "Stats")
	float CurrentCharge = 0;
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stats")
	float ChargePerSecond = 5;
	UFUNCTION()
	virtual void OnRep_CurrentCharge();
	
	virtual void Fire(const float DeltaTime) override;

	virtual void Server_EndFire_Implementation() override;
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnChargeChangedDelegate OnChargeChangedDelegate;
};
