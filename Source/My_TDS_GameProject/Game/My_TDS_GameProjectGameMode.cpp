// Copyright Epic Games, Inc. All Rights Reserved.

#include "My_TDS_GameProjectGameMode.h"
#include "My_TDS_GameProjectPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TDS_MyGameState.h"
#include "Net/UnrealNetwork.h"

AMy_TDS_GameProjectGameMode::AMy_TDS_GameProjectGameMode()
{

}

void AMy_TDS_GameProjectGameMode::PlayCharacterDead()
{

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

