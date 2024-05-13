#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

enum EWeaponType : uint8;

UCLASS()
class LESTASTART_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* ChargeBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AmmoText;

	UFUNCTION()
	virtual void SetHealth(float CurrentHealth, float MaxHealth);
	UFUNCTION()
	virtual void ChangeWeapon(const EWeaponType WeaponType);
	UFUNCTION()
	virtual void SetAmmo(float CurrentAmmo, float MaxAmmo);
	UFUNCTION()
	virtual void SetCharge(float CurrentCharge, float MaxCharge);
	
};
