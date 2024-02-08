// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TDS_HealthComponent.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_TDS_GAMEPROJECT_API UTDS_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTDS_HealthComponent();

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
		FOnHealthChange OnHealthChange;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
		FOnDead OnDead;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	float Health = 100.0f;

	UPROPERTY(Replicated)
		bool bIsAlive = true;

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
		virtual void ChangeHealthValue_OnServer(float ChangeValue);
	
	UFUNCTION(NetMulticast, Reliable)
		void HealthChangeEvent_Multicast(float newHealth, float value);


	UFUNCTION(NetMulticast, Reliable)
		void DeadEvent_Multicast();


};
