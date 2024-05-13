// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SentryController.generated.h"


UCLASS()
class LESTASTART_API ASentryController : public AAIController
{
	GENERATED_BODY()

public:
	ASentryController();

protected:
	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* AIPerceptionComponent;

	void OnPossess(APawn* InPawn) override;
	
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> WeakTargets;
	
	UFUNCTION()
	virtual void OnPerceptionTargetUpdate(AActor* Actor, struct FAIStimulus Stimulus);
	
	UPROPERTY(VisibleAnywhere)
	class ASentry* CurrentPawn;

	UPROPERTY(EditDefaultsOnly)
	float MaxSightAngle = 90;

public:
	UFUNCTION()
	virtual  AActor* GetClosestTarget();
	UFUNCTION()
	virtual void RemoveTarget(AActor* Target);
};
