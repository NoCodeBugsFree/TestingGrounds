// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "BTS_UpdateLastLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_UpdateLastLocation::UBTS_UpdateLastLocation()
{
	Enemy.SelectedKeyName = "Enemy";
	LastSeenLocation.SelectedKeyName = "LastSeenLocation";
	NodeName = FString("Update Last Location");

	Interval = 1.f;
	RandomDeviation = 0.2f;
	bCallTickOnSearchStart = false;
}

void UBTS_UpdateLastLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB)
	{
		if(AActor* EnemyPawn = Cast<AActor>(BB->GetValueAsObject(Enemy.SelectedKeyName)))
		{
			FVector EnemyPawnLocation = EnemyPawn->GetActorLocation();
			BB->SetValueAsVector(LastSeenLocation.SelectedKeyName, EnemyPawnLocation);
		}
	}
}
