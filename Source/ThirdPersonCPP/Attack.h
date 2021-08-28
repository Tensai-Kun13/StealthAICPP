// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI_Bot.h"
#include "Attack.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONCPP_API UAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UAttack(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool MontageHasFinished(AAI_Bot* const Bot);
};
