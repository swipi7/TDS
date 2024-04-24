// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FuncLibrary/Types.h"
#include "TDS_Inventar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSwitchWeapon, FName, WeaponIdName, FAdditionalWeaponInfo, WeaponAdditionalInfo, int32, NewCurrentIndexWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChange, EWeaponType, TypeAmmo, int32, Cout);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponAdditionalInfoChange, int32, IndexSlot, FAdditionalWeaponInfo, AdditionalInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAmmoEmpty, EWeaponType, WeaponType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAmmoAviable, EWeaponType, WeaponType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateWeaponSlots, int32, IndexSlotChange, FWeaponSlot, NewInfo);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_TDS_GAMEPROJECT_API UTDS_Inventar : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTDS_Inventar();

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnSwitchWeapon OnSwitchWeapon;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnAmmoChange OnAmmoChange;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnWeaponAdditionalInfoChange OnWeaponAdditionalInfoChange;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnWeaponAmmoEmpty OnWeaponAmmoEmpty;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnWeaponAmmoAviable OnWeaponAmmoAviable;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FOnUpdateWeaponSlots OnUpdateWeaponSlots;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Replicated, EditAnywhere, BluePrintReadWrite, Category = "Weapons")
		TArray<FWeaponSlot> WeaponSlots;

	UPROPERTY(Replicated, EditAnywhere, BluePrintReadWrite, Category = "Weapons")
		TArray<FAmmoSlot> AmmoSlots;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Weapons")
		int32 MaxSlotsWeapon = 0;


	bool SwitchWeaponToIndex(int32 ChangeToIndex, int32 OldIndex, FAdditionalWeaponInfo OldInfo, bool bIsForward);

	FAdditionalWeaponInfo GetAdditionalInfoWeapon(int32 IndexWeapon);
	int32 GetWeaponIndexSlotByName(FName IdWeaponName);
	FName GetWeaponNameBySlotIndex(int32 indexSlot);
	void SetAdditionalInfoWeapon(int32 IndexWeapon, FAdditionalWeaponInfo NewInfo);

	UFUNCTION(BlueprintCallable)
	void AmmoSlotChangeValue(EWeaponType TypeWeapon, int32 CoutChangeAmmo);

	bool CheckAmmoForWeapon(EWeaponType TypeWeapon, int8 &AviableAmmoForWeapon);

	UFUNCTION(BlueprintCallable, Category = "Interface")
		bool CheckCanTakeAmmo(EWeaponType AmmoType);
	UFUNCTION(BlueprintCallable, Category = "Interface")
		bool CheckCanTakeWeapon(int32& FreeSlot);
	UFUNCTION(BlueprintCallable, Category = "Interface")
		bool SwitchWeaponToInventory(FWeaponSlot NewWeapon, int32 IndexSlot, int32 CurrentIndexWeaponChar, FDropItem& DropItemInfo);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interface")
		void TryGetWeaponToInventory_OnServer(AActor* PickUpActor, FWeaponSlot NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Interface")
		bool GetDropItemInfoFromInventory(int32 IndexSlot, FDropItem& DropItemInfo);

	UFUNCTION(BlueprintCallable, Category = "Inventar")
		TArray<FWeaponSlot> GetWeaponSlot();

	UFUNCTION(BlueprintCallable, Category = "Inventar")
		TArray<FAmmoSlot> GetAmmoSlot();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventar")
		void InitInventory_OnServer(const TArray<FWeaponSlot>& NewWeaponSlotsInfo, const TArray<FAmmoSlot>& NewAmmoSlotsInfo);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Inventar")
		void AmmoChangeEvent_Multicast(EWeaponType TypeWeapon, int32 Cout);

	UFUNCTION(Server, Reliable)
		void SwitchWeaponEvent_OnServer(FName WeaponName, FAdditionalWeaponInfo AdditionalInfo, int32 IndexSlot);


};
