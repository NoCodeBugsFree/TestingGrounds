// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "ActorPool.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

AActor* UActorPool::CheckOut()
{
	/** pool is empty  */
	if (Pool.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("pool is empty!!!"));
		return nullptr;
	}

	/**  Pops element from the array  */
	return Pool.Pop();
}

void UActorPool::Return(AActor* ActorToReturn)
{
	if (!ensure(ActorToReturn)) { return; }
	Add(ActorToReturn);
}

void UActorPool::Add(AActor* ActorToAdd)
{
	if (!ensure(ActorToAdd)) { return; }
	Pool.Push(ActorToAdd);
}

