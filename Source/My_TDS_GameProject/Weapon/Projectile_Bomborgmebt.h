// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "My_TDS_GameProject/ProjectileDefault.h"
#include "Projectile_Bomborgmebt.generated.h"

UCLASS()
class MY_TDS_GAMEPROJECT_API AProjectile_Bomborgmebt : public AProjectileDefault
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_Bomborgmebt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TimerExplose(float Deltatime);

	virtual void BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void ImpactProjectile() override;

	void Explose();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grinade")
	bool TimerEnabled = false;
	float TimerToExplose = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grinade")
	float TimeToExplose = 5.0f;

};
