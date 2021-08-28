// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBot_Controller.h"

#include "BlackboardKeys.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ConstructorHelpers.h"
#include "ThirdPersonCPPCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

AAIBot_Controller::AAIBot_Controller(FObjectInitializer const& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/MyContent/AI/BT_Bot.BT_Bot'"));
	if (obj.Succeeded())
	{
		BTree = obj.Object;
	}

	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
	SetupPerceptionSystem();
}

void AAIBot_Controller::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BTree);
	BehaviorTreeComponent->StartTree(*BTree);
}

void AAIBot_Controller::OnPossess(APawn* const Pawn)
{
	Super::OnPossess(Pawn);

	if (Blackboard)
	{
		Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);
	}
}

UBlackboardComponent* AAIBot_Controller::GetBlackboard() const
{
	return Blackboard;
}

void AAIBot_Controller::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto const ch = Cast<AThirdPersonCPPCharacter>(Actor))
	{
		GetBlackboard()->SetValueAsBool(bb_keys::CanSeePlayer, Stimulus.WasSuccessfullySensed());
	}
}

void AAIBot_Controller::SetupPerceptionSystem()
{
	// create and initialise sight configuration object
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	SightConfig->SightRadius = 500.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.0f;
	SightConfig->PeripheralVisionAngleDegrees = 45.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Add sight configuration component to perception component
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIBot_Controller::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}
