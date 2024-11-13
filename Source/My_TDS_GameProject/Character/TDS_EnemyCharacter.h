// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TDS_IGameActor.h"


#include "TDS_EnemyCharacter.generated.h"




UCLASS()
class MY_TDS_GAMEPROJECT_API ATDS_EnemyCharacter : public ACharacter, public ITDS_IGameActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATDS_EnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlag) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void RemoveEffect(UTDS_StateEffect* RemoveEffect);
	void RemoveEffect_Implementation(UTDS_StateEffect* RemoveEffect);//override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void AddEffect(UTDS_StateEffect* newEffect);
	void AddEffect_Implementation(UTDS_StateEffect* newEffect);//override;

	//Effect
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Setting")
		TArray<UTDS_StateEffect*> Effects;


	UPROPERTY(ReplicatedUsing = EffectAdd_OnRep)
		UTDS_StateEffect* EffectAdd = nullptr;

	UPROPERTY(ReplicatedUsing = EffectRemove_OnRep)
		UTDS_StateEffect* EffectRemove = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dbag")
		TArray<UParticleSystemComponent*> ParticleSystemEffects;
	

	UFUNCTION()
		void EffectAdd_OnRep();
	UFUNCTION()
		void EffectRemove_OnRep();

	UFUNCTION(Server, Reliable)
		void ExecuteEffectAdded_OnServer(UParticleSystem* ExecuteFX);

	UFUNCTION(NetMulticast, Reliable)
		void ExecuteEffectAdded_Multicast(UParticleSystem* ExecuteFX);

	UFUNCTION()
		void SwitchEffect(UTDS_StateEffect* Effect, bool bIsAdd);

};
