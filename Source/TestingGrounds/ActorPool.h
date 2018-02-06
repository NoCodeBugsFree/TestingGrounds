// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUNDS_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	/** calls to Pop element from the pool  */
	AActor* CheckOut();

	/** calls to Push element to the pool  */
	void Return(AActor* ActorToReturn);

	/** calls to Push element to the pool  */
	void Add(AActor* ActorToAdd);

protected:

	// Sets default values for this component's properties
	UActorPool();

private:
	
	/** collects all actors previously added to pool  */
	UPROPERTY()
	TArray<AActor*> Pool;

};
