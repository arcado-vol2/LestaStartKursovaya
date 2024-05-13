// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Sentry.generated.h"

//These are the states of the sentry itself, not its state machine
UENUM()
enum ESentryStates : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Combat UMETA(DisplayName = "Combat"),
};

UCLASS()
class LESTASTART_API ASentry : public AEnemy
{
	GENERATED_BODY()

public:
	ASentry();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Head;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Muzzle;
	
	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* FaceDirection;
	
	UPROPERTY(Replicated)
	TEnumAsByte<ESentryStates> CurrentState;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	AActor* CurrentTarget;
	
public:

	UFUNCTION()
	virtual void Fire(float DeltaTime);
	UFUNCTION()
	virtual void FireEffect();
	
	UFUNCTION()
	virtual void IdleRotate(float DeltaTime);

	UFUNCTION()
	virtual bool RotateTo(float DeltaTime, const FVector Target);
	UFUNCTION()
	virtual void SetRotationAngle(float NewAngle);

	UFUNCTION()
	virtual void SetCurrentState(const ESentryStates NewState);

	UFUNCTION()
	virtual void SetCurrentTarget(AActor* NewTarget);
	UFUNCTION()
	virtual AActor* GetCurrentTarget();


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Damage = 5.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float FireRange = 700.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float BattleRotationSpeed = 5.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float IdleRotationSpeed = 12.f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float RotationTrashHold = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Animation" )
	float MaxIdleRotationAngle = 70;
	int8 CurrentRotationDirection = 1;
	
	UPROPERTY(Replicated)
	bool IsFiringVisual = false;

};
