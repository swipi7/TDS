// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "My_TDS_GameProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMy_TDS_GameProjectGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMy_TDS_GameProjectGameMode();

	void PlayCharacterDead();
};



