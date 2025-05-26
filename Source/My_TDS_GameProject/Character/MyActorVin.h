// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyActorVin.generated.h"

UCLASS()
class MY_TDS_GAMEPROJECT_API AMyActorVin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActorVin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = "Game")
	//	void OnVictory();

private:
	/*void DisablePlayerMovement();
	void DisableEnemies();
	void DisableWeapons();

	UPROPERTY(ReplicatedUsing = OnRep_Victory)
		bool bIsVictory;

	UFUNCTION()
		void OnRep_Victory();*/
};
//Вресенно нужно!