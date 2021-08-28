// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AIBot_Controller.h"
// #include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BlackboardKeys.h"

UFindRandomLocation::UFindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI contronller and its NPC
	auto const Controller = Cast<AAIBot_Controller>(OwnerComp.GetAIOwner());
	auto const NPC = Controller->GetPawn();

	// obtain npc location to use as an origin location
	FVector const Origin = NPC->GetActorLocation();
	FNavLocation Loc;

	// get the navigation system and generate a random location on the NavMesh
	UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc, nullptr))
	{
		Controller->GetBlackboard()->SetValueAsVector(bb_keys::TargetLocation, Loc.Location);
	}

	// finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
