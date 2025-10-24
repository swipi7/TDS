// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "TDS_EnemyCharacter.h" 
#include "TimerManager.h"

// Sets default values for this component's properties
UAuraComponent::UAuraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UAuraComponent::BeginPlay()
{
	Super::BeginPlay();

	// Создаём сферу только когда владелец уже существует
	if (AActor* Owner = GetOwner())
	{
		AuraSphere = NewObject<USphereComponent>(Owner, TEXT("AuraSphere"));
		if (AuraSphere)
		{
			AuraSphere->RegisterComponent(); // обязательно!
			AuraSphere->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			AuraSphere->SetSphereRadius(AuraRadius);
			AuraSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			AuraSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
			AuraSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		}
	}

	// Таймер на урон
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &UAuraComponent::DealDamage, TickInterval, true);
	}
	
}


// Called every frame
void UAuraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAuraComponent::DealDamage()
{
	if (!AuraSphere) return;

	TArray<AActor*> OverlappingActors;
	AuraSphere->GetOverlappingActors(OverlappingActors, ATDS_EnemyCharacter::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		if (ATDS_EnemyCharacter* Enemy = Cast<ATDS_EnemyCharacter>(Actor))
		{
			Enemy->TakeDamage(DamagePerTick, FDamageEvent(), GetOwner()->GetInstigatorController(), GetOwner());
		}
	}
}

void UAuraComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}