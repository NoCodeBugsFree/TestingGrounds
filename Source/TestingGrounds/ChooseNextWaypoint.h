// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "ChooseNextWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API UChooseNextWaypoint : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:

	UChooseNextWaypoint();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** next way point index  */
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector IndexKey;

	/** way point reference  */
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector WayPointKey;
	
};