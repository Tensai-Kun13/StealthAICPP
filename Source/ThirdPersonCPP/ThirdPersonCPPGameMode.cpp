// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonCPPGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"
#include "ThirdPersonCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "UserWidget.h"

AThirdPersonCPPGameMode::AThirdPersonCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Use our custom HUD class
	HUDClass = APlayerHUD::StaticClass();

	Minutes = 0;
	Seconds = 30;

	// Set main menu widget
	static ConstructorHelpers::FClassFinder<UUserWidget> MainObj(TEXT("/Game/MyContent/UI/Widget_GameStart"));
	MainMenuWidgetClass = MainObj.Class;

	// Set start menu widget
	static ConstructorHelpers::FClassFinder<UUserWidget> StartObj(TEXT("/Game/MyContent/UI/Widget_StartLevel"));
	StartMenuWidgetClass = StartObj.Class;

	// Set gameover widget
	static ConstructorHelpers::FClassFinder<UUserWidget> LoseObj(TEXT("/Game/MyContent/UI/Widget_LoseMenu"));
	LoseWidgetClass = LoseObj.Class;
}

void AThirdPersonCPPGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AThirdPersonCPPGameMode::Timer, 1.0f, true, 0.5f);

	SetCurrentState(EGamePlayState::EPlaying);

	MyCharacter = Cast<AThirdPersonCPPCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AThirdPersonCPPGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

int32 AThirdPersonCPPGameMode::GetMinutes()
{
	return Minutes;
}

int32 AThirdPersonCPPGameMode::GetSeconds()
{
	return Seconds;
}

EGamePlayState AThirdPersonCPPGameMode::GetCurrentState() const
{
	return CurrentState;
}

void AThirdPersonCPPGameMode::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void AThirdPersonCPPGameMode::Timer()
{
	if (Seconds == 0)
	{
		if (Minutes == 0)
		{
			SetCurrentState(EGamePlayState::EGameOver);
		}
		else
		{
			Seconds = 59;
			--Minutes;
		}
	}
	else
	{
		--Seconds;
	}
}

void AThirdPersonCPPGameMode::HandleNewState(EGamePlayState NewState)
{
	switch (NewState)
	{
	case EGamePlayState::EPlaying:
		if ((GetWorld()->GetName() == TEXT("MainMenu")) && (MainMenuWidgetClass != nullptr))
	{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);

				if (CurrentWidget)
				{
					CurrentWidget->AddToViewport();
				}
	}
		// Setup Start Menu Widget
		else if ((GetWorld()->GetName() == TEXT("FirstPersonExampleMap")) && StartMenuWidgetClass != nullptr)
		{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StartMenuWidgetClass);

			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport();
			}
		}
		break;

	case EGamePlayState::EGameOver:
		// Setup Gameover Widget
		if (LoseWidgetClass != nullptr)
		{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), LoseWidgetClass);

			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport();
			}
		}
		break;

	case EGamePlayState::EUnknown:
	default:
		// Do nothing
		break;
	}
}
