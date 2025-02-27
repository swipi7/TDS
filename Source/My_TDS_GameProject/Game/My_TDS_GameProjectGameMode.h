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

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

	UPROPERTY(Replicated)
	bool bIsGamePaused;

	//UFUNCTION(BlueprintCallable)
	//void OnVictory();

	void OnBossDefeated();

	UFUNCTION(BlueprintCallable, Category = "Map Transition")
		void TravelToNewMap();

	//void SetPauseState(bool bPaused);

	//virtual bool ClearPause() override;

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//UFUNCTION()
	//void OnRep_GamePaused();

private:
	void CallAllClientsSetWorldSettingsPauser(APlayerState* Pauser);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_NotifyClientsAboutTravel();

	// Загрузка новой карты
	UFUNCTION(BlueprintCallable)
	void LoadNewMap();


};



