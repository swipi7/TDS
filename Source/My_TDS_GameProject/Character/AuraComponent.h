// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AuraComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_TDS_GAMEPROJECT_API UAuraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAuraComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY()
    class USphereComponent* AuraSphere;

    // Таймер
    FTimerHandle DamageTimerHandle;

    // Настройки урона
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aura")
    float DamagePerTick = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aura")
    float TickInterval = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aura")
    float AuraRadius = 300.0f;

    void DealDamage();


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
