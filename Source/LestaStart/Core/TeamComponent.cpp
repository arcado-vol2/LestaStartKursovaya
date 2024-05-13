// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamComponent.h"
#include "Net/UnrealNetwork.h"

UTeamComponent::UTeamComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentTeam = FTeam({ETeams::Neutral, {},{},{}, FLinearColor(1,1,1)});
}

void UTeamComponent::BeginPlay()
{
	SetIsReplicated(true);
	Super::BeginPlay();
}


bool UTeamComponent::CanDealDamageTo(AActor* Target)
{
	if (UTeamComponent* TargetTeamComponent = Target->FindComponentByClass<UTeamComponent>())
	{
		if (CurrentTeam.Enemies.Contains(TargetTeamComponent->GetCurrentTeam()))
		{
			return true;
		}
	}
	return false;
}

void UTeamComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTeamComponent, CurrentTeam);
}

