#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "LestaCharacter.generated.h"

class AWeapon;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChangeDelegate, AWeapon*, CurrentWeapon, const AWeapon*, PrevWeapon);
UCLASS()
class LESTASTART_API ALestaCharacter : public ACharacter
{
	GENERATED_BODY() 

public:
	ALestaCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	TArray<TSubclassOf<AWeapon>> StartWeaponsClasses;
public:
	//Weapon
	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Weapons" )
	TArray<AWeapon*> WeaponsList;
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing = OnRep_CurrentWeapon, Category = "Weapons")
	AWeapon* CurrentWeapon;
	UPROPERTY(VisibleInstanceOnly, Category = "Weapons")
	int32 CurrentWeaponIndex = 0;
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnWeaponChangeDelegate OnWeaponChangeDelegate;
	UFUNCTION(Category = "Character")
	virtual void EquipWeapon(const int32 Index);
	
	//UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UPlayerHud> HudClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	class UPlayerHud* Hud;
	UPROPERTY(EditAnywhere, Category = "UI")
	class UFloatingUIComponent* HealthWidget;
	UFUNCTION()
	virtual void SetupWeaponUI(const AWeapon* PrevWeapon);
	
	//Stats
	UPROPERTY( EditDefaultsOnly, Category = "Stats")
	class UHealthComponent* HealthComponent;
	UPROPERTY( EditDefaultsOnly, Category = "Stats")
	class UTeamComponent* TeamComponent;
	
	//AI
	UPROPERTY(EditDefaultsOnly)
	class UAIPerceptionStimuliSourceComponent* AIStimuliSourceComponent;

	//Other
	virtual TObjectPtr<UCameraComponent> GetCamera();
	
protected:

	UFUNCTION()
	virtual void OnRep_CurrentWeapon(const class AWeapon* PrevWeapon);
	UFUNCTION(Server, Reliable)
	void Server_SetCurrentWeapon(class AWeapon* NewWeapon);
	
	//Camera
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
	

	//Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ShootInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> WeaponScrollAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> WeaponSelect1Action;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> WeaponSelect2Action;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ReloadAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	//Input handlers
	virtual void OnMoveInput(const FInputActionInstance& InputActionInstance);
	virtual void OnLookInput(const FInputActionInstance& InputActionInstance);
	virtual void OnShootStart();
	virtual void OnShootEnd();
	virtual void OnWeaponSelect(const int Id);
	virtual void OnWeaponScroll(const FInputActionInstance& InputActionInstance);
	virtual void OnReload();
	virtual void OnJump();

	//Other
	UFUNCTION()
	void OnDeath();
	

};