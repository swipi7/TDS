// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TDS_HealthComponent.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, Shield, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_TDS_GAMEPROJECT_API UTDS_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTDS_HealthComponent();

	float GetHealth() const { return Health; }


	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
		FOnHealthChange OnHealthChange;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
		FOnDead OnDead;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Shield")
		FOnShieldChange OnShieldChange;

	FTimerHandle TimerHandle_CollDownShieldTimer;
	//FTimerHandle TimerHandle_ShieldRecoveryRateTimer;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float MaxShield = 100.0f;

	UPROPERTY(Replicated)
	bool bHasShield = true;

	UPROPERTY(Replicated)
		bool bIsAlive = true;
private:
	UPROPERTY(Replicated)
	float Shield = 0.0f;
	UPROPERTY(Replicated)
	float Health = 0.0f;

public:	

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float CoefDamage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invul")
		bool Invul = true;
	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetCurrentHealth();
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float NewHealth);
	
	UFUNCTION(BlueprintCallable, Category = "Health")
		bool GetIsAlive();


	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
		void ChangeHealthValue_OnServer(float ChangeValue);
	
	UFUNCTION(NetMulticast, Reliable)
		virtual void HealthChangeEvent_Multicast(float newHealth, float value);


	UFUNCTION(NetMulticast, Reliable)
		void DeadEvent_Multicast();



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
		float CoolDownShieldRecoveryTime = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
		float ShieldRecoverValue = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
		float ShieldRecoverRate = 0.1f;


	

	float GetCorrentShield();

	void ChangeShieldValue(float ChangeValue);
	void CoolDownShieldEnd();
	void RecoveryShield();

	UFUNCTION(BlueprintCallable)
		float GetShieldValue();

	UFUNCTION(NetMulticast, Reliable)
		void ShieldChangeEvent_Multicast(float newShield, float Damage);

};
