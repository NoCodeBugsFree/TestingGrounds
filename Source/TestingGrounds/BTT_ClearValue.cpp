// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "BTT_ClearValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_ClearValue::UBTT_ClearValue()
{
	LastSeenLocation.SelectedKeyName = "LastSeenLocation";
	NodeName = FString("Clear Value");
}

EBTNodeResult::Type UBTT_ClearValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		BlackboardComp->ClearValue(LastSeenLocation.SelectedKeyName);
	}

	return EBTNodeResult::Succeeded;
}
