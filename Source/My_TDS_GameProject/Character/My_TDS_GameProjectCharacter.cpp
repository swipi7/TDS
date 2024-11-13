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
#include "ProjectileDefault.h"
#include "My_TDS_GameProject.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/ActorChannel.h"
#include "TDS_HealthComponent.h"


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
		HealthComponent = CreateDefaultSubobject<UTDS_HealthComponent>(TEXT("HealthComponent"));

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

	//NetWork
	bReplicates = true;
}

void AMy_TDS_GameProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentCursor)
	{
		APlayerController* myPC = Cast<APlayerController>(GetController());
		if (myPC && myPC->IsLocalPlayerController())
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
	//ChangeMovementState();
	CharSprint();



}

void AMy_TDS_GameProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	Invul = false;


	if (GetWorld() && GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		//InitWeapon(InitWeaponName);

		if (CursorMaterial && (GetLocalRole() == ROLE_AutonomousProxy || GetLocalRole() == ROLE_Authority))
		{
			CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
		}
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

	NewInputComponent->BindAction(TEXT("SwitchNextWeapon"), EInputEvent::IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TrySwitchNextWeapon_OnServer);
	NewInputComponent->BindAction(TEXT("TrySwitchPreviosWeapon"), EInputEvent::IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TrySwitchPreviosWeapon_OnServer);

	NewInputComponent->BindAction(TEXT("AbilityAction"), EInputEvent::IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TryAbilityEnabled);

	NewInputComponent->BindAction(TEXT("AbilityInvul"), EInputEvent::IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TryAbilityInvul);

	NewInputComponent->BindAction(TEXT("DropCurrentWeapon"), EInputEvent::IE_Pressed, this, &AMy_TDS_GameProjectCharacter::DropCurrentWeapon);

	/*TArray<FKey> HotKeys;
	HotKeys.Add(EKeys::One);
	HotKeys.Add(EKeys::Two);
	HotKeys.Add(EKeys::Three);
	HotKeys.Add(EKeys::Four);
	HotKeys.Add(EKeys::Five);
	HotKeys.Add(EKeys::Six);
	HotKeys.Add(EKeys::Seven);
	HotKeys.Add(EKeys::Eight);
	HotKeys.Add(EKeys::Nine);
	HotKeys.Add(EKeys::Zero);

	NewInputComponent->BindKey(HotKeys[1], IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TKeyPressed<1>);
	NewInputComponent->BindKey(HotKeys[2], IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TKeyPressed<2>);
	NewInputComponent->BindKey(HotKeys[3], IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TKeyPressed<3>);
	NewInputComponent->BindKey(HotKeys[4], IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TKeyPressed<4>);
	NewInputComponent->BindKey(HotKeys[5], IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TKeyPressed<5>);
	NewInputComponent->BindKey(HotKeys[6], IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TKeyPressed<6>);
	NewInputComponent->BindKey(HotKeys[7], IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TKeyPressed<7>);
	NewInputComponent->BindKey(HotKeys[8], IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TKeyPressed<8>);
	NewInputComponent->BindKey(HotKeys[9], IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TKeyPressed<9>);
	NewInputComponent->BindKey(HotKeys[0], IE_Pressed, this, &AMy_TDS_GameProjectCharacter::TKeyPressed<0>);*/
}

bool AMy_TDS_GameProjectCharacter::bSprintForward()
{
	return SprintRunEnabled && TrueVector;
}

bool AMy_TDS_GameProjectCharacter::TrySwitchWeaponToIndexByKeyInput(int32 ToIndex)
{
	bool bIsSuccess = false;
	if (CurrentWeapon && !CurrentWeapon->WeaponReloading && InventoryComponent->WeaponSlots.IsValidIndex(ToIndex))
	{
		if (CurrentIndexWeapon != ToIndex && InventoryComponent)
		{
			int32 OldIndex = CurrentIndexWeapon;
			FAdditionalWeaponInfo OldInfo;

			if (CurrentWeapon)
			{
				OldInfo = CurrentWeapon->AdditionalWeaponInfo;
				if (CurrentWeapon->WeaponReloading)
					CurrentWeapon->CancelReload();
			}

			bIsSuccess = InventoryComponent->SwitchWeaponByIndex(ToIndex, OldIndex, OldInfo);
		}
	}
	return bIsSuccess;
}

void AMy_TDS_GameProjectCharacter::DropCurrentWeapon()
{
	if (InventoryComponent)
	{
		FDropItem ItemInfo;
		InventoryComponent->DropWeapobByIndex_OnServer(CurrentIndexWeapon);
	}
}

bool AMy_TDS_GameProjectCharacter::CharSprint()
{
	FVector StartVelocity = GetCapsuleComponent()->GetRelativeLocation();
	FVector Velocity = StartVelocity + GetVelocity().GetSafeNormal() * 200.f;

	FVector StartForward = GetCapsuleComponent()->GetRelativeLocation();
	FVector Forward = StartForward + GetCapsuleComponent()->GetForwardVector() * 200.f;

	DrawDebugLine(GetWorld(), StartVelocity, Velocity, FColor::Green, false, 0.f, 0.f, 2.f);
	DrawDebugLine(GetWorld(), StartForward, Forward, FColor::Blue, false, 0.f, 0.f, 2.f);

	//UE_LOG(LogTemp, Display, TEXT("SPRINT %f"), TrueVector);

	return TrueVector = Forward.Equals(Velocity, 80.f);
}

void AMy_TDS_GameProjectCharacter::ResetInvul()
{
	
	if (!GetWorld()->GetTimerManager().IsTimerActive(TimerInvul))
	{
		GetWorld()->GetTimerManager().SetTimer(TimerInvul, this, &AMy_TDS_GameProjectCharacter::ChangeInvul, InvulTim, false, -1.0f);
	}
}

void AMy_TDS_GameProjectCharacter::ChangeInvul()
{
	Invul = false;
}

void AMy_TDS_GameProjectCharacter::InputAxisX(float Value)
{
	if (GetIsAlive())
	{
		AxisX = Value;
	}
	
}

void AMy_TDS_GameProjectCharacter::InputAxisY(float Value)
{
	if (GetIsAlive())
	{
		AxisY = Value;
	}
}

void AMy_TDS_GameProjectCharacter::InputAttackPressed()
{

	if (HealthComponent && HealthComponent->GetIsAlive())
	{
		AttackCharEvent(true);

	}
}

void AMy_TDS_GameProjectCharacter::InputAttackReleased()
{
	AttackCharEvent(false);
}

void AMy_TDS_GameProjectCharacter::InputWalkPressed()
{
	WalkEnabled = true;
	ChangeMovementState();
}

void AMy_TDS_GameProjectCharacter::InputWalkReleased()
{
	WalkEnabled = false;
	ChangeMovementState();
}

void AMy_TDS_GameProjectCharacter::InputSprintPressed()
{
	SprintRunEnabled = true;
	ChangeMovementState();
}

void AMy_TDS_GameProjectCharacter::InputSprintReleased()
{
	SprintRunEnabled = false;
	ChangeMovementState();
}

void AMy_TDS_GameProjectCharacter::InputAimPressed()
{
	AimEnabled = true;
	ChangeMovementState();
}

void AMy_TDS_GameProjectCharacter::InputAimReleased()
{
	AimEnabled = false;
	ChangeMovementState();
}

void AMy_TDS_GameProjectCharacter::MovementTick(float DeltaTime)
{
	if (HealthComponent && HealthComponent->GetIsAlive())
	{

		if (GetController() && GetController()->IsLocalPlayerController())
		{
			AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisX);
			AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisY);

			FString SEnum = UEnum::GetValueAsString(GetMovementState());
			UE_LOG(LogMy_TDS_GameProject_Net, Error, TEXT("Movement State - %s"), *SEnum);

			if (MovementState == EMovementState::SprintRun_State)
			{
				FVector myRotationVector = FVector(AxisX, AxisY, 0.0f);
				FRotator myRotator = myRotationVector.ToOrientationRotator();

				SetActorRotation((FQuat(myRotator)));
				SetActorRotationByYaw_OnServer(myRotator.Yaw);
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
					SetActorRotationByYaw_OnServer(FindRotaterResultYaw);

					if (CurrentWeapon)
					{
						FVector Displacement = FVector(0);
						bool bIsReduceDispersion = false;
						switch (MovementState)
						{
						case EMovementState::Aim_State:
							Displacement = FVector(0.0f, 0.0f, 160.0f);
							//CurrentWeapon->ShouldReduceDispersion = true;
							bIsReduceDispersion = true;
							break;
						case EMovementState::AimWalk_State:
							//CurrentWeapon->ShouldReduceDispersion = true;
							Displacement = FVector(0.0f, 0.0f, 160.0f);
							bIsReduceDispersion = true;
							break;
						case EMovementState::Walk_State:
							Displacement = FVector(0.0f, 0.0f, 120.0f);
							//CurrentWeapon->ShouldReduceDispersion = false;
							break;
						case EMovementState::Run_State:
							Displacement = FVector(0.0f, 0.0f, 120.0f);
							//CurrentWeapon->ShouldReduceDispersion = false;
							break;
						case EMovementState::SprintRun_State:
							break;
						default:
							break;
						}
						//CurrentWeapon->ShootEndLocation = ResultHit.Location + Displacement;
						CurrentWeapon->UpdateWeaponByCharacterMovementState_OnServer(ResultHit.Location + Displacement, bIsReduceDispersion);
						//aim cursor like 3d Widget?
					}
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
		myWeapon->SetWeaponStateFire_OnServer(bIsFiring);
	}
	//else
		//UE_LOG(LogTemp, Warning, TEXT("FIRE22222"));
}

void AMy_TDS_GameProjectCharacter::CharacterUpdate()
{
	float ResSpeed = 600.0f;
	switch (MovementState)
	{
	case EMovementState::Aim_State:
		ResSpeed = MovementInfo.AimSpeedNormal;
		break;
	case EMovementState::AimWalk_State:
		ResSpeed = MovementInfo.AimSpeedWalk;
		break;
	case EMovementState::Walk_State:
		ResSpeed = MovementInfo.WalkSpeedNormal;
		break;
	case EMovementState::Run_State:
		ResSpeed = MovementInfo.RunSpeedNormal;
		break;
	case EMovementState::SprintRun_State:
		ResSpeed = MovementInfo.SprinRunSpeedRun;
		break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = ResSpeed;
}

void AMy_TDS_GameProjectCharacter::ChangeMovementState()
{
	EMovementState NewState = EMovementState::Run_State;

	if (!WalkEnabled && !SprintRunEnabled && !AimEnabled)
	{
		NewState = EMovementState::Run_State;
	}
	else
	{
		if (SprintRunEnabled)
		{
			WalkEnabled = false;
			AimEnabled = false;
			NewState = EMovementState::SprintRun_State;
		}
		else
		{
			if (WalkEnabled && !SprintRunEnabled && AimEnabled)
			{
				NewState = EMovementState::AimWalk_State;
			}
			else
			{
				if (WalkEnabled && !SprintRunEnabled && !AimEnabled)   // && !TrueVector
				{
					NewState = EMovementState::Walk_State;
				}
				else
				{
					if (!WalkEnabled && !SprintRunEnabled && AimEnabled)   // && TrueVector
					{
						NewState = EMovementState::Aim_State;
						
					}
				}
			}
		}
	}

	SetMovementState_OnServer(NewState);

	//MovementState = NewState;
	//CharacterUpdate();

	//Weapon state update
	AWeaponDefaults* myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		myWeapon->UpdateStateWeapon_OnServer(NewState);
	}
}

AWeaponDefaults* AMy_TDS_GameProjectCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

//Spawn weapon trought the table
void AMy_TDS_GameProjectCharacter::InitWeapon(FName IdWeaponName, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon)
{
	// On Server
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
					myWeapon->UpdateStateWeapon_OnServer(MovementState);

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

					
				}
			}
		}
		
		else
		{
			/*UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::InitWeapon - Weapon not found in table -NULL"));
			UE_LOG(LogTemp, Warning, TEXT("WeaponSpawn"));*/

		}
	}
}

void AMy_TDS_GameProjectCharacter::TryReloadWeapon()
{
	if (HealthComponent && HealthComponent->GetIsAlive() && CurrentWeapon && !CurrentWeapon->WeaponReloading)
	{
		TryReloadWeapon_OnServer();
		
		
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
	UE_LOG(LogTemp, Warning, TEXT("WeaponOtdacha"));
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

void AMy_TDS_GameProjectCharacter::TrySwitchNextWeapon_OnServer_Implementation()
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

void AMy_TDS_GameProjectCharacter::TrySwitchPreviosWeapon_OnServer_Implementation()
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

void AMy_TDS_GameProjectCharacter::TryAbilityEnabled()
{
	if (AbilityEffect)//TODO Cool down
	{
		UTDS_StateEffect* NewEffect = NewObject<UTDS_StateEffect>(this, AbilityEffect);
		if (NewEffect)
		{
			NewEffect->InitObject(this, NAME_None);
		}
	}
}

void AMy_TDS_GameProjectCharacter::TryAbilityInvul()
{
	//UE_LOG(LogTemp, Warning, TEXT("INVULNNNNNN"));

	Invul = true;

	ResetInvul();


	//if (AbilityEffects)//TODO Cool down
	//{
	//	UTDS_StateEffect_ExecuteInvulnerability* NewEffect = NewObject<UTDS_StateEffect_ExecuteInvulnerability>(this, AbilityEffects);
	//	if (NewEffect)
	//	{
	//		NewEffect->InitObject(this);
	//	}
	//}
}

EPhysicalSurface AMy_TDS_GameProjectCharacter::GetSurfaceType()
{
	EPhysicalSurface Result = EPhysicalSurface::SurfaceType_Default;
	if (HealthComponent)
	{
		if (HealthComponent->GetCorrentShield() <= 0)
		{
			if (GetMesh())
			{
				UMaterialInterface* myMaterial = GetMesh()->GetMaterial(0);
				if (myMaterial)
				{
					Result = myMaterial->GetPhysicalMaterial()->SurfaceType;
				}
			}
		}
	}

	
	return Result;
}

TArray<UTDS_StateEffect*>  AMy_TDS_GameProjectCharacter::GetAllCurrentEffects()
{
	return Effects;
}

void AMy_TDS_GameProjectCharacter::RemoveEffect(UTDS_StateEffect* RemoveEffect)
{
	Effects.Remove(RemoveEffect);

	SwitchEffect(RemoveEffect, false);
	EffectRemove = RemoveEffect;
}

void AMy_TDS_GameProjectCharacter::AddEffect(UTDS_StateEffect* newEffect)
{
	Effects.Add(newEffect);

	SwitchEffect(newEffect, true);
	EffectAdd = newEffect;
}

bool AMy_TDS_GameProjectCharacter::GetIsAlive()
{
	bool result = false;
	if (HealthComponent)
	{
		result = HealthComponent->GetIsAlive();
	}
	return result;
}

EMovementState AMy_TDS_GameProjectCharacter::GetMovementState()
{
	return MovementState;
}

void AMy_TDS_GameProjectCharacter::PlayAnim_Multicast_Implementation(UAnimMontage* Anim)
{
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Anim);
		
	}
}


void AMy_TDS_GameProjectCharacter::CharDead()
{
	
	CharDead_BP();

	if (HasAuthority())
	{
		float TimeAnim = 0.0f;
		int32 rnd = FMath::RandHelper(DeadsAnim.Num());
		if (DeadsAnim.IsValidIndex(rnd) && DeadsAnim[rnd] && GetMesh() && GetMesh()->GetAnimInstance())
		{
			TimeAnim = DeadsAnim[rnd]->GetPlayLength();
			//GetMesh()->GetAnimInstance()->Montage_Play(DeadsAnim[rnd]);
			PlayAnim_Multicast(DeadsAnim[rnd]);
		}
		
		
		if (GetController())
		{
			if(APlayerController* CurentController = Cast<APlayerController>(GetController()))
			{ 
				UE_LOG(LogTemp, Warning, TEXT("VALID PPOSSESS"));
				CurentController->UnPossess();
			}
		}
		//UnPossessed();
		//Timer Ragdoll
		//AttackCharEvent(false);

		float DecraeseAnimTimer = FMath::FRandRange(0.2f, 1.0f);

		GetWorldTimerManager().SetTimer(TimerHandle_RagDollTimer, this, &AMy_TDS_GameProjectCharacter::EnableRagDoll_Multicast, TimeAnim, false);
		
		SetLifeSpan(20.0f);
		
		if (GetCurrentWeapon())
		{
			GetCurrentWeapon()->SetLifeSpan(20.0f);
		}
	}
	else
	{
		if (GetCursorToWorld())
		{
			GetCursorToWorld()->SetVisibility(false);
		}

		AttackCharEvent(false);
		
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
		GetCapsuleComponent()->SetGenerateOverlapEvents(false);
		SetLifeSpan(2.f);
	}


	
}

void AMy_TDS_GameProjectCharacter::EnableRagDoll_Multicast_Implementation()
{
	if (GetMesh())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);

		GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
		GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
		SetCanBeDamaged(false);
	}
}

float AMy_TDS_GameProjectCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Damage Character

	if (HealthComponent && (Invul || !GetIsAlive()))
	{
		return 0.f;
	}
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (HealthComponent)
	{	
		//UE_LOG(LogTemp, Warning, TEXT("Damage"));
			HealthComponent->ChangeHealthValue_OnServer(ActualDamage);
	
	}

	if(DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		AProjectileDefault* myProjectile = Cast<AProjectileDefault>(DamageCauser);
		if (myProjectile)
		{
			
			UTypes::AddEffectBySurfaceType(this, NAME_None, myProjectile->ProjectileSetting.Effect, GetSurfaceType()); //to do NAME_None - bone radialDamage
			
		}
	}

	return ActualDamage;
	
}

void AMy_TDS_GameProjectCharacter::SetActorRotationByYaw_OnServer_Implementation(float Yaw)
{
	SetActorRotationByYaw_Multicast(Yaw);
}

void AMy_TDS_GameProjectCharacter::SetActorRotationByYaw_Multicast_Implementation(float Yaw)
{
	if (Controller && !Controller->IsLocalPlayerController())
	{
		SetActorRotation(FQuat(FRotator(0.0f, Yaw, 0.0f)));  
	}
}



void AMy_TDS_GameProjectCharacter::SetMovementState_OnServer_Implementation(EMovementState NewState)
{
	SetMovementState_Multicast(NewState);
}

void AMy_TDS_GameProjectCharacter::SetMovementState_Multicast_Implementation(EMovementState NewState)
{
	MovementState = NewState;
	CharacterUpdate();
}

void AMy_TDS_GameProjectCharacter::TryReloadWeapon_OnServer_Implementation()
{
	if (CurrentWeapon->GetWeaponRound() < CurrentWeapon->WeaponSetting.MaxRound && CurrentWeapon->CheckCanWeaponReload())
		CurrentWeapon->InitReload();

}


void AMy_TDS_GameProjectCharacter::CharDead_BP_Implementation()
{
	//BP
}

void AMy_TDS_GameProjectCharacter::EffectAdd_OnRep()
{
	if (EffectAdd)
	{
		SwitchEffect(EffectAdd, false);
	}
}

void AMy_TDS_GameProjectCharacter::EffectRemove_OnRep()
{
	if (EffectRemove)
	{
		SwitchEffect(EffectRemove, false);
	}
}

void AMy_TDS_GameProjectCharacter::SwitchEffect(UTDS_StateEffect* Effect, bool bIsAdd)
{
	if (bIsAdd)
	{
		if (Effect && Effect->ParticleEffect)
		{
			FName NameBoneToAttached = Effect->NameBone;
			FVector Loc = FVector(0);

			USkeletalMeshComponent* myMesh = GetMesh();
			if (myMesh)
			{
				UParticleSystemComponent* newParticleSystem = UGameplayStatics::SpawnEmitterAttached(Effect->ParticleEffect, myMesh, NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
				ParticleSystemEffects.Add(newParticleSystem);
			}
		}
	}
	else
	{
		if (Effect && Effect->ParticleEffect)
		{
			int32 i = 0;
			bool bIsFind = false;
			if (ParticleSystemEffects.Num() > 0)
			{
				while (i < ParticleSystemEffects.Num() && !bIsFind)
				{
					if (ParticleSystemEffects[i] && ParticleSystemEffects[i]->Template && Effect->ParticleEffect && Effect->ParticleEffect == ParticleSystemEffects[i]->Template)
					{
						bIsFind = true;
						ParticleSystemEffects[i]->DeactivateSystem();
						ParticleSystemEffects[i]->DestroyComponent();
						ParticleSystemEffects.RemoveAt(i);
					}
					i++;
				}
			}
		}
	}
}

bool AMy_TDS_GameProjectCharacter::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool Wrote = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (int32 i = 0; i < Effects.Num(); i++)
	{
		if (Effects[i])
		{
			Wrote |= Channel->ReplicateSubobject(Effects[i], *Bunch, *RepFlags);
		}
	}
	return Wrote;
}

void AMy_TDS_GameProjectCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	
	DOREPLIFETIME(AMy_TDS_GameProjectCharacter, MovementState);
	DOREPLIFETIME(AMy_TDS_GameProjectCharacter, CurrentWeapon);
	DOREPLIFETIME(AMy_TDS_GameProjectCharacter, Effects);
	DOREPLIFETIME(AMy_TDS_GameProjectCharacter, EffectAdd);
	DOREPLIFETIME(AMy_TDS_GameProjectCharacter, EffectRemove);
}