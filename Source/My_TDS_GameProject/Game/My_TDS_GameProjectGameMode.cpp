// Copyright Epic Games, Inc. All Rights Reserved.

#include "My_TDS_GameProjectGameMode.h"
#include "My_TDS_GameProjectPlayerController.h"
//#include "My_TDS_GameProject/Character/My_TDS_GameProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

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
}

void AMy_TDS_GameProjectGameMode::PlayCharacterDead()
{
}
