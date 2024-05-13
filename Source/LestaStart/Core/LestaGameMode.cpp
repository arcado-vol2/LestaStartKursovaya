// Fill out your copyright notice in the Description page of Project Settings.

#include "LestaGameMode.h"

#include "TeamComponent.h"

//Since it is GameMode that determines the commands, we define them here
ALestaGameMode::ALestaGameMode()
{
	PossiblePlayersTeams = {
		{ETeams::Red, {ETeams::Red}, {ETeams::Blue, ETeams::Bot}, {ETeams::Neutral}, FLinearColor(1, 0, 0 )},
		{ETeams::Blue, {ETeams::Blue}, {ETeams::Red, ETeams::Bot}, {ETeams::Neutral}, FLinearColor(0,0,1)}
	};
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

