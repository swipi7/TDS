// Fill out your copyright notice in the Description page of Project Settings.


#include "TDS_Inventar.h"
#include "TDS_IGameActor.h"
#include "TDS_GameInstance.h"

#include "Net/UnrealNetwork.h"
#pragma optimize ("", off)

// Sets default values for this component's properties
UTDS_Inventar::UTDS_Inventar()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UTDS_Inventar::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	
}


// Called every frame
void UTDS_Inventar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UTDS_Inventar::SwitchWeaponToIndex(int32 ChangeToIndex, int32 OldIndex, FAdditionalWeaponInfo OldInfo, bool bIsForward)
{
	bool bIsSuccess = false;
	int8 CorrectIndex = ChangeToIndex;
	if (ChangeToIndex > WeaponSlots.Num() - 1)
		CorrectIndex = 0;
	else
		if (ChangeToIndex < 0)
			CorrectIndex = WeaponSlots.Num() - 1;

	FName NewIdWeapon;
	FAdditionalWeaponInfo NewAdditionalInfo;
	int32 NewCurrentIndex = 0;

	if (WeaponSlots.IsValidIndex(CorrectIndex))
	{
		if (!WeaponSlots[CorrectIndex].NameItem.IsNone())
		{
			if (WeaponSlots[CorrectIndex].AdditionalInfo.Round > 0)
			{
				//good weapon have ammo start change
				bIsSuccess = true;
			}
			else
			{
				UTDS_GameInstance* myGI = Cast<UTDS_GameInstance>(GetWorld()->GetGameInstance());
				if (myGI)
				{
					//check ammoSlots for this weapon
					FWeaponInfo myInfo;
					myGI->GetWeaponInfoByName(WeaponSlots[CorrectIndex].NameItem, myInfo);

					bool bIsFind = false;
					int8 j = 0;
					while (j < AmmoSlots.Num() && !bIsFind)
					{
						if (AmmoSlots[j].WeaponType == myInfo.WeaponType && AmmoSlots[j].Cout > 0)
						{
							//good weapon have ammo start change
							bIsSuccess = true;
							bIsFind = true;
						}
						j++;
					}
				}
			}
			if (bIsSuccess)
			{
				NewCurrentIndex = CorrectIndex;
				NewIdWeapon = WeaponSlots[CorrectIndex].NameItem;
				NewAdditionalInfo = WeaponSlots[CorrectIndex].AdditionalInfo;
			}
		}
	}
	if (!bIsSuccess)
	{
		
		int8 iteration = 0;
		int8 Seconditeration = 0;
		int8 tmpIndex = 0;
		while (iteration < WeaponSlots.Num() && !bIsSuccess)
		{
			iteration++;

			if (bIsForward)
			{

				tmpIndex = ChangeToIndex + iteration;
			}
			else
			{
				Seconditeration = WeaponSlots.Num() - 1;

				tmpIndex = ChangeToIndex - iteration;
			}

			if (WeaponSlots.IsValidIndex(tmpIndex))
			{
				if (!WeaponSlots[tmpIndex].NameItem.IsNone())
				{
					if (WeaponSlots[tmpIndex].AdditionalInfo.Round > 0)
					{
						//WeaponGood
						bIsSuccess = true;
						NewIdWeapon = WeaponSlots[tmpIndex].NameItem;
						NewAdditionalInfo = WeaponSlots[tmpIndex].AdditionalInfo;
						NewCurrentIndex = tmpIndex;
					}
					else
					{
						FWeaponInfo myInfo;
						UTDS_GameInstance* myGI = Cast<UTDS_GameInstance>(GetWorld()->GetGameInstance());

						myGI->GetWeaponInfoByName(WeaponSlots[tmpIndex].NameItem, myInfo);

						bool bIsFind = false;
						int8 j = 0;
						while (j < AmmoSlots.Num() && !bIsFind)
						{
							if (AmmoSlots[j].WeaponType == myInfo.WeaponType && AmmoSlots[j].Cout > 0)
							{
								//WeaponGood
								bIsSuccess = true;
								NewIdWeapon = WeaponSlots[tmpIndex].NameItem;
								NewAdditionalInfo = WeaponSlots[tmpIndex].AdditionalInfo;
								NewCurrentIndex = tmpIndex;
								bIsFind = true;
							}
							j++;
						}
					}
				}
			}
			else
			{
				//go to end of LEFT of array weapon slots
				if (OldIndex != Seconditeration)
				{
					if (WeaponSlots.IsValidIndex(Seconditeration))
					{
						if (!WeaponSlots[Seconditeration].NameItem.IsNone())
						{
							if (WeaponSlots[Seconditeration].AdditionalInfo.Round > 0)
							{
								//WeaponGood
								bIsSuccess = true;
								NewIdWeapon = WeaponSlots[Seconditeration].NameItem;
								NewAdditionalInfo = WeaponSlots[Seconditeration].AdditionalInfo;
								NewCurrentIndex = Seconditeration;
							}
							else
							{
								FWeaponInfo myInfo;
								UTDS_GameInstance* myGI = Cast<UTDS_GameInstance>(GetWorld()->GetGameInstance());

								myGI->GetWeaponInfoByName(WeaponSlots[Seconditeration].NameItem, myInfo);

								bool bIsFind = false;
								int8 j = 0;
								while (j < AmmoSlots.Num() && !bIsFind)
								{
									if (AmmoSlots[j].WeaponType == myInfo.WeaponType && AmmoSlots[j].Cout > 0)
									{
										//WeaponGood
										bIsSuccess = true;
										NewIdWeapon = WeaponSlots[Seconditeration].NameItem;
										NewAdditionalInfo = WeaponSlots[Seconditeration].AdditionalInfo;
										NewCurrentIndex = Seconditeration;
										bIsFind = true;
									}
									j++;
								}
							}
						}
					}
				}
				else
				{
					//go to same weapon when start
					if (WeaponSlots.IsValidIndex(Seconditeration))
					{
						if (!WeaponSlots[Seconditeration].NameItem.IsNone())
						{
							if (WeaponSlots[Seconditeration].AdditionalInfo.Round > 0)
							{
								//WeaponGood, it same weapon do nothing
							}
							else
							{
								FWeaponInfo myInfo;
								UTDS_GameInstance* myGI = Cast<UTDS_GameInstance>(GetWorld()->GetGameInstance());

								myGI->GetWeaponInfoByName(WeaponSlots[Seconditeration].NameItem, myInfo);

								bool bIsFind = false;
								int8 j = 0;
								while (j < AmmoSlots.Num() && !bIsFind)
								{
									if (AmmoSlots[j].WeaponType == myInfo.WeaponType)
									{
										if (AmmoSlots[j].Cout > 0)
										{
											//WeaponGood, it same weapon do nothing
										}
										else
										{
											//Not find weapon with ammo need init Pistol with infinity ammo
											//UE_LOG(LogTemp, Error, TEXT("UTPSInventoryComponent::SwitchWeaponToIndex - Init PISTOL - NEED"));
										}
									}
									j++;
								}
							}
						}
					}
				}
				if (bIsForward)
				{
					Seconditeration++;
				}
				else
				{
					Seconditeration--;
				}
			}
		}
		
	}
	if (bIsSuccess)
	{
		SetAdditionalInfoWeapon(OldIndex, OldInfo);
		//OnSwitchWeapon.Broadcast(NewIdWeapon, NewAdditionalInfo, NewCurrentIndex);
		SwitchWeaponEvent_OnServer(NewIdWeapon, NewAdditionalInfo, NewCurrentIndex);
		//OnWeaponAmmoAviable.Broadcast()
	}

	return bIsSuccess;
}

bool UTDS_Inventar::SwitchWeaponByIndex(int32 IndexWeaponToChange, int32 PreviosIndex, FAdditionalWeaponInfo PreviosWeaponInfo)
{
	bool bIsSuccess = false;
	FName ToSwitchIdWeapon;
	FAdditionalWeaponInfo ToSwitchAdditionalInfo;

	ToSwitchIdWeapon = GetWeaponNameBySlotIndex(IndexWeaponToChange);
	ToSwitchAdditionalInfo = GetAdditionalInfoWeapon(IndexWeaponToChange);

	if (!ToSwitchIdWeapon.IsNone())
	{
		SetAdditionalInfoWeapon(PreviosIndex, PreviosWeaponInfo);

		SwitchWeaponEvent_OnServer(ToSwitchIdWeapon, ToSwitchAdditionalInfo, IndexWeaponToChange);

		EWeaponType ToSwitchWeaponType;
		if (GetWeaponTypeByNameWeapon(ToSwitchIdWeapon, ToSwitchWeaponType))
		{
			int8 AviableAmmoForWeapon = -1;
			if (CheckAmmoForWeapon(ToSwitchWeaponType, AviableAmmoForWeapon))
			{

			}
		}
		bIsSuccess = true;
	}
	return bIsSuccess;
}

FAdditionalWeaponInfo UTDS_Inventar::GetAdditionalInfoWeapon(int32 IndexWeapon)
{
	FAdditionalWeaponInfo result;
	if (WeaponSlots.IsValidIndex(IndexWeapon))
	{
		bool bIsFind = false;
		int8 i = 0;
		while (i < WeaponSlots.Num() && !bIsFind)
		{
			if (/*WeaponSlots[i].IndexSlot*/i == IndexWeapon)
			{
				result = WeaponSlots[i].AdditionalInfo;
				bIsFind = true;
			}
			i++;
		}
		//if(bIsFind)
	}
	


	return result;
}

int32 UTDS_Inventar::GetWeaponIndexSlotByName(FName IdWeaponName)
{
	int32 result = -1;
	int8 i = 0;
	bool bIsFind = false;
	while (i < WeaponSlots.Num() && !bIsFind)
	{
		if (WeaponSlots[i].NameItem == IdWeaponName)
		{
			bIsFind = true;
			result = i;
		}
		i++;
	}

	return result;
}

FName UTDS_Inventar::GetWeaponNameBySlotIndex(int32 indexSlot)
{
	FName result;

	if (WeaponSlots.IsValidIndex(indexSlot))
	{
		result = WeaponSlots[indexSlot].NameItem;
	}
	return result;
}

void UTDS_Inventar::SetAdditionalInfoWeapon(int32 indexSlot, FAdditionalWeaponInfo NewInfo)
{
	if (WeaponSlots.IsValidIndex(indexSlot))
	{
		bool bIsFind = false;
		int8 i = 0;
		while (i < WeaponSlots.Num() && !bIsFind)
		{
			if (/*WeaponSlots[i].IndexSlot*/i == indexSlot)
			{
				WeaponSlots[i].AdditionalInfo = NewInfo;
				bIsFind = true;
				
				//OnWeaponAdditionalInfoChange.Broadcast(IndexWeapon, NewInfo);
				WeaponAdditionalInfoChangeEvent_Multicast(indexSlot, NewInfo);
			}
			i++;
		}
	}
}

bool UTDS_Inventar::GetWeaponTypeByNameWeapon(FName IdWeaponName, EWeaponType& WeaponType)
{
	bool bIsFind = false;
	FWeaponInfo OutInfo;
	WeaponType = EWeaponType::RifleType;
	UTDS_GameInstance* myGI = Cast<UTDS_GameInstance>(GetWorld()->GetGameInstance());
	if (myGI)
	{
		myGI->GetWeaponInfoByName(IdWeaponName, OutInfo);
		WeaponType = OutInfo.WeaponType;
		bIsFind = true;
	}
	return bIsFind;
}

void UTDS_Inventar::AmmoSlotChangeValue(EWeaponType TypeWeapon, int32 CoutChangeAmmo)
{
	bool bIsFind = false;
	int32  i = 0;
	while (i<AmmoSlots.Num()&& !bIsFind)
	{
		if (AmmoSlots[i].WeaponType == TypeWeapon)
		{
			AmmoSlots[i].Cout += CoutChangeAmmo;
			if (AmmoSlots[i].Cout > AmmoSlots[i].MaxCout)
				AmmoSlots[i].Cout = AmmoSlots[i].MaxCout;

			AmmoChangeEvent_Multicast(AmmoSlots[i].WeaponType, AmmoSlots[i].Cout);
			
			//OnAmmoChange.Broadcast(AmmoSlots[i].WeaponType, AmmoSlots[i].Cout);

			bIsFind = true;

		}
		i++;
	}
}

bool UTDS_Inventar::CheckAmmoForWeapon(EWeaponType TypeWeapon, int8 &AviableAmmoForWeapon)
{
	AviableAmmoForWeapon = 0;
	bool bIsFind = false;
	int8 i = 0;

	while (i < AmmoSlots.Num() && !bIsFind)
	{

		if (AmmoSlots[i].WeaponType == TypeWeapon)
		{
			bIsFind = true;
			AviableAmmoForWeapon = AmmoSlots[i].Cout;
			if (AmmoSlots[i].Cout > 0)
			{

			return true;
			}
		}
			
		i++;
	}

	if (AviableAmmoForWeapon <= 0)
	{
		WeaponAmmoEmptyEvemt_Multicast(TypeWeapon);
	}
	else
	{
		WeaponAmmoAviable_Multicast(TypeWeapon);
	}

	//OnWeaponAmmoEmpty.Broadcast(TypeWeapon);

	return false;
}

bool UTDS_Inventar::CheckCanTakeAmmo(EWeaponType AmmoType)
{
	bool result = false;
	int8 i = 0;
	while (i < AmmoSlots.Num() && !result)
	{
		if (AmmoSlots[i].WeaponType == AmmoType && AmmoSlots[i].Cout < AmmoSlots[i].MaxCout)
			result = true;
		i++;
	}
	return result;
}

bool UTDS_Inventar::CheckCanTakeWeapon(int32 &FreeSlot)
{
	bool bIsFreeSlot = false;
	int8 i = 0;
	while (i < WeaponSlots.Num() && !bIsFreeSlot)
	{
		if (WeaponSlots[i].NameItem.IsNone())
		{
			bIsFreeSlot = true;
			FreeSlot = i;
		}
		i++;
	}
	return bIsFreeSlot;
}

bool UTDS_Inventar::SwitchWeaponToInventory(FWeaponSlot NewWeapon, int32 IndexSlot, int32 CurrentIndexWeaponChar, FDropItem& DropItemInfo)
{
	bool result = false;

	if (WeaponSlots.IsValidIndex(IndexSlot) && GetDropItemInfoFromInventory(IndexSlot, DropItemInfo))
	{
		WeaponSlots[IndexSlot] = NewWeapon;

		SwitchWeaponToIndex(CurrentIndexWeaponChar, -1, NewWeapon.AdditionalInfo, true);

		UpdateWeaponSlotsEvent_Multicast(IndexSlot, NewWeapon);
		//OnUpdateWeaponSlots.Broadcast(IndexSlot, NewWeapon);
		result = true;
	}
	return result;
}

void UTDS_Inventar::TryGetWeaponToInventory_OnServer_Implementation(AActor* PickUpActor, FWeaponSlot NewWeapon)
{

	int32 indexSlot = -1;
	if (CheckCanTakeWeapon(indexSlot))
	{
		if (WeaponSlots.IsValidIndex(indexSlot))
		{
			WeaponSlots[indexSlot] = NewWeapon;
			
			UpdateWeaponSlotsEvent_Multicast(indexSlot, NewWeapon);
			
				//OnUpdateWeaponSlots.Broadcast(indexSlot, NewWeapon);
			if (PickUpActor)
			{
				PickUpActor->Destroy();
			}


		}
	}
}

bool UTDS_Inventar::GetDropItemInfoFromInventory(int32 IndexSlot, FDropItem& DropItemInfo)
{
	bool result = false;

	FName DropItemName = GetWeaponNameBySlotIndex(IndexSlot);

	UTDS_GameInstance* myGI = Cast<UTDS_GameInstance>(GetWorld()->GetGameInstance());
	if (myGI)
	{
		result = myGI->GetWeaponInfoByWeaponName(DropItemName, DropItemInfo);
		if (WeaponSlots.IsValidIndex(IndexSlot))
		{
			DropItemInfo.WeaponInfo.AdditionalInfo = WeaponSlots[IndexSlot].AdditionalInfo;
		}
	}

	return result;
}

TArray<FWeaponSlot> UTDS_Inventar::GetWeaponSlot()
{
	return WeaponSlots;
}

TArray<FAmmoSlot> UTDS_Inventar::GetAmmoSlot()
{
	return AmmoSlots;
}

void UTDS_Inventar::DropWeapobByIndex_OnServer_Implementation(int32 ByIndex)
{
	FDropItem DropItemInfo;
	FWeaponSlot EmtyWeaponSlot;

	bool bIsCanDrop = false;
	int8 i = 0;
	int8 AviableWeaponNum = 0;
	while (i < WeaponSlots.Num() && !bIsCanDrop)
	{
		if (!WeaponSlots[i].NameItem.IsNone())
		{
			AviableWeaponNum++;
			if (AviableWeaponNum > 1)
				bIsCanDrop = true;
		}
		i++;
	}

	if (bIsCanDrop && WeaponSlots.IsValidIndex(ByIndex) && GetDropItemInfoFromInventory(ByIndex, DropItemInfo))
	{

		//switch weapon to valid slot weapon from start weapon slots array
		bool bIsFindWeapon = false;
		int8 j = 0;
		while (j < WeaponSlots.Num() && !bIsFindWeapon)
		{
			if (!WeaponSlots[j].NameItem.IsNone())
			{
				SwitchWeaponEvent_OnServer(WeaponSlots[j].NameItem, WeaponSlots[j].AdditionalInfo, j);
				//OnSwitchWeapon.Broadcast(WeaponSlots[j].NameItem, WeaponSlots[j].AdditionalInfo, j);
			}
			j++;
		}

		WeaponSlots[ByIndex] = EmtyWeaponSlot;
		if (GetOwner()->GetClass()->ImplementsInterface(UTDS_Inventar::StaticClass()))
		{
			ITDS_IGameActor::Execute_DropWeaponToWorld(GetOwner(), DropItemInfo);
		}

		UpdateWeaponSlotsEvent_Multicast(ByIndex, EmtyWeaponSlot);
		//OnUpdateWeaponSlots.Broadcast(ByIndex, EmtyWeaponSlot);
	}
}




void UTDS_Inventar::WeaponAmmoEmptyEvemt_Multicast_Implementation(EWeaponType TypeWeapon)
{
	OnWeaponAmmoEmpty.Broadcast(TypeWeapon);
}

void UTDS_Inventar::WeaponAmmoAviable_Multicast_Implementation(EWeaponType TypeWeapon)
{
	OnWeaponAmmoAviable.Broadcast(TypeWeapon);
}

void UTDS_Inventar::WeaponAdditionalInfoChangeEvent_Multicast_Implementation(int32 IndexSlot, FAdditionalWeaponInfo AdditionalInfo)
{
	OnWeaponAdditionalInfoChange.Broadcast(IndexSlot, AdditionalInfo);
}

void UTDS_Inventar::UpdateWeaponSlotsEvent_Multicast_Implementation(int32 IndexSlotChange, FWeaponSlot NewInfo)
{
	OnUpdateWeaponSlots.Broadcast(IndexSlotChange, NewInfo);
}


void UTDS_Inventar::SwitchWeaponEvent_OnServer_Implementation(FName WeaponName, FAdditionalWeaponInfo AdditionalInfo, int32 IndexSlot)
{
	OnSwitchWeapon.Broadcast(WeaponName, AdditionalInfo, IndexSlot);
	
}

void UTDS_Inventar::AmmoChangeEvent_Multicast_Implementation(EWeaponType TypeWeapon, int32 Cout)
{
	OnAmmoChange.Broadcast(TypeWeapon, Cout);
}

void UTDS_Inventar::InitInventory_OnServer_Implementation(const TArray<FWeaponSlot>& NewWeaponSlotsInfo, const TArray<FAmmoSlot>& NewAmmoSlotsInfo)
{

	WeaponSlots = NewWeaponSlotsInfo;
	AmmoSlots = NewAmmoSlotsInfo;



	MaxSlotsWeapon = WeaponSlots.Num();

	if (WeaponSlots.IsValidIndex(0))
	{
		if (!WeaponSlots[0].NameItem.IsNone())
			//OnSwitchWeapon.Broadcast(WeaponSlots[0].NameItem, WeaponSlots[0].AdditionalInfo, 0);
			SwitchWeaponEvent_OnServer(WeaponSlots[0].NameItem, WeaponSlots[0].AdditionalInfo, 0);
	}
}

void UTDS_Inventar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(UTDS_Inventar, WeaponSlots);
	DOREPLIFETIME(UTDS_Inventar, AmmoSlots);


}

#pragma optimize ("", on)
