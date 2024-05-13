// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingUIComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "FloatingUIWidget.h"


UFloatingUIComponent::UFloatingUIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetWorldScale3D(FVector(0.175, 0.175, 0.175));
}


void UFloatingUIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		SetWorldRotation(FRotator(
			-FMath::UnwindDegrees(PlayerController->GetControlRotation().Pitch),
			FMath::UnwindDegrees(PlayerController->GetControlRotation().Yaw + 180.f),
			0.f)
			);
	}
	
	
}

void UFloatingUIComponent::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (UEnemyFloatingUI* FloatingWidget = Cast<UEnemyFloatingUI>(GetWidget()))
	{
		FloatingWidget->SetHealth(CurrentHealth, MaxHealth);
	}
}

void UFloatingUIComponent::SetStartValues(float CurrentHealth, float MaxHealth, FLinearColor TeamColor)
{
	if (UEnemyFloatingUI* FloatingWidget = Cast<UEnemyFloatingUI>(GetWidget()))
	{
		FloatingWidget->SetHealth(CurrentHealth, MaxHealth);
		FloatingWidget->SetTeamColor(TeamColor);
	}
}
