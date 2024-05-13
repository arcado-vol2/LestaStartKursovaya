// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHud.h"
#include "../Weapon/Weapon.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHud::SetHealth(const float CurrentHealth, const float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth/MaxHealth);
	}
}

void UPlayerHud::SetAmmo(const float CurrentAmmo, const float MaxAmmo)
{
	if (AmmoText)
	{
		const FString AmmoString = FString::Printf(TEXT("%d / %d"), FMath::Max(FMath::TruncToInt(CurrentAmmo), 0), FMath::TruncToInt(MaxAmmo));
		AmmoText->SetText(FText::FromString(AmmoString));
	}
}

void UPlayerHud::ChangeWeapon(const EWeaponType WeaponType)
{
	switch (WeaponType)
	{
	case (EWeaponType::Charge):
		ChargeBar->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		ChargeBar->SetVisibility(ESlateVisibility::Hidden);
		break;
	}
}

void UPlayerHud::SetCharge(const float CurrentCharge, const float MaxCharge)
{
	if (ChargeBar)
	{
		ChargeBar->SetPercent(CurrentCharge/MaxCharge);
	}
}
