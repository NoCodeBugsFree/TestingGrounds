// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_InRange.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API UBTD_InRange : public UBTDecorator
{
	GENERATED_BODY()

	UBTD_InRange();
	
	/** Enemy Blackboard key */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector Enemy; 
	
	/** distance to check from character to enemy  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float AcceptableDistance = 500.f;
	
	/** calculates raw, core value of decorator's condition. Should not include calling IsInversed */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
