// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingUIWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyFloatingUI::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth/MaxHealth);
		HealthTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::Max(FMath::TruncToInt(CurrentHealth), 0), FMath::TruncToInt(MaxHealth))));
	}
}

void UEnemyFloatingUI::SetTeamColor(const FLinearColor TeamColor)
{
	TeamColorBlock->SetColorAndOpacity(TeamColor);
}
