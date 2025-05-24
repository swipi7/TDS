// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "My_TDS_GameProjectPlayerController.generated.h"

UCLASS()
class AMy_TDS_GameProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMy_TDS_GameProjectPlayerController();

	//UFUNCTION(BlueprintCallable)
		/*virtual void Pause() override;*/

	//UFUNCTION(reliable, client)
	//	void ClientSetPauser(APlayerState* Pauser);
	

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		bool IsGamePaused() const;

private:
	//UFUNCTION(reliable, server, WithValidation)
	//	void ServerPauseGame();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface
	virtual void UpdateRotation(float DeltaTime) override;
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	//UFUNCTION(BlueprintCallable)
	//void BP_Pause(bool flag);

	/*UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void Pausesis_Multicast(bool flag);*/

	//virtual void OnUnPossess() override;


};


