// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TDS_MyGameState.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Character/My_TDS_GameProjectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UObject/UnrealType.h"




void ATDS_MyGameState::OnRep_IsGamePaused()
{
    UWorld* World = GetWorld();
    UE_LOG(LogTemp, Warning, TEXT("UpdateRotation: Paused = %d"));
    if (!World) return;
    //if (!World || World->IsNetMode(NM_DedicatedServer)) return;
    
       //  Блокируем ввод для всех локальных игроков
       for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
       {
         APlayerController* PC = It->Get();
           if (PC && PC->IsLocalController())
           {
               PC->SetIgnoreMoveInput(bIsGamePaused);
               PC->SetIgnoreLookInput(bIsGamePaused);
               if (bIsGamePaused) PC->ConsoleCommand("pause"); // Альтернатива

          }
      }
      


    
    UGameplayStatics::SetGlobalTimeDilation(World, bIsGamePaused ? 0.0f : 1.0f);
}

void ATDS_MyGameState::SetPauseState(bool bPaused)
{
    if (HasAuthority()) 
    {
        bIsGamePaused = bPaused;
        OnRep_IsGamePaused(); 


        UGameplayStatics::SetGamePaused(GetWorld(), bPaused);
       
        if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
        {
            ForceNetUpdate(); 
        }
    }
}

void ATDS_MyGameState::PauseGameAfterBossDeath(const UObject* WorldContextObject, float Delay)
{
    if (!WorldContextObject) return;

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World) return;

    ATDS_MyGameState* GameState = World->GetGameState<ATDS_MyGameState>();
    if (!GameState) return;

    if (Delay <= 0.0f)
    {
        GameState->SetPauseState(true);
    }
    else
    {
        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimerForNextTick(/*TimerHandle */  [GameState]()
            {
                GameState->SetPauseState(true);
            });
    }
}

void ATDS_MyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATDS_MyGameState, bIsGamePaused);
}