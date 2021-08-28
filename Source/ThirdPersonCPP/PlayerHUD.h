// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONCPP_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	APlayerHUD();

	void BeginPlay() override;

private:
	/* HUD Widget asset pointer */
	UPROPERTY(EditAnywhere, Category = "Timer")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Timer")
	class UUserWidget* CurrentWidget;
};
