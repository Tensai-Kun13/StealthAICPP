// Fill out your copyright notice in the Description page of Project Settings.


#include "IsPlayerInRange.h"
#include "AIBot_Controller.h"
#include "AI_Bot.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardKeys.h"

UIsPlayerInRange::UIsPlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player in Range");
}


void UIsPlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// Get Bot
	AAIBot_Controller* const Controller = Cast<AAIBot_Controller>(OwnerComp.GetAIOwner());
	AAI_Bot* const Bot = Cast<AAI_Bot>(Controller->GetPawn());

	// Get player character
	ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Write true or false depending on whether the player is within Range
	Controller->GetBlackboard()->SetValueAsBool(bb_keys::PlayerIsInRange, Bot->GetDistanceTo(Player) <= Range);
}
