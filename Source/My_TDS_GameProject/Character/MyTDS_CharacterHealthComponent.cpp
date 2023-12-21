// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyTDS_CharacterHealthComponent.h"

void UMyTDS_CharacterHealthComponent::ChangeHealthValue(float ChangeValue)
{

	float CurrentDamage = ChangeValue * CoefDamage;
	//Health += ChangeValue;

	if (Shield > 0.0f && ChangeValue <0.0f)
	{
		ChangeShieldValue(ChangeValue);
		
		if (Shield < 0.0f)
		{
			//fX
		}
	}
	else
	{
		Super::ChangeHealthValue(ChangeValue);
	}
	


	
	//for char
}

float UMyTDS_CharacterHealthComponent::GetCorrentShield()
{
	return Shield;
}

void UMyTDS_CharacterHealthComponent::ChangeShieldValue(float ChangeValue)
{
	Shield += ChangeValue;
	
	OnShieldChange.Broadcast(Shield, ChangeValue);

	if (Shield > 100.0f)
	{
		Shield = 100.0f;

	}
	else
	{
		if (Shield < 0.0f)
		{
			Shield = 0.0f;
		}
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_CollDownShieldTimer, this, &UMyTDS_CharacterHealthComponent::CoolDownShieldEnd, CoolDownShieldRecoveryTime, false);

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
	}
	
}

void UMyTDS_CharacterHealthComponent::CoolDownShieldEnd()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShieldRecoveryRateTimer, this, &UMyTDS_CharacterHealthComponent::RecoveryShield, ShieldRecoverRate, true);
	}
}

void UMyTDS_CharacterHealthComponent::RecoveryShield()
{
	float tmp = Shield;
	tmp = tmp + ShieldRecoverValue;

	if (tmp > 100.0f)
	{
		Shield = 100.0f;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
		}
	}
	else
	{
		Shield = tmp;
	}
	OnShieldChange.Broadcast(Shield, ShieldRecoverValue);
}

float UMyTDS_CharacterHealthComponent::GetShieldValue()
{
	return Shield;
}
