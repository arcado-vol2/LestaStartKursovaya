// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingUIWidget.generated.h"

UCLASS()
class LESTASTART_API UEnemyFloatingUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void SetHealth(float CurrentHealth, float MaxHealth);
	UFUNCTION()
	virtual void SetTeamColor(const FLinearColor TeamColor);
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HealthTextBlock;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* TeamColorBlock;
	
	
};
