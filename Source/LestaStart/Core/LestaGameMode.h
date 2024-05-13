// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LestaGameMode.generated.h"

/** Base Game Mode class for the Lesta Start project. */
class ALestaCharacter;
UCLASS()
class LESTASTART_API ALestaGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ALestaGameMode();
protected:
	UPROPERTY()
	//Alive players array
	TArray<ALestaCharacter*> PlayersCharactersList;

public:
	UFUNCTION()
	virtual FTeam AddPlayer(ALestaCharacter* NewPlayer);
	UFUNCTION()
	virtual void RemovePlayer(ALestaCharacter* RemovedPlayer);
	UFUNCTION()
	virtual ALestaCharacter* GetAlivePlayer(const int PossiblePlayerId);
	UPROPERTY(EditDefaultsOnly)
	//Array of teams for this gamemode
	TArray<struct FTeam> PossiblePlayersTeams;
};
