// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SetFocusAtActor.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API UBTS_SetFocusAtActor : public UBTService
{
	GENERATED_BODY()

	UBTS_SetFocusAtActor();
	
	/** update next tick interval
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** Target To Follow Blackboard Key */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector Enemy;
	
};
