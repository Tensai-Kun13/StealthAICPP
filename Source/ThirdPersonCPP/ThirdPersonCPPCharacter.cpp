// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonCPPCharacter.h"

#include "AI_Bot.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ThirdPersonCPPGameMode.h"

//////////////////////////////////////////////////////////////////////////
// AThirdPersonCPPCharacter

AThirdPersonCPPCharacter::AThirdPersonCPPCharacter() : CurrentHealth(MaxHealth)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	SetupStimulus();
}

void AThirdPersonCPPCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Limit pitch of camera
	APlayerCameraManager* const CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	CameraManager->ViewPitchMin = -50.0f;
	CameraManager->ViewPitchMax = 10.0f;

	// Set material color of character
	UMaterialInstanceDynamic* const MaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	if (MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue("BodyColor", FLinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		GetMesh()->SetMaterial(0, MaterialInstance);
	}
}

void AThirdPersonCPPCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AAI_Bot>(OtherActor))
	{
		AThirdPersonCPPGameMode* MyGameMode = (AThirdPersonCPPGameMode*)GetWorld()->GetAuthGameMode();
		MyGameMode->SetCurrentState(EGamePlayState::EGameOver);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThirdPersonCPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AThirdPersonCPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThirdPersonCPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AThirdPersonCPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AThirdPersonCPPCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AThirdPersonCPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AThirdPersonCPPCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AThirdPersonCPPCharacter::OnResetVR);

	// Bind ChangeCamera event
	PlayerInputComponent->BindAction("Map", IE_Pressed, this, &AThirdPersonCPPCharacter::ChangeCamera).bExecuteWhenPaused = true;

	// Bind DisplayRaycast event
	PlayerInputComponent->BindAction("Raycast", IE_Pressed, this, &AThirdPersonCPPCharacter::DisplayRaycast);
}

void AThirdPersonCPPCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AThirdPersonCPPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AThirdPersonCPPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AThirdPersonCPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonCPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonCPPCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AThirdPersonCPPCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AThirdPersonCPPCharacter::ChangeCamera()
{
	const float SmoothBlendTime = 0.75f;

	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (MapCamera && (OurPlayerController->GetViewTarget() == PlayerCamera))
	{
		// Cut insantly to MapCamera.
		OurPlayerController->SetViewTarget(MapCamera);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	else if (PlayerCamera)
	{
		// Blend smoothly to PlayerCamera
		OurPlayerController->SetViewTargetWithBlend(PlayerCamera, SmoothBlendTime);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void AThirdPersonCPPCharacter::DisplayRaycast()
{
	// Instantiate variables
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector EndTrace = ((ForwardVector * 3319.0f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	// checks the hit result and displays result to in engine debugger
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		// Draws a red debug line on screen that persists
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);

		// Prints "You hit: (actor name)" as engine screen debug message
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitResult->Actor->GetName()));
	}
	else
	{
		// Draws a red debug line on screen that persists
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);

		// Prints debug message that the line trace did not hit an actor
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("You did not hit an actor")));
	}
}

float AThirdPersonCPPCharacter::GetHealth() const
{
	return CurrentHealth;
}

float AThirdPersonCPPCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void AThirdPersonCPPCharacter::SetHealth(float const NewHealth)
{
	CurrentHealth = NewHealth;
	if (CurrentHealth <= 0)
	{
		AThirdPersonCPPGameMode* GameMode = (AThirdPersonCPPGameMode*)GetWorld()->GetAuthGameMode();
		GameMode->SetCurrentState(EGamePlayState::EGameOver);
	}
}

void AThirdPersonCPPCharacter::SetupStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}
