// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TDS_HealthComponent.h"
#include "MyTDS_CharacterHealthComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, Shield, float, Damage);


UCLASS()
class MY_TDS_GAMEPROJECT_API UMyTDS_CharacterHealthComponent : public UTDS_HealthComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Shield")
		FOnShieldChange OnShieldChange;

	FTimerHandle TimerHandle_CollDownShieldTimer;
	FTimerHandle TimerHandle_ShieldRecoveryRateTimer;

protected:
	float Shield = 100.0f;


public:
	
	//Shield 


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
		float CoolDownShieldRecoveryTime = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
		float ShieldRecoverValue = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
		float ShieldRecoverRate = 0.1f;

	
	void ChangeHealthValue_OnServer(float ChangeValue) override;

	float GetCorrentShield();

	void ChangeShieldValue(float ChangeValue);
	void CoolDownShieldEnd();
	void RecoveryShield();

	UFUNCTION(BlueprintCallable)
	float GetShieldValue();

	UFUNCTION(NetMulticast, Reliable)
		void ShieldChangeEvent_Multicast(float newShield, float Damage);

};
