// Fill out your copyright notice in the Description page of Project Settings.


#include "IncrementPathIndex.h"
#include "AIBot_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_Bot.h"
#include "BlackboardKeys.h"

UIncrementPathIndex::UIncrementPathIndex(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UIncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI Controller
	AAIBot_Controller* const Controller = Cast<AAIBot_Controller>(OwnerComp.GetAIOwner());
	AAI_Bot* const Bot = Cast<AAI_Bot>(Controller->GetPawn());
	int const NumberOfPoints = Bot->GetPatrolPath()->Num();
	int const MinIndex = 0;
	int const MaxIndex = NumberOfPoints - 1;

	// Get and set the blackboard index key
	int Index = Controller->GetBlackboard()->GetValueAsInt(bb_keys::PatrolPathIndex);

	// Reverses direction if at first or last index
	if (Index >= MaxIndex && Direction == EDirectionType::Forward)
	{
		Direction = EDirectionType::Reverse;
	}
	else if (Index == MinIndex && Direction == EDirectionType::Reverse)
	{
		Direction = EDirectionType::Forward;
	}
	// If going forward, increment index otherwise decrement
	Controller->GetBlackboard()->SetValueAsInt(bb_keys::PatrolPathIndex, (Direction == EDirectionType::Forward ? ++Index : --Index) % NumberOfPoints);

	// Finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
