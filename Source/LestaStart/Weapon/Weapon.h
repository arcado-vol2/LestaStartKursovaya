#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM()
enum EWeaponState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Firing UMETA(DisplayName = "Firing"),
	Reloading UMETA(DisplayName = "Reloading"),
	Cooldown UMETA(DisplayName = "Cooldown"),
};

UENUM()
enum EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	Laser UMETA(DisplayName = "Laser"),
	Charge UMETA(DisplayName = "Charge"),
	Cooled UMETA(DisplayName = "Cooled"),
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangeDelegate, float, Clip, float, Belt );
UCLASS(Abstract)
class LESTASTART_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Configaration")
	TEnumAsByte<EWeaponType> WeaponType = EWeaponType::None;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UPROPERTY(Replicated)
	TEnumAsByte<EWeaponState> WeaponState;
	UPROPERTY(Replicated)
	bool IsFiringVisual;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stats")
	float Range = 600;
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stats")
	float Damage = 8;
	

	//Because all our weapons fire every frame, everything connected with ammo should be float
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stats")
	float BeltSize = 500;
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stats")
	float ClipSize = 100;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentClip, VisibleInstanceOnly, Category = "Stats")
	float CurrentClip;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentBelt, VisibleInstanceOnly, Category = "Stats")
	float CurrentBelt;
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stats")
	float AmmoPerSecond = 10;
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stats")
	float ReloadTime = 2;
	
	UFUNCTION()
	virtual void OnRep_CurrentClip();
	UFUNCTION()
	virtual void OnRep_CurrentBelt();

	virtual void Fire(const float DeltaTime);

	UFUNCTION()
	virtual void OnEndFire();
	
	
public:
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnAmmoChangeDelegate OnAmmoChangeDelegate;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY( VisibleAnywhere, Category = "State")
	class ALestaCharacter* CurrentOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	FTransform PlacementTransform;

	UFUNCTION()
	virtual void StartFire();
	
	UFUNCTION()
	virtual void EndFire();

	UFUNCTION()
	virtual void Reload();
	
	UFUNCTION(Server, Reliable)
	virtual void Server_StartFire();

	UFUNCTION(Server, Reliable)
	virtual void Server_EndFire();
	
	UFUNCTION()
	virtual void FireEffect();

	UFUNCTION()
	virtual FIntPoint GetCurrentAmmoInfo();

	UFUNCTION()
	virtual EWeaponState GetWeaponState();

	UFUNCTION(Server, Reliable)
	virtual void Server_Reload();

	UFUNCTION()
	virtual EWeaponType GetWeaponType();

	
	
};
