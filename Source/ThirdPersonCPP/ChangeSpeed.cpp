// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeSpeed.h"
#include "AI_Bot.h"
#include "AIBot_Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

UChangeSpeed::UChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}


void UChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto const Controller = OwnerComp.GetAIOwner();
	AAI_Bot* const Bot = Cast<AAI_Bot>(Controller->GetPawn());
	Bot->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

FString UChangeSpeed::GetStaticDescription() const
{
	return FString("Change the Bot speed");
}
