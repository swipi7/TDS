// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/TDS_StateEffect.h"
#include "Character/TDS_HealthComponent.h"
#include "TDS_IGameActor.h"
#include "Character/My_TDS_GameProjectCharacter.h"

#include "Kismet/GameplayStatics.h"

bool UTDS_StateEffect::InitObject(AActor* Actor)
{
	myActor = Actor;

	ITDS_IGameActor* myInterface = Cast<ITDS_IGameActor>(myActor);
	if (myInterface)
	{
		myInterface->AddEffect(this);
	}


	return  true;
}

void UTDS_StateEffect::DestroyObject()
{

	ITDS_IGameActor* myInterface = Cast<ITDS_IGameActor>(myActor);
	if (myInterface)
	{
		myInterface->RemoveEffect(this);
	}

	myActor = nullptr;
	if (this && this->IsValidLowLevel())
	{
		this->ConditionalBeginDestroy();
	}
}

bool UTDS_StateEffect_ExecuteOnce::InitObject(AActor* Actor)
{
	Super::InitObject(Actor);
	ExecuteOnce();
	return true;
}

void UTDS_StateEffect_ExecuteOnce::DestroyObject()
{
	Super::DestroyObject();
}

void UTDS_StateEffect_ExecuteOnce::ExecuteOnce()
{
	if (myActor)
	{
		UTDS_HealthComponent* myHeathComp = Cast<UTDS_HealthComponent>(myActor->GetComponentByClass(UTDS_HealthComponent::StaticClass()));
		if (myHeathComp)
		{
			myHeathComp->ChangeHealthValue(Power);
		}
	}

	DestroyObject();
}

bool UTDS_StateEffect_ExecuteTimer::InitObject(AActor* Actor)
{
	Super::InitObject(Actor);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimer, this, &UTDS_StateEffect_ExecuteTimer::DestroyObject, Timer, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimer, this, &UTDS_StateEffect_ExecuteTimer::Execute, RateTime, true);

	if (ParticleEffect)
	{
		FName NameBoneToAttached;
		FVector Loc = FVector(0);

		ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, myActor->GetRootComponent(), NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
	}

	return true;
}

void UTDS_StateEffect_ExecuteTimer::DestroyObject()
{
	ParticleEmitter->DestroyComponent();
	ParticleEmitter = nullptr;
	Super::DestroyObject();
}

void UTDS_StateEffect_ExecuteTimer::Execute()
{
	if (myActor)
	{
		//UGameplayStatics::ApplyDamage(myActor,Power,nullptr,nullptr,nullptr);	
		UTDS_HealthComponent* myHeathComp = Cast<UTDS_HealthComponent>(myActor->GetComponentByClass(UTDS_HealthComponent::StaticClass()));
		if (myHeathComp)
		{
			myHeathComp->ChangeHealthValue(Power);
		}
	}
}

bool UTDS_StateEffect_ExecuteInvulnerability::InitObject(AActor* Actor)
{
	Super::InitObject(Actor);
	return true;
}

void UTDS_StateEffect_ExecuteInvulnerability::DestroyObject()
{
	ExecuteInvul();
	Super::DestroyObject();
}

void UTDS_StateEffect_ExecuteInvulnerability::ExecuteInvul()
{
	if (myActor)
	{
		AMy_TDS_GameProjectCharacter* myNoDamage = Cast <AMy_TDS_GameProjectCharacter>(myActor->GetComponentByClass(AMy_TDS_GameProjectCharacter::StaticClass()));
		if (myNoDamage)
		{
			UE_LOG(LogTemp, Warning, TEXT("INVULNERABILITy"));
			myNoDamage->TryAbilityInvul();
			
					
		}
	
	}
}

