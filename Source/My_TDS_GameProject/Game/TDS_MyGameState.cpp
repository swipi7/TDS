// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TDS_MyGameState.h"

#include "kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UObject/UnrealType.h"



ATDS_MyGameState::ATDS_MyGameState()
{
    bIsGamePaused = false;
    bReplicates = true;
}

void ATDS_MyGameState::PauseGameVIN_Implementation()
{

   // UE_LOG(LogTemp, Error, TEXT("PauseGame: %d")); // Только сервер может изменять состояние
    if (GetLocalRole() == ROLE_Authority)
    {
            bIsGamePaused = true;
            Multicast_PauseGame(true); // Вызов multicast функции
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PauseGame called on client!"));
    }
}

void ATDS_MyGameState::Multicast_PauseGame_Implementation(bool bPaused)
{
    FString RoleString = UEnum::GetValueAsString(GetLocalRole());//Временно!!!!
    
      UE_LOG(LogTemp, Error, TEXT("Multicast_PauseGame: bPaused = %d, Role = %s"), bPaused, *RoleString);//Временно!!!!
      bIsGamePaused = bPaused;
      HandlePause(bPaused);
    
}

void ATDS_MyGameState::ResumeGameVin()
{
    if (GetLocalRole() == ROLE_Authority) // Только сервер может изменять состояние
    {
        UE_LOG(LogTemp, Error, TEXT("PauseGame called on client!"));
        bIsGamePaused = false;
        Multicast_PauseGame(false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ResumeGame called on client!"));//Временно!!!!
    }
}

bool ATDS_MyGameState::IsGamePausedVIN() const
{
    return bIsGamePaused;
}

//void ATDS_MyGameState::OnRep_GamePaused()
//{
//    FString RoleString = UEnum::GetValueAsString(GetLocalRole()); //Временно!!!!
//    UE_LOG(LogTemp, Error, TEXT("OnRep_GamePaused: bIsGamePaused = %d, Role = %s"), bIsGamePaused, *RoleString);//Временно!!!!
//    HandlePause(bIsGamePaused);
//}

void ATDS_MyGameState::HandlePause(bool bPaused)
{
    FString RoleString = UEnum::GetValueAsString(GetLocalRole()); // Объявление переменной
    UE_LOG(LogTemp, Warning, TEXT("HandlePause: bPaused = %d, Role = %s"), bPaused, *RoleString);

    // Вызываем SetGamePaused только на сервере
    if (GetLocalRole() == ROLE_Authority)
    {
        UGameplayStatics::SetGamePaused(this, bPaused); // Пауза или возобновление игры
    }

    // На клиенте обрабатываем паузу через отключение ввода или другие механизмы
    if (GetLocalRole() == ROLE_SimulatedProxy || GetLocalRole() == ROLE_AutonomousProxy)
    {
        // Получаем PlayerController
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
        if (PlayerController)
        {
            if (bPaused)
            {
                // Отключаем ввод
                PlayerController->SetIgnoreMoveInput(true);
                PlayerController->SetIgnoreLookInput(true);
                UE_LOG(LogTemp, Warning, TEXT("Input disabled on client!"));
            }
            else
            {
                // Включаем ввод
                PlayerController->SetIgnoreMoveInput(false);
                PlayerController->SetIgnoreLookInput(false);
                UE_LOG(LogTemp, Warning, TEXT("Input enabled on client!"));
            }
        }
    }
    //else
    //{
    //    UGameplayStatics::SetGamePaused(this, false); // Возобновление игры
    //}
}

void ATDS_MyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
   // UE_LOG(LogTemp, Error, TEXT("PauseGame: %d"));
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    UE_LOG(LogTemp, Error, TEXT("PauseGameVIN: %d"));
    // Репликация переменной bIsGamePaused
    DOREPLIFETIME(ATDS_MyGameState, bIsGamePaused);
}