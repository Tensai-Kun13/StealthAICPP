// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "AIBot_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BlackboardKeys.h"

UChasePlayer::UChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get TargetLocation from blackboard via AI Controller
	AAIBot_Controller* const cont = Cast<AAIBot_Controller>(OwnerComp.GetAIOwner());
	FVector const PlayerLocation = cont->GetBlackboard()->GetValueAsVector(bb_keys::TargetLocation);

	// move to the player's location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

	//finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
