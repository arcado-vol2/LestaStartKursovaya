// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeamComponent.generated.h"

UENUM()
enum ETeams : int8
{
	Red UMETA(DisplayName = "Red"), 
	Blue UMETA(DisplayName = "Blue"),
	
	Neutral UMETA(DisplayName = "Neutral"),
	Bot UMETA(DisplayName = "Bot")
};

USTRUCT()
struct FTeam
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETeams> ThisTeam = ETeams::Neutral;
	UPROPERTY(EditDefaultsOnly)
 	TArray<TEnumAsByte<ETeams>> Allies;
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<ETeams>> Enemies; 
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<ETeams>> Neutrals;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor TeamColor = FLinearColor(1, 1, 1);
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTeamComponent();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	UPROPERTY(Replicated, VisibleAnywhere)
	FTeam CurrentTeam;
	
public:
	UFUNCTION()
	virtual bool CanDealDamageTo(AActor* Target);
	virtual TEnumAsByte<ETeams> GetCurrentTeam(){return CurrentTeam.ThisTeam;}
	virtual void SetCurrentTeam(FTeam NewTeam) {CurrentTeam = NewTeam;}
	virtual FLinearColor GetTeamColor(){return  CurrentTeam.TeamColor;}
};
