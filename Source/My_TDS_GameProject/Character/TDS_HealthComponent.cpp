// Fill out your copyright notice in the Description page of Project Settings.


//#include "Character/TDS_HealthComponent.h"
#include "TDS_HealthComponent.h"
#include "Net/UnrealNetwork.h"

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

void UTDS_HealthComponent::ChangeHealthValue_OnServer_Implementation(float ChangeValue)
{
	
	
	
		ChangeValue = ChangeValue * CoefDamage;

		Health += ChangeValue;

		//OnHealthChange.Broadcast(Health, ChangeValue);
		HealthChangeEvent_Multicast(Health, ChangeValue);

		if (Health > 100.0f)
		{
			Health = 100.0f;
		}
		else
		{
			if (Health < 0.0f)
			{
				//OnDead.Broadcast();
				DeadEvent_Multicast();
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

void UTDS_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(UTDS_HealthComponent, Health);
	
}

