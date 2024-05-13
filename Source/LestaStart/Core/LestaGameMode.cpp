// Fill out your copyright notice in the Description page of Project Settings.

#include "LestaGameMode.h"
#include "../Enemy/Enemy.h"
#include "TeamComponent.h"

//Since it is GameMode that determines the commands, we define them here
ALestaGameMode::ALestaGameMode()
{
	PossiblePlayersTeams = {
		{ETeams::Red, {ETeams::Red}, {ETeams::Blue, ETeams::Bot}, {ETeams::Neutral}, FLinearColor(1, 0, 0 )},
		{ETeams::Blue, {ETeams::Blue}, {ETeams::Red, ETeams::Bot}, {ETeams::Neutral}, FLinearColor(0,0,1)}
	};
}

void ALestaGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AEnemy*> OccupiedSpawnPoints;
	for (const FTransform& SpawnPoint : SpawnPoints)
	{
		bool bIsEnemyPresent = false;
		for (AEnemy* SpawnedActor : OccupiedSpawnPoints)
		{
			if (SpawnedActor->GetActorLocation().Equals(SpawnPoint.GetLocation(), SpawnTolerance))
			{
				bIsEnemyPresent = true;
				break;
			}
		}
		if (!bIsEnemyPresent)
		{
			const int32 RandomIndex = FMath::RandRange(0, 1);
			TSubclassOf<AEnemy> EnemyClass = RandomIndex == 0 ? CubeClass : SentryClass;
			FActorSpawnParameters SpawnParams;
			AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnPoint.GetLocation(), SpawnPoint.Rotator(), SpawnParams);
			if (NewEnemy)
			{
				NewEnemy->SetActorScale3D(SpawnPoint.GetScale3D());
				OccupiedSpawnPoints.Add(NewEnemy);
			}
		}
		else
		{
			UE_LOG(LogInput, Error, TEXT("Point occupaied x: %f y: %f z: %f"), SpawnPoint.GetLocation().X, SpawnPoint.GetLocation().Y, SpawnPoint.GetLocation().Z);
		}
	}
}


FTeam ALestaGameMode::AddPlayer(ALestaCharacter* NewPlayer)
{
	PlayersCharactersList.AddUnique(NewPlayer);
	return PossiblePlayersTeams[PlayersCharactersList.Num() % PossiblePlayersTeams.Num()];
}

void ALestaGameMode::RemovePlayer(ALestaCharacter* RemovedPlayer)
{
	if (PlayersCharactersList.Contains(RemovedPlayer))
	{
		PlayersCharactersList.Remove(RemovedPlayer);
	}

	if (PlayersCharactersList.Num() == 0)
	{
		UGameInstance* GameInstance = GetGameInstance();
		if (!GameInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameInstance not found!"));
			return;
		}
		//Level reload
		FString CurrentLevel = GetWorld()->GetMapName();
		if (GameInstance->GetWorld())
		{
			GameInstance->GetWorld()->ServerTravel(CurrentLevel);
		}
	}
	
}


ALestaCharacter* ALestaGameMode::GetAlivePlayer(const int PossiblePlayerId)
{
	return PlayersCharactersList[PossiblePlayerId % PlayersCharactersList.Num()];
}

