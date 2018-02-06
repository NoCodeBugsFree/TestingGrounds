// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "BTD_InRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTD_InRange::UBTD_InRange()
{
	Enemy.SelectedKeyName = "Enemy";
	NodeName = FString("In Range");
	FlowAbortMode = EBTFlowAbortMode::Both;
	SetIsInversed(false);
}

bool UBTD_InRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (OwnerComp.GetAIOwner() && OwnerComp.GetAIOwner()->GetPawn())
	{
		FVector SelfLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();

		if (OwnerComp.GetBlackboardComponent())
		{
			if(AActor* PlayerPawn = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Enemy.SelectedKeyName)))
			{
				FVector PlayerPawnLocation = PlayerPawn->GetActorLocation();

				float DistanceBetween = (PlayerPawnLocation - SelfLocation).Size();
				return DistanceBetween < AcceptableDistance;
			}
		}
	}

	return false;
}
