// Copyright Epic Games, Inc. All Rights Reserved.

#include "My_TDS_GameProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "My_TDS_GameProject/TDS_GameInstance.h"


AMy_TDS_GameProjectCharacter::AMy_TDS_GameProjectCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
		
		InventoryComponent = CreateDefaultSubobject<UTDS_Inventar>(TEXT ("InventoryComponet"));
		HealthComponent = CreateDefaultSubobject<UMyTDS_CharacterHealthComponent>(TEXT("HealthComponent"));

		if (HealthComponent)
		{
			HealthComponent->OnDead.AddDynamic(this, &AMy_TDS_GameProjectCharacter::CharDead);
		}
		if(InventoryComponent)
		{
			InventoryComponent->OnSwitchWeapon.AddDynamic(this, &AMy_TDS_GameProjectCharacter::InitWeapon);
		}
		

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMy_TDS_GameProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentCursor)
	{
		APlayerController* myPC = Cast<APlayerController>(GetController());
		if (myPC)
		{
			FHitResult TraceHitResult;
			myPC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();

			CurrentCursor->SetWorldLocation(TraceHitResult.Location);
			CurrentCursor->SetWorldRotation(CursorR);
		}
	}

	MovementTick(DeltaSeconds);
	//bIsMovingForward = UKismetMathLibrary::EqualEqual_VectorVector(TreceForward(), TreceVeloCity(), 20.0f);
	ChangeMovementState();
	CharSprint();



}

void AMy_TDS_GameProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	//InitWeapon(InitWeaponName);

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
	
}

void AMy_TDS_GameProjectCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
	Super::SetupPlayerInputComponent(NewInputComponent);

	NewInputComponent->BindAxis(TEXT("MoveForward"), this, &AMy_TDS_GameProjectCharacter::InputAxisX);
	NewInputComponent->BindAxis(TEXT("MoveRight"), this, &AMy_TDS_GameProjectCharacter::InputAxisY);
	
	NewInputComponent->BindAction((TEXT("FireEvent")), EInputEvent::IE_Pressed, this, & AMy_TDS_GameProjectCharacter::InputAttackPressed);
	NewInputComponent->BindAction((TEXT("FireEvent")), EInputEvent::IE_Released, this, &AMy_TDS_GameProjectCharacter::InputAttackReleased);

	NewInputComponent->BindAction((TEXT("ReloadEvent")), EInputEvent::IE_Released, this, &AMy_TDS_GameProjectCharacter::TryReloadWeapon);

	NewInputComponent->BindAction(TEXT("SwitchNextWeapon"), EInputEvent::IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TrySwitchNextWeapon); //E
	NewInputComponent->BindAction(TEXT("SwitchPreviosWeapon"), EInputEvent::IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TrySwitchPreviosWeapon); //Q
}

bool AMy_TDS_GameProjectCharacter::bSprintForward()
{
	return SprintRunEnabled && TrueVector;
}

bool AMy_TDS_GameProjectCharacter::CharSprint()
{
	FVector StartVelocity = GetCapsuleComponent()->GetRelativeLocation();
	FVector Velocity = StartVelocity + GetVelocity().GetSafeNormal() * 200.f;

	FVector StartForward = GetCapsuleComponent()->GetRelativeLocation();
	FVector Forward = StartForward + GetCapsuleComponent()->GetForwardVector() * 200.f;

	DrawDebugLine(GetWorld(), StartVelocity, Velocity, FColor::Green, false, 0.f, 0.f, 2.f);
	DrawDebugLine(GetWorld(), StartForward, Forward, FColor::Blue, false, 0.f, 0.f, 2.f);

	UE_LOG(LogTemp, Display, TEXT("SPRINT %f"), TrueVector);

	return TrueVector = Forward.Equals(Velocity, 80.f);
}

void AMy_TDS_GameProjectCharacter::InputAxisX(float Value)
{
	AxisX = Value;
}

void AMy_TDS_GameProjectCharacter::InputAxisY(float Value)
{
	AxisY = Value;
}

void AMy_TDS_GameProjectCharacter::InputAttackPressed()
{
	AttackCharEvent(true);
}

void AMy_TDS_GameProjectCharacter::InputAttackReleased()
{
	AttackCharEvent(false);
}

void AMy_TDS_GameProjectCharacter::MovementTick(float DeltaTime)
{
	if (bIsAlive)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisX);
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisY);

		if (MovementState == EmovementState::SprintRun_State)
		{
			FVector myRotationVector = FVector(AxisX, AxisY, 0.0f);
			FRotator myRotator = myRotationVector.ToOrientationRotator();
			SetActorRotation((FQuat(myRotator)));
		}
		else
		{
			APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (myController)
			{
				FHitResult ResultHit;
				//myController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery6, false, ResultHit);// bug was here Config\DefaultEngine.Ini
				myController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

				float FindRotaterResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
				SetActorRotation(FQuat(FRotator(0.0f, FindRotaterResultYaw, 0.0f)));

				if (CurrentWeapon)
				{
					FVector Displacement = FVector(0);
					switch (MovementState)
					{
					case EmovementState::Aim_State:
						Displacement = FVector(0.0f, 0.0f, 160.0f);
						CurrentWeapon->ShouldReduceDispersion = true;
						break;
					case EmovementState::AimWalk_State:
						CurrentWeapon->ShouldReduceDispersion = true;
						Displacement = FVector(0.0f, 0.0f, 160.0f);
						break;
					case EmovementState::Walk_State:
						Displacement = FVector(0.0f, 0.0f, 120.0f);
						CurrentWeapon->ShouldReduceDispersion = false;
						break;
					case EmovementState::Run_State:
						Displacement = FVector(0.0f, 0.0f, 120.0f);
						CurrentWeapon->ShouldReduceDispersion = false;
						break;
					case EmovementState::SprintRun_State:
						break;
					default:
						break;
					}

					CurrentWeapon->ShootEndLocation = ResultHit.Location + Displacement;
					//aim cursor like 3d Widget?
				}
			}
		}
	}

	
}

void AMy_TDS_GameProjectCharacter::AttackCharEvent(bool bIsFiring)
{
	AWeaponDefaults* myWeapon = nullptr;
	myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
	
		//ToDo Check melee or range
		myWeapon->SetWeaponStateFire(bIsFiring);
		
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("FIRE22222"));

}

void AMy_TDS_GameProjectCharacter::CharacterUpdate()
{
	float ResSpeed = 600.0f;
	switch (MovementState)
	{
	case EmovementState::Aim_State:
		ResSpeed = MovementInfo.AimSpeedNormal;
		break;
	case EmovementState::AimWalk_State:
		ResSpeed = MovementInfo.AimSpeedWalk;
		break;
	case EmovementState::Walk_State:
		ResSpeed = MovementInfo.WalkSpeedNormal;
		break;
	case EmovementState::Run_State:
		ResSpeed = MovementInfo.RunSpeedNormal;
		break;
	case EmovementState::SprintRun_State:
		ResSpeed = MovementInfo.SprinRunSpeedRun;
		break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = ResSpeed;
}

void AMy_TDS_GameProjectCharacter::ChangeMovementState()
{
	EmovementState NewState = EmovementState::Run_State;

	if (!WalkEnabled && !SprintRunEnabled && !AimEnabled)
	{
		NewState = EmovementState::Run_State;
	}
	else
	{
		if (bSprintForward())
		{
			WalkEnabled = false;
			AimEnabled = false;
			NewState = EmovementState::SprintRun_State;
		}

		else
		{
			if (WalkEnabled && !SprintRunEnabled && !AimEnabled && !TrueVector)
			{
				NewState = EmovementState::Walk_State;
			}
			else
			{
				if (!WalkEnabled && !SprintRunEnabled && AimEnabled && !TrueVector)
				{
					NewState = EmovementState::Aim_State;
				}
			}
		}
	}
	MovementState = NewState;
	CharacterUpdate();

	//Weapon state update
	AWeaponDefaults* myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		myWeapon->UpdateStateWeapon(MovementState);
	}
}

AWeaponDefaults* AMy_TDS_GameProjectCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

//Spawn weapon trought the table
void AMy_TDS_GameProjectCharacter::InitWeapon(FName IdWeaponName, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	UTDS_GameInstance* myGI = Cast<UTDS_GameInstance>(GetGameInstance());
	FWeaponInfo myWeaponInfo;
	if (myGI)
	{
		if (myGI->GetWeaponInfoByName(IdWeaponName, myWeaponInfo))
		{
			if (myWeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				AWeaponDefaults* myWeapon = Cast<AWeaponDefaults>(GetWorld()->SpawnActor(myWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (myWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					myWeapon->AttachToComponent(GetMesh(), Rule, FName("WeaponSocetRightHand"));
					CurrentWeapon = myWeapon;

					

					myWeapon->WeaponSetting = myWeaponInfo;

					//myWeapon->AdditionalWeaponInfo.Round = myWeaponInfo.MaxRound;

					//Remove !!! Debug
					myWeapon->ReloadTime = myWeaponInfo.ReloadTime;
					myWeapon->UpdateStateWeapon(MovementState);

					myWeapon->AdditionalWeaponInfo = WeaponAdditionalInfo;
					//if (InventoryComponent)
					CurrentIndexWeapon = NewCurrentIndexWeapon;//InventoryComponent -> GetWeaponIndexSlotByName(IdWeaponName);

					myWeapon->OnWeaponReloadStart.AddDynamic(this, &AMy_TDS_GameProjectCharacter::WeaponReloadStart);

					myWeapon->OnWeaponReloadEnd.AddDynamic(this, &AMy_TDS_GameProjectCharacter::WeaponReloadEnd);

					myWeapon->OnWeaponFireStart.AddDynamic(this, &AMy_TDS_GameProjectCharacter::WeaponFireStart);

					if (CurrentWeapon->GetWeaponRound() <= 0 && CurrentWeapon->CheckCanWeaponReload())
						CurrentWeapon->InitReload();

					if (InventoryComponent)
						InventoryComponent->OnWeaponAmmoAviable.Broadcast(myWeapon->WeaponSetting.WeaponType);

					UE_LOG(LogTemp, Warning, TEXT("WeaponSpawn"));
				}
			}
		}
		
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::InitWeapon - Weapon not found in table -NULL"));
			UE_LOG(LogTemp, Warning, TEXT("WeaponSpawn"));

		}
	}
}

void AMy_TDS_GameProjectCharacter::TryReloadWeapon()
{
	if (CurrentWeapon && !CurrentWeapon->WeaponReloading)
	{
		if (CurrentWeapon->GetWeaponRound() < CurrentWeapon->WeaponSetting.MaxRound && CurrentWeapon->CheckCanWeaponReload())
			CurrentWeapon->InitReload();

		//UE_LOG(LogTemp, Warning, TEXT("WeaponSpawn"));
	}
}

void AMy_TDS_GameProjectCharacter::WeaponReloadStart(UAnimMontage* Anim)
{

	WeaponReloadStart_BP(Anim);

}

void AMy_TDS_GameProjectCharacter::WeaponReloadEnd(bool bIsSuccess, int32 AmmoTake)
{
	if (InventoryComponent && CurrentWeapon)
	{
		InventoryComponent->AmmoSlotChangeValue(CurrentWeapon->WeaponSetting.WeaponType, AmmoTake);
		InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->AdditionalWeaponInfo);
	}
	WeaponReloadEnd_BP(bIsSuccess);
}

void AMy_TDS_GameProjectCharacter::WeaponFireStart(UAnimMontage* Anim)
{

	if (InventoryComponent && CurrentWeapon)
		InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->AdditionalWeaponInfo);
	WeaponFireStart_BP(Anim);
}

void AMy_TDS_GameProjectCharacter::WeaponFireStart_BP_Implementation(UAnimMontage* Anim)
{

}







void AMy_TDS_GameProjectCharacter::WeaponReloadStart_BP_Implementation(UAnimMontage* Anim)
{

}

void AMy_TDS_GameProjectCharacter::WeaponReloadEnd_BP_Implementation(bool bIsSuccess)
{

}





//Running with acceleration
FVector AMy_TDS_GameProjectCharacter::TreceVeloCity() const
{
	FVector StartPosition = GetCapsuleComponent()->GetRelativeLocation();
	FVector Velocity = StartPosition + GetVelocity().GetSafeNormal() * 1000.f;


	return FVector();
}

//Walking
FVector AMy_TDS_GameProjectCharacter::TreceForward() const
{
	FVector StartPosition = GetCapsuleComponent()->GetRelativeLocation();
	FVector DirectionVector = StartPosition + GetCapsuleComponent()->GetForwardVector() * 100.f;


	return FVector();
}


UDecalComponent* AMy_TDS_GameProjectCharacter::GetCursorToWorld()
{
	return CurrentCursor;
}

void AMy_TDS_GameProjectCharacter::TrySwitchNextWeapon()
{
	if (InventoryComponent->WeaponSlots.Num() > 1)
	{
		//We have more then one weapon go switch
		int8 OldIndex = CurrentIndexWeapon;
		FAdditionalWeaponInfo OldInfo;
		if (CurrentWeapon)
		{
			OldInfo = CurrentWeapon->AdditionalWeaponInfo;
			if (CurrentWeapon->WeaponReloading)
				CurrentWeapon->CancelReload();
		}

		if (InventoryComponent)
		{
			if (InventoryComponent->SwitchWeaponToIndex(CurrentIndexWeapon + 1, OldIndex, OldInfo, true))
			{
			}
		}
	}
}

void AMy_TDS_GameProjectCharacter::TrySwitchPreviosWeapon()
{
	if (InventoryComponent->WeaponSlots.Num() > 1)
	{
		int8 OldIndex = CurrentIndexWeapon;
			FAdditionalWeaponInfo OldInfo;
		if (CurrentWeapon)
		{
			OldInfo = CurrentWeapon->AdditionalWeaponInfo;
			if (CurrentWeapon->WeaponReloading)
				CurrentWeapon -> CancelReload();
		}

		if (InventoryComponent)
		{
			if (InventoryComponent->SwitchWeaponToIndex(CurrentIndexWeapon - 1, OldIndex, OldInfo, false))
			{ }
		}
	}
}

void AMy_TDS_GameProjectCharacter::CharDead()
{
	float TimeAnim = 0.0f;
	int32 rnd = FMath::RandHelper(DeadsAnim.Num());
	if (DeadsAnim.IsValidIndex(rnd) && DeadsAnim[rnd] && GetMesh() && GetMesh()->GetAnimInstance())
	{
		TimeAnim = DeadsAnim[rnd]->GetPlayLength();
		GetMesh()->GetAnimInstance()->Montage_Play(DeadsAnim[rnd]);
	}

	bIsAlive = false;
	UnPossessed();
	//Timer Ragdoll

	GetWorldTimerManager().SetTimer(TimerHandle_RagDollTimer, this, &AMy_TDS_GameProjectCharacter::EnableRagDoll, TimeAnim, false);

	GetCursorToWorld()->SetVisibility(false);
}

void AMy_TDS_GameProjectCharacter::EnableRagDoll()
{
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
	}
}

float AMy_TDS_GameProjectCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (bIsAlive)
	{
		HealthComponent->ChangeHealthValue(-DamageAmount);

	}

	return ActualDamage;
	
}
