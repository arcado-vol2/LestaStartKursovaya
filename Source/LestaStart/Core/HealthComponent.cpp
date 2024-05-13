// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "TeamComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxHealth = 100;
	CurrentHealth = 0;
	CurrentGameplayStatus = EGameplayActorStatus::Alive;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	if (GetOwner()->HasAuthority())
	{
		CurrentHealth = MaxHealth;
	}
	
}

void UHealthComponent::TakeDamage(AActor* DamageActor, float DamageAmount, AController* InstigatedBy)
{
	if (DamageAmount <= 0 || !GetOwner()->HasAuthority())
	{
		return;
	}
	if(UTeamComponent* DamageActorTeamComponent = InstigatedBy->GetPawn()->FindComponentByClass<UTeamComponent>())
	{
		if (DamageActorTeamComponent->CanDealDamageTo(GetOwner())) // If our owner among damage dealer enemies
		{
			CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
			OnHealthChangeDelegate.Broadcast(CurrentHealth, MaxHealth);
			IsAliveCheck();
		}
	}

}

void UHealthComponent::SetHealth(float amount)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	CurrentHealth = FMath::Clamp(amount, 0.0f, MaxHealth);
	OnHealthChangeDelegate.Broadcast(CurrentHealth, MaxHealth);
	IsAliveCheck();
}


void UHealthComponent::IsAliveCheck()
{
	if (CurrentHealth == 0)
	{
		OnHealthGone.Broadcast();
		CurrentGameplayStatus = EGameplayActorStatus::Dead;
	}
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
}

void UHealthComponent::OnRep_Health()
{
	OnHealthChangeDelegate.Broadcast(CurrentHealth, MaxHealth);
	IsAliveCheck();
}

