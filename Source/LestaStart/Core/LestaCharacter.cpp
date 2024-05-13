#include "LestaCharacter.h"
#include "EnhancedInputComponent.h"
#include "HealthComponent.h"
#include "LestaGameMode.h"
#include "Camera/CameraComponent.h"
#include "../Weapon/Weapon.h"
#include "Net/UnrealNetwork.h"
#include "../UI/PlayerHud.h"
#include "LestaPlayerController.h"
#include "TeamComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LestaStart/UI/FloatingUIComponent.h"
#include "LestaStart/Weapon/ChargeWeapon.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ALestaCharacter::ALestaCharacter()
{
	NetUpdateFrequency = 60.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CameraComponent->bUsePawnControlRotation = true; // Camera rotation is synchronized with Player Controller rotation
	CameraComponent->SetupAttachment(GetMesh(), FName("Head"));

	Hud = nullptr;
	HudClass = nullptr;
	HealthWidget = CreateDefaultSubobject<UFloatingUIComponent>(TEXT("Health Widget"));
	HealthWidget->SetupAttachment(GetMesh());
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("Team"));
	
	AIStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerception Stimulate Source"));
	AIStimuliSourceComponent->bAutoRegister = true;
	AIStimuliSourceComponent->RegisterForSense(UAISenseConfig_Sight::StaticClass());
}

void ALestaCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Create HUD
	if (IsLocallyControlled() && HudClass)
	{
		ALestaPlayerController* PlayerController = GetController<ALestaPlayerController>();
		check(PlayerController);
		Hud = CreateWidget<UPlayerHud>(PlayerController, HudClass);
		check(Hud);
		Hud->AddToPlayerScreen();
		if(HealthComponent)
		{
			HealthComponent->OnHealthChangeDelegate.AddDynamic(Hud, &UPlayerHud::SetHealth);
			
		}
		SetupWeaponUI(nullptr);
	}
	

	//Crate weapons
	if (HasAuthority())
	{
		for (const TSubclassOf<AWeapon>& WeaponClass: StartWeaponsClasses)
		{
			if (!WeaponClass)
				continue;
			FActorSpawnParameters WeaponParameters;
			WeaponParameters.Owner = this;
			AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, WeaponParameters);
			const int32 WeaponIndex = WeaponsList.Add(NewWeapon);
			if (CurrentWeaponIndex == WeaponIndex)
			{
				CurrentWeapon = NewWeapon;
				OnRep_CurrentWeapon(nullptr);
			}
		}
	}

	//Setup componentns
	if (HealthComponent)
	{
		HealthComponent->OnHealthGone.AddDynamic(this, &ALestaCharacter::OnDeath);
	}
	ALestaGameMode* LestaGameMode = Cast<ALestaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (LestaGameMode)
	{
		FTeam MyTeam = LestaGameMode->AddPlayer(this);
		if (TeamComponent)
		{
			TeamComponent->SetCurrentTeam(MyTeam);
		}
	}

	//Create floating health UI
	if (HealthWidget)
	{
		if (IsLocallyControlled())
		{
			HealthWidget->SetVisibility(false);
			HealthWidget->RemoveFromRoot();
			HealthWidget = nullptr;
		}
		else if (HealthComponent && TeamComponent)
		{
			HealthComponent->OnHealthChangeDelegate.AddDynamic(HealthWidget, &UFloatingUIComponent::SetHealth);
			HealthWidget->SetStartValues(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth(), TeamComponent->GetTeamColor());
		}
	}
	
}

void ALestaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ALestaCharacter, WeaponsList, COND_None);
	DOREPLIFETIME_CONDITION(ALestaCharacter, CurrentWeapon, COND_None);
}

void ALestaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EIC) // should validate component because developers can change input component class through Project Settings
	{
		EIC->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMoveInput);
		EIC->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnLookInput);
		EIC->BindAction(ShootInputAction, ETriggerEvent::Started, this, &ALestaCharacter::OnShootStart);
		EIC->BindAction(ShootInputAction, ETriggerEvent::Completed, this, &ALestaCharacter::OnShootEnd);
		EIC->BindAction(WeaponScrollAction, ETriggerEvent::Triggered, this, &ALestaCharacter::OnWeaponScroll);
		EIC->BindAction(WeaponSelect1Action, ETriggerEvent::Started, this, &ALestaCharacter::OnWeaponSelect, 1);
		EIC->BindAction(WeaponSelect2Action, ETriggerEvent::Started, this, &ALestaCharacter::OnWeaponSelect, 2);
		EIC->BindAction(ReloadAction, ETriggerEvent::Started, this, &ALestaCharacter::OnReload);
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ALestaCharacter::OnJump);
	}
	else
	{
		UE_LOG(LogInput, Error, TEXT("Unexpected input component class: %s"), *GetFullNameSafe(PlayerInputComponent))
	}
}


void ALestaCharacter::OnMoveInput(const FInputActionInstance& InputActionInstance)
{
	//this tmp soultion, better use a mapping context
	if (HealthComponent->GetCurrentGameplayStatus() == EGameplayActorStatus::Dead)
	{
		return;
	}
	const float YawDegree = GetControlRotation().Yaw; // controller rotation Yaw (in degrees)
	const float YawRadian = FMath::DegreesToRadians(YawDegree); // controller rotation Yaw (in radians)
	const FVector ForwardDirection = FVector(FMath::Cos(YawRadian), FMath::Sin(YawRadian), 0.f);
	const FVector RightDirection = FVector(FMath::Cos(YawRadian + UE_HALF_PI), FMath::Sin(YawRadian + UE_HALF_PI), 0.f);

	const FVector2D Input2D = InputActionInstance.GetValue().Get<FVector2D>();
	AddMovementInput(ForwardDirection * Input2D.X + RightDirection * Input2D.Y);


}

void ALestaCharacter::OnLookInput(const FInputActionInstance& InputActionInstance)
{
	const FVector2D Input2D = InputActionInstance.GetValue().Get<FVector2D>();
	AddControllerYawInput(Input2D.X);
	AddControllerPitchInput(Input2D.Y);
}

void ALestaCharacter::OnRep_CurrentWeapon(const AWeapon* PrevWeapon)
{
	
	if (PrevWeapon)
	{
		PrevWeapon->Mesh->SetVisibility(false);
	}
	
	if (CurrentWeapon)
	{
		if (!CurrentWeapon->CurrentOwner)
		{
			const FTransform& StartTransform = CurrentWeapon->PlacementTransform * GetMesh()->GetSocketTransform(FName("weapon_r"));
			CurrentWeapon->SetActorTransform(StartTransform, false, nullptr, ETeleportType::TeleportPhysics);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("weapon_r"));
			CurrentWeapon->CurrentOwner = this;
		}
		CurrentWeapon->Mesh->SetVisibility(true);
	}
	SetupWeaponUI(PrevWeapon);
	OnWeaponChangeDelegate.Broadcast(CurrentWeapon, PrevWeapon);
}

void ALestaCharacter::OnShootStart()
{
	//this tmp soultion, better use a mapping context
	if (HealthComponent->GetCurrentGameplayStatus() == EGameplayActorStatus::Dead)
	{
		return;
	}
	if (CurrentWeapon)
		CurrentWeapon->StartFire();
}

void ALestaCharacter::OnShootEnd()
{
	//this tmp soultion, better use a mapping context
	if (HealthComponent->GetCurrentGameplayStatus() == EGameplayActorStatus::Dead)
	{
		return;
	}
	if (CurrentWeapon)
		CurrentWeapon->EndFire();
}

void ALestaCharacter::OnWeaponScroll(const FInputActionInstance& InputActionInstance)
{
	//this tmp soultion, better use a mapping context
	if (HealthComponent->GetCurrentGameplayStatus() == EGameplayActorStatus::Dead)
	{
		return;
	}
	if (InputActionInstance.GetValue().Get<float>() > 0.)
	{
		EquipWeapon((CurrentWeaponIndex + 1) % WeaponsList.Num());
	}
	else
	{
		EquipWeapon((CurrentWeaponIndex - 1 + WeaponsList.Num()) % WeaponsList.Num());
	}
	
}
void ALestaCharacter::OnReload()
{
	//this tmp soultion, better use a mapping context
	if (HealthComponent->GetCurrentGameplayStatus() == EGameplayActorStatus::Dead)
	{
		return;
	}
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void ALestaCharacter::OnJump()
{
	//this tmp soultion, better use a mapping context
	if (HealthComponent->GetCurrentGameplayStatus() == EGameplayActorStatus::Dead)
	{
		return;
	}
	this->Jump();
}


void ALestaCharacter::Server_SetCurrentWeapon_Implementation(AWeapon* NewWeapon)
{
	const AWeapon* PrevWeapon = CurrentWeapon;
	CurrentWeapon = NewWeapon;
	OnRep_CurrentWeapon(PrevWeapon);
}



void ALestaCharacter::EquipWeapon(const int32 Index)
{
	if (!WeaponsList.IsValidIndex(Index) || CurrentWeapon == WeaponsList[Index] || CurrentWeapon->GetWeaponState() != EWeaponState::Idle)
	{
		return;
	}
	if (IsLocallyControlled())
	{
		CurrentWeaponIndex = Index;
		const AWeapon* PrevWeapon = CurrentWeapon;
		CurrentWeapon = WeaponsList[Index];
		
		
		OnRep_CurrentWeapon(PrevWeapon);
		if (GetLocalRole() < ROLE_Authority)
		{
			Server_SetCurrentWeapon(CurrentWeapon);
		}
	}
	else if (!HasAuthority())
	{
		Server_SetCurrentWeapon(WeaponsList[Index]);
	}
	
}


void ALestaCharacter::OnWeaponSelect(const int32 Id)
{
	//Mb better to use Id as index of weapon in list, but i prefer it like it's a num on keyboard
	EquipWeapon(Id - 1);
}


void ALestaCharacter::SetupWeaponUI(const AWeapon* PrevWeapon)
{
	if (!Hud || !IsLocallyControlled() || !CurrentWeapon)
		return;
	if (CurrentWeapon)
	{
		if(!CurrentWeapon->OnAmmoChangeDelegate.IsBound())
		{
			CurrentWeapon->OnAmmoChangeDelegate.AddDynamic(Hud, &UPlayerHud::SetAmmo);
		}

		if (AChargeWeapon* CurrentChargeWeapon = Cast<AChargeWeapon>(CurrentWeapon))
		{
			if (!CurrentChargeWeapon->OnChargeChangedDelegate.IsBound())
			{
				CurrentChargeWeapon->OnChargeChangedDelegate.AddDynamic(Hud, &UPlayerHud::SetCharge);
			}
		}
		Hud->ChangeWeapon(CurrentWeapon->GetWeaponType());
		const FIntPoint AmmoInfo = CurrentWeapon->GetCurrentAmmoInfo();
		Hud->SetAmmo(AmmoInfo[0], AmmoInfo[1]);
	}
}

TObjectPtr<UCameraComponent> ALestaCharacter::GetCamera()
{
	return CameraComponent;
}

void ALestaCharacter::OnDeath()
{
	if (AIStimuliSourceComponent)
	{
		AIStimuliSourceComponent->UnregisterFromPerceptionSystem();
	}
	Destroy();
}


void ALestaCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//Destroy HUD
	if (Hud)
	{
		Hud->RemoveFromParent();
		Hud = nullptr;
	}
	//Destroy weapons
	for (const auto Weapon : WeaponsList)
	{
		Weapon->Destroy();
	}
	//Destroy health widget
	if (HealthWidget)
	{
		HealthWidget->SetVisibility(false);
		HealthWidget->RemoveFromRoot();
		HealthWidget = nullptr;
	}
	//Tell Game Mode to remove this player from alive player list
	if (ALestaGameMode* LestaGameMode = Cast<ALestaGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		LestaGameMode->RemovePlayer(this);
	}
	
}
