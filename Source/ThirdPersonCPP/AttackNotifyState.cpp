// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackNotifyState.h"
#include "Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "AI_Bot.h"

void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AAI_Bot* const Bot = Cast<AAI_Bot>(MeshComp->GetOwner()))
		{
			Bot->AttackStart();
		}
	}
}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AAI_Bot* const Bot = Cast<AAI_Bot>(MeshComp->GetOwner()))
		{
			Bot->AttackEnd();
		}
	}
}
