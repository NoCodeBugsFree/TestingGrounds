// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

UChooseNextWaypoint::UChooseNextWaypoint()
{
	IndexKey.SelectedKeyName = "NextWaypointIndex";
	WayPointKey.SelectedKeyName = "WayPoint";
	NodeName = FString("Choose Next Waypoint");
}

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	// Get Patrol Route
	if(AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		APawn* ControlledPawn = AIController->GetPawn();

		if (ControlledPawn)
		{
			UPatrolRoute* PatrolRouteComp = ControlledPawn->FindComponentByClass<UPatrolRoute>();

			if (!ensure(PatrolRouteComp)) { return EBTNodeResult::Failed; }

			// Get Patrol Points , WARN about empty patrol route
			TArray<AActor*> PatrolPoints = PatrolRouteComp->GetPatrolPoints();
			if (PatrolPoints.Num() == 0)
			{
				UE_LOG(LogTemp, Error, TEXT("A Guard is missing Patrol Points!"));
				return EBTNodeResult::Failed;
			}

			// ----- Set Next Way Point -------------------------------------

			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

			if (BlackboardComp)
			{
				/** get current index from BB  */
				int32 Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

				/** set next way point  */
				BlackboardComp->SetValueAsObject(WayPointKey.SelectedKeyName, PatrolPoints[Index]);

				/** Cycle Index to further update  */
				Index++;
				Index %= PatrolPoints.Num(); // Cycle itself

				/** safe found index in BB */
				BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, Index);

				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}