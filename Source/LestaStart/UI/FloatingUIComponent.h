// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "FloatingUIComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API UFloatingUIComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFloatingUIComponent();


protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
	virtual void SetHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	virtual void SetStartValues(float CurrentHealth, float MaxHealth, FLinearColor TeamColor);
	
};
