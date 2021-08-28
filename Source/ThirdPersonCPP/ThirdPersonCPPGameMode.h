// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThirdPersonCPPCharacter.h"
#include "ThirdPersonCPPGameMode.generated.h"

UENUM()
enum class EGamePlayState
{
	EPlaying,
	EGameOver,
	EUnknown
};

UCLASS(minimalapi)
class AThirdPersonCPPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AThirdPersonCPPGameMode();

	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

	AThirdPersonCPPCharacter* MyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	int32 Seconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	int32 Minutes;

	UPROPERTY(EditAnywhere, Category = "Timer")
	FTimerHandle MemberTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Timer")
	int32 GetMinutes();

	UFUNCTION(BlueprintPure, Category = "Timer")
	int32 GetSeconds();

	UFUNCTION(BlueprintPure, Category = "GameState")
	EGamePlayState GetCurrentState() const;

	void SetCurrentState(EGamePlayState NewState);

	void Timer();

private:
	EGamePlayState CurrentState;

	void HandleNewState(EGamePlayState NewState);

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> StartMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> LoseWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UUserWidget* CurrentWidget;
};



