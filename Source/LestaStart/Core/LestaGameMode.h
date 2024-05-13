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
	virtual void BeginPlay() override;
	//Alive players array
	UPROPERTY()
	TArray<ALestaCharacter*> PlayersCharactersList;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AEnemy> CubeClass;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AEnemy> SentryClass;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TArray<FTransform> SpawnPoints;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float SpawnTolerance = 10.0f;
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
