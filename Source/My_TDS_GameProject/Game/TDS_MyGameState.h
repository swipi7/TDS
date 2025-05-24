// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TDS_MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class MY_TDS_GAMEPROJECT_API ATDS_MyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
    UPROPERTY(ReplicatedUsing = OnRep_IsGamePaused, BlueprintReadOnly, Category = "Gameplay")
        bool bIsGamePaused = false;

    // Функция для паузы (вызывается на сервере, реплицируется на клиенты)
    UFUNCTION(BlueprintCallable, Category = "Gameplay", meta = (WorldContext = "WorldContextObject"))
        static void PauseGameAfterBossDeath(const UObject* WorldContextObject, float Delay = 0.0f);

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void SetPauseState(bool bPaused);

protected:
    // Репликация свойства
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    // Реплицируемая функция для обновления паузы на клиентах
    UFUNCTION()
        void OnRep_IsGamePaused();

    // Таймер для задержки паузы
    FTimerHandle PauseTimerHandle; 
};
