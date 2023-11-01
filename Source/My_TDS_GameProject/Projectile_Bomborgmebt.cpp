// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Bomborgmebt.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

int32 DebugExplodeShow = 0;
FAutoConsoleVariableRef CVARExplodeShow(TEXT("TPS.DebugExplode"), DebugExplodeShow, TEXT("Draw Debug for Explode"), ECVF_Cheat);

// Sets default values
AProjectile_Bomborgmebt::AProjectile_Bomborgmebt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectile_Bomborgmebt::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile_Bomborgmebt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerExplose(DeltaTime);

}

void AProjectile_Bomborgmebt::TimerExplose(float Deltatime)
{
	if (TimerEnabled)
	{
		if (TimerToExplose > TimerToExplose)
		{
			Explose();
		}
		else
		{
			TimerToExplose += Deltatime;
		}
	}
}

void AProjectile_Bomborgmebt::BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::BulletCollisionSphereHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}



void AProjectile_Bomborgmebt::ImpactProjectile()
{
	//Init Bomborgment
	TimerEnabled = true;
}

void AProjectile_Bomborgmebt::Explose()
{
	if (DebugExplodeShow)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ProjectileMaxRadiusDamage, 12, FColor::Green, false, 12.0f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ProjectileMinRadiusDamage, 12, FColor::Green, false, 12.0f);
	}
	

	TimerEnabled = false;
	if (ProjectileSetting.ExploseFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSetting.ExploseFX, GetActorLocation(), GetActorRotation(), FVector(1.0f));
	}
	if (ProjectileSetting.ExploseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.ExploseSound, GetActorLocation());
	}

	TArray<AActor*> IgnoredActor;
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
		ProjectileSetting.ExploseMaxDamage,
		ProjectileSetting.ExploseMaxDamage * 0.2f,
		GetActorLocation(),
		1000.0f,
		2000.0f,
		5,
		NULL, IgnoredActor, nullptr, nullptr);

	this->Destroy();
}

