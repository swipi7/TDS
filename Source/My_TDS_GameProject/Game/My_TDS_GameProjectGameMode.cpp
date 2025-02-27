// Copyright Epic Games, Inc. All Rights Reserved.

#include "My_TDS_GameProjectGameMode.h"
#include "My_TDS_GameProjectPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "kismet/GameplayStatics.h"
#include "TDS_MyGameState.h"
#include "Net/UnrealNetwork.h"

AMy_TDS_GameProjectGameMode::AMy_TDS_GameProjectGameMode()
{
	//// use our custom PlayerController class
	//PlayerControllerClass = AMy_TDS_GameProjectPlayerController::StaticClass();

	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/TopDownCharacter"));
	//if (PlayerPawnBPClass.Class != nullptr)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
	/*bReplicates = true;*/
}

void AMy_TDS_GameProjectGameMode::PlayCharacterDead()
{

}

bool AMy_TDS_GameProjectGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate /*= FCanUnpause()*/)
{
	if (Super::SetPause(PC, CanUnpauseDelegate))
	{
		//hack because replication during pause is not working correctly.
		//CallAllClientsSetWorldSettingsPauser(PC->PlayerState);
		return true;
	}

	return false;
}

//void AMy_TDS_GameProjectGameMode::OnVictory()
//{
//	if (HasAuthority())
//	{
//		ATDS_GameStateBase* GameState1 = GetGameState<ATDS_GameStateBase>();
//		if (GameState1)
//		{
//			GameState1->SetPauseState(true);
//		}
//	}
//}

void AMy_TDS_GameProjectGameMode::OnBossDefeated()
{
	ATDS_MyGameState* MyGAME = Cast<ATDS_MyGameState>(GetWorld()->GetGameState()); // Используем GameState из AGameMode
	if (MyGAME && GetLocalRole() == ROLE_Authority) // Только сервер может вызывать паузу
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss defeated! Pausing game..."));
		MyGAME->PauseGameVIN(); // Ставим игру на паузу
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameState is null or not authoritative!"));
	}
}


void AMy_TDS_GameProjectGameMode::TravelToNewMap()
{
	if (GetWorld()->GetAuthGameMode() == this) // Проверка, что код выполняется на сервере
	{

		Multicast_NotifyClientsAboutTravel();
	}
}

void AMy_TDS_GameProjectGameMode::Multicast_NotifyClientsAboutTravel_Implementation()
{
	if (GetWorld()->GetNetMode() == NM_DedicatedServer) return;
}

void AMy_TDS_GameProjectGameMode::LoadNewMap()
{
	if (GetWorld()->GetAuthGameMode() == this)
	{
		// Загружаем новую карту
		FString MapName = "TopDownExampleMap3"; // Имя новой карты
		GetWorld()->ServerTravel(MapName);
	}
}

//void AMy_TDS_GameProjectGameMode::SetPauseState(bool bPaused)
//{
//	if (HasAuthority())
//	{
//		UE_LOG(LogTemp, Error, TEXT("SetPauseState: %d"), bPaused);
//		bIsGamePaused = bPaused;
//		OnRep_GamePaused(); // Вызов для сервера
//	}
//}
//
//bool AMy_TDS_GameProjectGameMode::ClearPause()
//{
//	if (Super::ClearPause())
//	{
//		//hack because replication during pause is not working correctly.
//		CallAllClientsSetWorldSettingsPauser(nullptr);
//		return true;
//	}
//
//	return false;
//}
//
//

//void AMy_TDS_GameProjectGameMode::OnRep_GamePaused()
//{
//	UE_LOG(LogTemp, Error, TEXT("OnRep_GamePaused: %d"), bIsGamePaused);
//	if (bIsGamePaused)
//	{
//		UGameplayStatics::SetGamePaused(GetWorld(), true);
//	}
//	else
//	{
//		UGameplayStatics::SetGamePaused(GetWorld(), false);
//	}
//
//	/*if (bIsGamePaused)
//	{
//		
//		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
//		{
//			APlayerController* PlayerController = It->Get();
//			if (PlayerController)
//			{
//				PlayerController->SetPause(true);
//			}
//		}
//	}*/
//}
//
////hack because replication during pause is not working correctly.
//void AMy_TDS_GameProjectGameMode::CallAllClientsSetWorldSettingsPauser(APlayerState* Pauser)
//{
//	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
//	{
//		AMy_TDS_GameProjectPlayerController* PlayerController = Cast<AMy_TDS_GameProjectPlayerController>(*Iterator);
//		PlayerController->ClientSetPauser(Pauser);
//	}
//}
//
//void AMy_TDS_GameProjectGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	UE_LOG(LogTemp, Error, TEXT("GetLifetimeReplicatedPropscalled"));
//	DOREPLIFETIME(AMy_TDS_GameProjectGameMode, bIsGamePaused);
//}