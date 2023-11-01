// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FuncLibrary/Types.h"
#include "Engine/DataTable.h"
#include "WeaponDefaults.h"
#include "TDS_GameInstance.generated.h"

UCLASS()
class MY_TDS_GAMEPROJECT_API UTDS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
public:	
	//table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " WeaponSetting ")
		UDataTable* WeaponInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " WeaponSetting ")
		UDataTable* DropItemInfoTable = nullptr;
	UFUNCTION(BlueprintCallable)
		bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo);
	
	UFUNCTION(BlueprintCallable)
		bool GetWeaponInfoByWeaponName(FName NameItem, FDropItem& OutInfo);
	UFUNCTION(BlueprintCallable)
		bool GetDropItemInfoByName(FName NameItem, FDropItem& OutInfo);

};
