// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

UAttack::UAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Attack");
}


EBTNodeResult::Type UAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the bot
	AAIController* const Controller = OwnerComp.GetAIOwner();
	
	// Test to see if the Bot class supports the ICombateInterface
	if (AAI_Bot* const Bot = Cast<AAI_Bot>(Controller->GetPawn()))
	{
		// If the montage has finished playing, play it again - stops "jittering"
		if (MontageHasFinished(Bot))
		{
			// Execute the melee attack
			Bot->OnAttack();
		}
	}
	

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UAttack::MontageHasFinished(AAI_Bot* const Bot)
{
	return Bot->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Bot->GetMontage());
}
