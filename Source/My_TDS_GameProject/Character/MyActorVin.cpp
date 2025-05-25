// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyActorVin.h"
#include "GameFramework/Character.h"
#include "My_TDS_GameProjectCharacter.h" 
#include "EngineUtils.h"

#include "Net/UnrealNetwork.h"


// Sets default values
AMyActorVin::AMyActorVin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
}

// Called when the game starts or when spawned
void AMyActorVin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActorVin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AMyActorVin::OnVictory()
//{
//    if (HasAuthority()) 
//    {
//        bIsVictory = true; 
//        OnRep_Victory(); 
//    }
//}

//void AMyActorVin::DisablePlayerMovement()
//{
//    for (TActorIterator<AMy_TDS_GameProjectCharacter> It(GetWorld()); It; ++It)
//    {
//        AMy_TDS_GameProjectCharacter* Meen = *It;
//        if (Meen)
//        {
//            Meen->GetCharacterMovement()->DisableMovement();
//            Meen->DisableInput(nullptr);
//        }
//    }
//}

//void AMyActorVin::DisableEnemies()
//{
//    for (TActorIterator<AMy_TDS_GameProjectCharacter> It(GetWorld()); It; ++It)
//    {
//        AMy_TDS_GameProjectCharacter* Enemy = *It;
//        if (Enemy && Enemy->ActorHasTag("Enemy"))
//        {
//            Enemy->Destroy();
//        }
//    }
//}

//void AMyActorVin::DisableWeapons()
//{
//    for (TActorIterator<AMy_TDS_GameProjectCharacter> It(GetWorld()); It; ++It)
//    {
//        AMy_TDS_GameProjectCharacter* Character = *It;
//        if (Character)
//        {
//            //Character->DisableWeapon(); // Предположим, что у вас есть метод DisableWeapon
//        }
//    }
//}

//void AMyActorVin::OnRep_Victory()
//{
//    if (bIsVictory)
//    {
//        DisablePlayerMovement();
//        DisableEnemies();
//        DisableWeapons();
//    }
//}

//void AMyActorVin::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//    DOREPLIFETIME(AMyActorVin, bIsVictory);
//}