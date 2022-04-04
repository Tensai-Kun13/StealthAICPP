// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverVolume.h"

#include "Components/BoxComponent.h"

// Sets default values
ACoverVolume::ACoverVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	CollisionMesh->InitBoxExtent(FVector(200.0f, 200.0f, 200.0f));
	CollisionMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
	SetRootComponent(CollisionMesh);
}

// Called when the game starts or when spawned
void ACoverVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoverVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

