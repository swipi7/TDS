// Fill out your copyright notice in the Description page of Project Settings.


#include "TDS_GameInstance.h"


bool UTDS_GameInstance::GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo)
{
	bool bIsFind = false;
	FWeaponInfo* WeaponInfoRow;
	//Table
	if (WeaponInfoTable)
	{

		WeaponInfoRow = WeaponInfoTable->FindRow<FWeaponInfo>(NameWeapon, "", false);
		if (WeaponInfoRow)
		{
			bIsFind = true;
			OutInfo = *WeaponInfoRow;

			UE_LOG(LogTemp, Warning, TEXT("44444444"));
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTDS_GameInstance::GetWeaponInfoByName - WeaponTable -NULL"));
	}

	return bIsFind;
}

bool UTDS_GameInstance::GetWeaponInfoByWeaponName(FName NameItem, FDropItem& OutInfo)
{
	bool bIsFind = false;

	if (DropItemInfoTable)
	{
		FDropItem* DropItemInfoRow;
		TArray<FName>RowNames = DropItemInfoTable->GetRowNames();

		int8 i = 0;
		while (i < RowNames.Num() && !bIsFind)
		{
			DropItemInfoRow = DropItemInfoTable->FindRow<FDropItem>(RowNames[i], "");
			if (DropItemInfoRow->WeaponInfo.NameItem == NameItem)
			{
				OutInfo = (*DropItemInfoRow);
				bIsFind = true;
			}
			i++;//fix
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTPSGameInstance::GetDropItemInfoByName - DropItemInfoTable -NULL"));
	}

	return bIsFind;
}

bool UTDS_GameInstance::GetDropItemInfoByName(FName NameItem, FDropItem& OutInfo)
{
	bool bIsFind;
	//FDropItem* DropItemInfoRow;

	if (DropItemInfoTable)
	{
		FDropItem* DropItemInfoRow = DropItemInfoTable->FindRow<FDropItem>(NameItem, "", false);
		if (DropItemInfoRow)
		{
			bIsFind = true;
			OutInfo = *DropItemInfoRow;

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTPSGameInstance::GetDropItemInfoByName - DropItemInfoTable -NULL"));
	}
	return bIsFind;
}
