// Fill out your copyright notice in the Description page of Project Settings.


//#include "Character/TDS_HealthComponent.h"
#include "TDS_HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/ActorComponent.h"


// Sets default values for this component's properties
UTDS_HealthComponent::UTDS_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UTDS_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	Shield = MaxShield;
	// ...
	
}


// Called every frame
void UTDS_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UTDS_HealthComponent::GetCurrentHealth()
{
	return Health;
}

void UTDS_HealthComponent::SetCurrentHealth(float NewHealth)
{
	Health = NewHealth;
}

bool UTDS_HealthComponent::GetIsAlive()
{
	return bIsAlive;
}

void UTDS_HealthComponent::ChangeHealthValue_OnServer_Implementation(float ChangeValue)
{
	
	if (bIsAlive)
	{
		if (bHasShield)
		{
			if (ChangeValue < 0)
			{
				ChangeValue = ChangeValue * CoefDamage;
				float cShield = Shield + ChangeValue;

				
				if (cShield <= 0)
				{
					Health = Health + (ChangeValue + Shield);
					Shield = 0;
					//UE_LOG(LogTemp, Warning, TEXT("Health"));
					//OnHealthChange.Broadcast(Health, ChangeValue);
					HealthChangeEvent_Multicast(Health, ChangeValue);
					//OnShieldChange.Broadcast(Shield, ChangeValue);
					ShieldChangeEvent_Multicast(Shield, ChangeValue);
					if (Health <= 0)
					{
						Health = 0;
						OnDead.Broadcast();
					}
				}
				else
				{
					Shield = cShield;
					
					//OnShieldChange.Broadcast(Shield, ChangeValue);
					ShieldChangeEvent_Multicast(Shield, ChangeValue);
					CoolDownShieldEnd();
					//UE_LOG(LogTemp, Warning, TEXT("Shield"));
				}
				
			}	
			
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("NOSHIELD!!!"))
		}
		

	}
	
}

void UTDS_HealthComponent::DeadEvent_Multicast_Implementation()
{
	OnDead.Broadcast();
}

void UTDS_HealthComponent::HealthChangeEvent_Multicast_Implementation(float newHealth, float value)
{
	OnHealthChange.Broadcast(newHealth, value);
}

//void UTDS_HealthComponent::ChangeHealthValue_OnServer(float ChangeValue)
//{
//
//	float CurrentDamage = ChangeValue * CoefDamage;
//	//Health += ChangeValue;
//
//	if (Shield > 0.0f && ChangeValue < 0.0f)
//	{
//		ChangeShieldValue(ChangeValue);
//
//		if (Shield < 0.0f)
//		{
//			//fX
//		}
//	}
//	else
//	{
		//Super::ChangeHealthValue
//	}

//}

float UTDS_HealthComponent::GetCorrentShield()
{
	return Shield;
}

void UTDS_HealthComponent::ChangeShieldValue(float ChangeValue)
{
	Shield += ChangeValue;

	//OnShieldChange.Broadcast(Shield, ChangeValue);
	ShieldChangeEvent_Multicast(Shield, ChangeValue);

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

	//if (GetWorld())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("TimerShieldColdown"))
	//	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CollDownShieldTimer, this, &UTDS_HealthComponent::CoolDownShieldEnd, CoolDownShieldRecoveryTime, false);
	//	UE_LOG(LogTemp, Warning, TEXT("TimerShieldColdown"))
	//	UE_LOG(LogTemp, Warning, TEXT("TimerShieldClear"))
	//	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
	//}

}

void UTDS_HealthComponent::CoolDownShieldEnd()
{
	if (GetWorld())
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_CollDownShieldTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_CollDownShieldTimer, this, &UTDS_HealthComponent::RecoveryShield, ShieldRecoverRate, true);

		}
		//UE_LOG(LogTemp, Warning, TEXT("TimerShieldEnd"))
	}
}

void UTDS_HealthComponent::RecoveryShield()
{
	float tmp = Shield;
	tmp = tmp + ShieldRecoverValue;

	if (tmp >= 100.0f)
	{
		Shield = 100.0f;
		if (GetWorld())
		{
			//UE_LOG(LogTemp, Warning, TEXT("TimerShieldRecovery STOP"))
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CollDownShieldTimer);
		
		}
	}
	else
	{
		Shield = tmp;
	}
	//OnShieldChange.Broadcast(Shield, ShieldRecoverValue);
	ShieldChangeEvent_Multicast(Shield, ShieldRecoverValue);
}

float UTDS_HealthComponent::GetShieldValue()
{
	return Shield;
}

void UTDS_HealthComponent::ShieldChangeEvent_Multicast_Implementation(float newShield, float Damage)
{
	OnShieldChange.Broadcast(newShield, Damage);
}

void UTDS_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(UTDS_HealthComponent, Health);
	DOREPLIFETIME(UTDS_HealthComponent, Shield);
	DOREPLIFETIME(UTDS_HealthComponent, bIsAlive);
	DOREPLIFETIME(UTDS_HealthComponent, bHasShield);
}