// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PatrolRoute.generated.h"

/**  
* A route card to help AI choose their next waypoint"
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUNDS_API UPatrolRoute : public UActorComponent
{
	GENERATED_BODY()
	
private:

	/** the list contains all patrol points  */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "PatrolRoute", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> PatrolPointsCpp;

public:

	/** returns patrol points list **/
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPointsCpp; }
	
};
