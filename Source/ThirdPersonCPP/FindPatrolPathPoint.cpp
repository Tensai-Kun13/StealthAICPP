// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPatrolPathPoint.h"
#include "AIBot_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_Bot.h"
#include "BlackboardKeys.h"

UFindPatrolPathPoint::UFindPatrolPathPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Patrol Path Point");
}

EBTNodeResult::Type UFindPatrolPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller for the patrolling NPC
	AAIBot_Controller* const Controller = Cast<AAIBot_Controller>(OwnerComp.GetAIOwner());

	// Get the current patrol path index from the blackboard
	int const Index = Controller->GetBlackboard()->GetValueAsInt(bb_keys::PatrolPathIndex);

	// Use the index to get the current patrol path from the bot's reference to the patrol path
	AAI_Bot* const Bot = Cast<AAI_Bot>(Controller->GetPawn());
	FVector const Point = Bot->GetPatrolPath()->GetPatrolPoint(Index);

	// Transform this point to a global position using its parent
	FVector const GlobalPoint = Bot->GetPatrolPath()->GetActorTransform().TransformPosition(Point);

	// Write the current global path point to the blackboard
	Controller->GetBlackboard()->SetValueAsVector(bb_keys::PatrolPathVector, GlobalPoint);
	
	// Finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
