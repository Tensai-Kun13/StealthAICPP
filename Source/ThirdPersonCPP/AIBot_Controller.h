// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIBot_Controller.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONCPP_API AAIBot_Controller : public AAIController
{
	GENERATED_BODY()

public:
	AAIBot_Controller(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	void BeginPlay() override;
	void OnPossess(APawn* const Pawn) override;
	class UBlackboardComponent* GetBlackboard() const;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BTree;

	class UBlackboardComponent* Blackboard;

	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	void SetupPerceptionSystem();
};
