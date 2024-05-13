// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickable.generated.h"

UCLASS()
class LESTASTART_API APickable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickable();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* TriggerVolume;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* VisualMesh;

	UFUNCTION()
	virtual void OnPeakUp(UPrimitiveComponent* OverlappedComp, AActor* PickerActor, UPrimitiveComponent* PickerComp, int32 PickerBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(Server, Reliable)
	virtual void Server_OnPeakUp(class ALestaCharacter* PickerPlayer);
};
