// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIBot_Controller.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BlackboardKeys.h"

UFindPlayerLocation::UFindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get player character and the NPC's controller
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const Controller = Cast<AAIBot_Controller>(OwnerComp.GetAIOwner());

	// get player location to use as an origin
	FVector const PlayerLocation = player->GetActorLocation();
	if (SearchRandom)
	{
		FNavLocation Location;

		// get the navigation system and generate a random location near the player
		UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

		if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Location, nullptr))
		{
			Controller->GetBlackboard()->SetValueAsVector(bb_keys::TargetLocation, Location.Location);
		}
	}
	else
	{
		Controller->GetBlackboard()->SetValueAsVector(bb_keys::TargetLocation, PlayerLocation);
	}

	// finish with success

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
