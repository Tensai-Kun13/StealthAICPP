// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bot.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ThirdPersonCPPCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AAI_Bot::AAI_Bot() :
	RightFistCollisionBox(CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox")))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Smooths NPC running and turning
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

    if (RightFistCollisionBox)
    {
		FVector const Extent(8.0f);
		RightFistCollisionBox->SetBoxExtent(Extent, false);
		RightFistCollisionBox->SetCollisionProfileName("NoCollision");
    }
}

// Called when the game starts or when spawned
void AAI_Bot::BeginPlay()
{
	Super::BeginPlay();

	// Set material color of character
	UMaterialInstanceDynamic* const material_instance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	if (material_instance)
	{
		material_instance->SetVectorParameterValue("BodyColor", FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		GetMesh()->SetMaterial(0, material_instance);
	}

	// Attach delegates to the collision box
	if (RightFistCollisionBox)
	{
		FAttachmentTransformRules const Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
		RightFistCollisionBox->AttachToComponent(GetMesh(), Rules, "hand_r_socket");
		RightFistCollisionBox->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));

		RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot::OnAttackOverlapBegin);
		RightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AAI_Bot::OnAttackOverlapEnd);
	}
	
}

void AAI_Bot::OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor,
	UPrimitiveComponent* OtherComponent, int const OtherBodyIndex, bool const FromSweep, FHitResult const& SweepResult)
{
	if (AThirdPersonCPPCharacter* const Player = Cast<AThirdPersonCPPCharacter>(OtherActor))
	{
		Player->SetHealth(Player->GetHealth() - Player->GetMaxHealth());
		UE_LOG(LogTemp, Error, TEXT("Hit"));
	}
}

void AAI_Bot::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor,
	UPrimitiveComponent* OtherComponent, int const OtherBodyIndex)
{

}

// Called every frame
void AAI_Bot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAI_Bot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

APatrolPath* AAI_Bot::GetPatrolPath()
{
	return PatrolPath;
}

void AAI_Bot::OnAttack()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}
}

void AAI_Bot::AttackStart()
{
	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
}

void AAI_Bot::AttackEnd()
{
	RightFistCollisionBox->SetCollisionProfileName("NoCollision");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(false);
}

UAnimMontage* AAI_Bot::GetMontage() const
{
	return Montage;
}

