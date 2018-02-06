// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ClearValue.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API UBTT_ClearValue : public UBTTaskNode
{
	GENERATED_BODY()
	
private:

	UBTT_ClearValue();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector LastSeenLocation;
		
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
