// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TDS_EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/ActorChannel.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATDS_EnemyCharacter::ATDS_EnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATDS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	
}

// Called every frame
void ATDS_EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATDS_EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATDS_EnemyCharacter::RemoveEffect_Implementation(UTDS_StateEffect* RemoveEffect)
{
	Effects.Remove(RemoveEffect);

	if (!RemoveEffect->bIsAutoDestroyParticleEffect)
	{
		SwitchEffect(RemoveEffect, false);
		EffectRemove = RemoveEffect;
	}
}



void ATDS_EnemyCharacter::AddEffect_Implementation(UTDS_StateEffect* newEffect)
{
	Effects.Add(newEffect);

	if (!newEffect->bIsAutoDestroyParticleEffect)
	{
		SwitchEffect(newEffect, true);
		EffectAdd = newEffect;
	}
	else
	{
		if (newEffect->ParticleEffect)
		{
			ExecuteEffectAdded_OnServer(newEffect->ParticleEffect);
		}
	}
}

void ATDS_EnemyCharacter::EffectAdd_OnRep()
{
	if (EffectAdd)
	{
		SwitchEffect(EffectAdd, true);
	}
}

void ATDS_EnemyCharacter::EffectRemove_OnRep()
{
	if (EffectRemove)
	{
		SwitchEffect(EffectRemove, false);
	}
}

void ATDS_EnemyCharacter::ExecuteEffectAdded_OnServer_Implementation(UParticleSystem* ExecuteFX)
{
	ExecuteEffectAdded_Multicast(ExecuteFX);
}

void ATDS_EnemyCharacter::ExecuteEffectAdded_Multicast_Implementation(UParticleSystem* ExecuteFX)
{
	UTypes::ExecuteEffectAdded(ExecuteFX, this, FVector(0), FName("spine_01"));
}

void ATDS_EnemyCharacter::SwitchEffect(UTDS_StateEffect* Effect, bool bIsAdd)
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

bool ATDS_EnemyCharacter::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
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

void ATDS_EnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	
	DOREPLIFETIME(ATDS_EnemyCharacter, Effects);
	DOREPLIFETIME(ATDS_EnemyCharacter, EffectAdd);
	DOREPLIFETIME(ATDS_EnemyCharacter, EffectRemove);
}