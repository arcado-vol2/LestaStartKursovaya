// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/BoxComponent.h"
#include "LestaStart/Core/HealthComponent.h"
#include "LestaStart/UI/FloatingUIComponent.h"
#include "LestaStart/Core/TeamComponent.h"

AEnemy::AEnemy()
{
	bReplicates = true;
	
	CubeCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Sphere"));
	SetRootComponent(CubeCollider);
	CubeCollider->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

	//For a real case, you will need to use skeletal mesh
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(CubeCollider);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("Team"));
	
	HealthWidgetComponent = CreateDefaultSubobject<UFloatingUIComponent>(TEXT("Health Widget"));
	HealthWidgetComponent->SetupAttachment(CubeCollider);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (TeamComponent && HealthComponent)
	{
		TeamComponent->SetCurrentTeam(FTeam(ETeams::Bot, {}, {ETeams::Blue, ETeams::Red}, {ETeams::Neutral}, FLinearColor(0.8,0,1)));
		HealthComponent->OnHealthChangeDelegate.AddDynamic(HealthWidgetComponent, &UFloatingUIComponent::SetHealth);
		HealthComponent->OnHealthGone.AddDynamic(this, &AEnemy::AEnemy::OnDeath);
		HealthWidgetComponent->SetStartValues(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth(), TeamComponent->GetTeamColor());
	}

}

void AEnemy::OnDeath()
{
	Server_Die();
}

void AEnemy::Server_Die_Implementation()
{
	Destroy();
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->RemoveFromRoot();
		HealthWidgetComponent = nullptr;
	}
	
}
