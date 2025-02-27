// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TDS_GameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class MY_TDS_GAMEPROJECT_API ATDS_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
    UPROPERTY(ReplicatedUsing = OnRep_IsPaused)
        bool bIsPaused;

    UFUNCTION()
        void OnRep_IsPaused();

    void SetPauseState(bool bPaused);

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
