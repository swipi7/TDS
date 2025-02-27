// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TDS_GameStateBase.h"
#include "kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"



void ATDS_GameStateBase::OnRep_IsPaused()
{
    UE_LOG(LogTemp, Error, TEXT("OnRep_IsPaused: %d"), bIsPaused);
    if (bIsPaused)
    {
        UGameplayStatics::SetGamePaused(GetWorld(), true);
    }
    else
    {
        UGameplayStatics::SetGamePaused(GetWorld(), false);
    }
}

void ATDS_GameStateBase::SetPauseState(bool bPaused)
{
    if (HasAuthority())
    {
        UE_LOG(LogTemp, Error, TEXT("SetPauseState: %d"), bPaused);
        bIsPaused = bPaused;
        OnRep_IsPaused();
    }
}

void ATDS_GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    UE_LOG(LogTemp, Error, TEXT("GetLifetimeReplicatedPropscalled"));
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ATDS_GameStateBase, bIsPaused);
}