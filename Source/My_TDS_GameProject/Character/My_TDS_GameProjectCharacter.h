// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "My_TDS_GameProject/FuncLibrary/Types.h"
#include "My_TDS_GameProject/Weapon/WeaponDefaults.h"
#include "My_TDS_GameProject/TDS_GameInstance.h"
#include "My_TDS_GameProject/TDS_Inventar.h"
#include "My_TDS_GameProject/Character/TDS_HealthComponent.h"
#include "TDS_IGameActor.h"
#include "Weapon/TDS_StateEffect.h"

#include "My_TDS_GameProjectCharacter.generated.h"

UCLASS(Blueprintable)
class AMy_TDS_GameProjectCharacter : public ACharacter, public ITDS_IGameActor
{
	GENERATED_BODY()

protected:
	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlag) override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "true")
		UTDS_HealthComponent* HealthComponent;

	template<int32 Id>
	void TKeyPressed()
	{
		TrySwitchWeaponToIndexByKeyInput(Id);
	}

public:
	AMy_TDS_GameProjectCharacter();

	FTimerHandle TimerHandle_RagDollTimer;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	//FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UTDS_Inventar* InventoryComponent;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UMyTDS_CharacterHealthComponent* HealthComponent;*/

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;*/

	bool bSprintForward();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
		UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
		FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(Replicated)
		EMovementState MovementState = EMovementState::Run_State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FCharacterSpeed MovementInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool SprintRunEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool WalkEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool AimEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bIsMovingForward = false;

	bool TrySwitchWeaponToIndexByKeyInput(int32 ToIndex);

	void DropCurrentWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		TArray<UAnimMontage*> DeadsAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TSubclassOf<UTDS_StateEffect> AbilityEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TSubclassOf<UTDS_StateEffect_ExecuteInvulnerability> AbilityEffects;

	bool CharSprint();

	bool TrueVector = false;

	bool Invul = false;

	
	//Timer Invul
	FTimerHandle TimerInvul;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InvulTime")
		float InvulTim = 3.0f;

	void ResetInvul();
	void ChangeInvul();
	//Weapon	
	UPROPERTY(Replicated)
	AWeaponDefaults* CurrentWeapon = nullptr;


	//Effects
	UPROPERTY(Replicated)
	TArray<UTDS_StateEffect*> Effects;
	UPROPERTY(ReplicatedUsing = EffectAdd_OnRep)
		UTDS_StateEffect* EffectAdd = nullptr;

	UPROPERTY(ReplicatedUsing = EffectRemove_OnRep)
		UTDS_StateEffect* EffectRemove = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dbag")
		TArray<UParticleSystemComponent*> ParticleSystemEffects;


	////for demo 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
	FName InitWeaponName;

	UDecalComponent* CurrentCursor = nullptr;

	

	UFUNCTION()
		void InputAxisX(float Value);
	UFUNCTION()
		void InputAxisY(float Value);
	UFUNCTION()
		void InputAttackPressed();
	UFUNCTION()
		void InputAttackReleased();

		void InputWalkPressed();
		void InputWalkReleased();

		void InputSprintPressed();
		void InputSprintReleased();

		void InputAimPressed();
		void InputAimReleased();


	float AxisX = 0.0f;
	float AxisY = 0.0f;

	//Tick Func
	UFUNCTION()
		void MovementTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		void CharacterUpdate();
	UFUNCTION(BlueprintCallable)
		void ChangeMovementState();

	UFUNCTION(BlueprintCallable)
		FVector TreceVeloCity() const;
	UFUNCTION(BlueprintCallable)
		FVector TreceForward() const;

	// Tick Func


	////Func
	UFUNCTION(BlueprintCallable)
		void AttackCharEvent(bool bIsFiring);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
		AWeaponDefaults* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable)
		void InitWeapon(FName IdWeaponName, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon);
	UFUNCTION(BlueprintCallable)
		void TryReloadWeapon();
	UFUNCTION()
		void WeaponReloadStart(UAnimMontage* Anim);
	UFUNCTION()
		void WeaponReloadEnd(bool bIsSuccess, int32 AmmoTake);

	UFUNCTION()
		void WeaponFireStart(UAnimMontage* Anim);

	UFUNCTION(BlueprintNativeEvent)
		void WeaponFireStart_BP(UAnimMontage* Anim);
	
	UFUNCTION(BlueprintNativeEvent)
		void WeaponReloadStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
		void WeaponReloadEnd_BP(bool bIsSuccess);

	

	UFUNCTION(BlueprintCallable)
		UDecalComponent* GetCursorToWorld();

	UFUNCTION(Server, Reliable)
	void TrySwitchNextWeapon_OnServer();
	UFUNCTION(Server, Reliable)
	void TrySwitchPreviosWeapon_OnServer();

	//Ability
	void TryAbilityEnabled();
	//Invul
	void TryAbilityInvul();

	UPROPERTY(BlueprintReadOnly, Replicated)
		int32 CurrentIndexWeapon = 0;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		EMovementState GetMovementState();

	UFUNCTION(BlueprintCallable, BluePrintPure)
		bool GetIsAlive();

	//Intarface
	EPhysicalSurface GetSurfaceType() override;
	TArray<UTDS_StateEffect*> GetAllCurrentEffects() override;
	void RemoveEffect(UTDS_StateEffect* RemoveEffect)override;
	void AddEffect(UTDS_StateEffect* newEffect)override;

	UFUNCTION()
	void CharDead();
	UFUNCTION(NetMulticast, Reliable)
	void EnableRagDoll_Multicast();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent)
		void CharDead_BP();

	UFUNCTION(Server, Unreliable)
		void SetActorRotationByYaw_OnServer(float Yaw);
	UFUNCTION(NetMulticast, Unreliable)
		void SetActorRotationByYaw_Multicast(float Yaw);

	UFUNCTION(Server, Reliable)
		void SetMovementState_OnServer(EMovementState NewState);
	UFUNCTION(NetMulticast, Reliable)
		void SetMovementState_Multicast(EMovementState NewState);

	UFUNCTION(Server, Reliable)
		void TryReloadWeapon_OnServer();

	UFUNCTION(NetMulticast, Reliable)
		void PlayAnim_Multicast(UAnimMontage* Anim);

	UFUNCTION()
		void EffectAdd_OnRep();
	UFUNCTION()
		void EffectRemove_OnRep();

	UFUNCTION()
		void SwitchEffect(UTDS_StateEffect* Effect, bool bIsAdd);

};