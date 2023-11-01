//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "Magazin.h"
//
//// Sets default values
//AMagazin::AMagazin()
//{
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh "));
//	StaticMeshComponent->SetGenerateOverlapEvents(false);
//	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
//	StaticMeshComponent->SetupAttachment(RootComponent);
//
//
//}
//
//// Called when the game starts or when spawned
//void AMagazin::BeginPlay()
//{
//	Super::BeginPlay();
//		
//	if(MagazinMesh)
//		StaticMeshComponent->SetStaticMesh(MagazinMesh);
//
//
//}
//
//// Called every frame
//void AMagazin::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
