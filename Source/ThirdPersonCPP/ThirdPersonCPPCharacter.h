// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCPPCharacter.generated.h"

UCLASS(config=Game)
class AThirdPersonCPPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AThirdPersonCPPCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "MapCamera")
	AActor* MapCamera;

	UPROPERTY(EditAnywhere, Category = "MapCamera")
	AActor* PlayerCamera;

	// Crouch to Stand Anim Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* CrouchToStand;

	// Stand to Crouch Anim Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* StandToCrouch;

	// Determines if the character is currently crouching.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bIsCrouching;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Getters and Setters for health etc.
	float GetHealth() const;

	float GetMaxHealth() const;

	void SetHealth(float const NewHealth);

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/** Called for Map input */
	void ChangeCamera();

	/** Called for Raycast input */
	void DisplayRaycast();

	/** Called for Cover input */
	void WallTrace();

	/** Called for Crouch input */
	void StartCrouch();
	void StopCrouch();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	
private:
	float const MaxHealth = 100.0f;
	float CurrentHealth;
	bool bIsInCover;

	class UAIPerceptionStimuliSourceComponent* Stimulus;

	void SetupStimulus();

	void StartCover(FVector PlaneNormal);

	void StopCover();

	void CoverTrace(FVector Direction, float Value);
};

