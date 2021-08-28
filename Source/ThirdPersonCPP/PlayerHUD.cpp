// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "UserWidget.h"

APlayerHUD::APlayerHUD()
{
	// Set the timer widget
	static ConstructorHelpers::FClassFinder<UUserWidget> TimerObj(TEXT("/Game/MyContent/UI/Timer_UI"));
	HUDWidgetClass = TimerObj.Class;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	// Setup HUD Widget
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
