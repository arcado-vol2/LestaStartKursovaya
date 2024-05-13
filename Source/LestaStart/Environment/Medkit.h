// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickable.h"
#include "Medkit.generated.h"

UCLASS()
class LESTASTART_API AMedkit : public APickable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMedkit();

protected:
	UPROPERTY(EditDefaultsOnly)
	float HealAmount = 40;
	virtual void Server_OnPeakUp_Implementation(ALestaCharacter* PickerPlayer) override;
	
};
