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
    ATDS_MyGameState();

    // Функция для паузы игры
    UFUNCTION(BlueprintCallable, Category = "GameState", Server, Reliable)
        void PauseGameVIN();

    

    // Функция для возобновления игры
    UFUNCTION(BlueprintCallable, Category = "GameState")
        void ResumeGameVin();

    // Получить состояние паузы
    UFUNCTION(BlueprintPure, Category = "GameState")
        bool IsGamePausedVIN() const;

protected:
    // Синхронизированная переменная для паузы
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
        bool bIsGamePaused;

    // Репликация переменной
  /*  UFUNCTION()
        void OnRep_GamePaused();*/

    // Инициализация репликации
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:
    // Внутренняя функция для обработки паузы
    void HandlePause(bool bPaused);

    UFUNCTION(NetMulticast, Reliable)
        void Multicast_PauseGame(bool bPaused);
};
