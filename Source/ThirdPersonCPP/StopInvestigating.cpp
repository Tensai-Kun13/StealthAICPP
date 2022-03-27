// Fill out your copyright notice in the Description page of Project Settings.


#include "StopInvestigating.h"
#include "AIBot_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardKeys.h"

UStopInvestigating::UStopInvestigating(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Stop Investigating");
}

EBTNodeResult::Type UStopInvestigating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get the NPC controller and write false to the blackboard key "IsInvestigating"
	auto const Controller = Cast<AAIBot_Controller>(OwnerComp.GetAIOwner());
	Controller->GetBlackboard()->SetValueAsBool(bb_keys::IsInvestigating, false);
	
	// finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return Super::ExecuteTask(OwnerComp, NodeMemory);
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
