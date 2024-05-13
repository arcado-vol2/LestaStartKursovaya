// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

UCLASS()
class LESTASTART_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnDeath();

	UFUNCTION(Server, Reliable)
	virtual void Server_Die();

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* CubeCollider;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere)
	class UFloatingUIComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly)
	class UHealthComponent* HealthComponent;

	UPROPERTY( EditDefaultsOnly, Category = "Stats")
	class UTeamComponent* TeamComponent;
};
