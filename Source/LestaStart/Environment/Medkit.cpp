// Fill out your copyright notice in the Description page of Project Settings.


#include "Medkit.h"

#include "LestaStart/Core/HealthComponent.h"
#include "LestaStart/Core/LestaCharacter.h"


// Sets default values
AMedkit::AMedkit()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMedkit::Server_OnPeakUp_Implementation(ALestaCharacter* PickerPlayer)
{
	if (UHealthComponent* HealthComponent = PickerPlayer->FindComponentByClass<UHealthComponent>())
	{
		if (HealthComponent->GetCurrentHealth() == HealthComponent->GetMaxHealth())
		{
			return;
		}
		HealthComponent->SetHealth(HealthComponent->GetCurrentHealth() + HealAmount);
		Super::Server_OnPeakUp_Implementation(PickerPlayer);
	}
	
}


