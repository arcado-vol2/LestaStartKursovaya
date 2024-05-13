// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UENUM()
enum EGameplayActorStatus : uint8
{
	Alive UMETA(DisplayName = "Alive"),
	Dead UMETA(DisplayName = "Dead"),
	Invincible UMETA(DisplayName = "Invincible")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthGone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangeDelegate, float, CurrentHealth, float, MaxHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthGone OnHealthGone;
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeDelegate OnHealthChangeDelegate;
	
	UFUNCTION()
	virtual void TakeDamage(AActor* DamageActor, float DamageAmount, AController* InstigatedBy);
	UFUNCTION()
	float GetCurrentHealth() const {return CurrentHealth;}
	UFUNCTION()
	float GetMaxHealth() const {return MaxHealth;}
	UFUNCTION()
	TEnumAsByte<EGameplayActorStatus> GetCurrentGameplayStatus() const {return CurrentGameplayStatus;}
	
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Health)
	float CurrentHealth;

	UFUNCTION()
	virtual void IsAliveCheck();
		
	UPROPERTY(VisibleInstanceOnly)
	TEnumAsByte<EGameplayActorStatus> CurrentGameplayStatus;
	
	UFUNCTION()
	virtual void OnRep_Health();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
