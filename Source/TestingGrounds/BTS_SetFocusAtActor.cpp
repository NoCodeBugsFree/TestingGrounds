// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "BTS_SetFocusAtActor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_SetFocusAtActor::UBTS_SetFocusAtActor()
{
	Enemy.SelectedKeyName = "Enemy";
	NodeName = FString("Focus At Actor");
	Interval = 0.5f;
	RandomDeviation = 0.1f;
	/** call Tick event when task search enters this node (SearchStart will be called as well) */
	bCallTickOnSearchStart = true;
}

void UBTS_SetFocusAtActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController*  AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
		if (BB)
		{
			if (APawn* EnemyPawn = Cast<APawn>(BB->GetValueAsObject(Enemy.SelectedKeyName)))
			{
				AIController->SetFocus(EnemyPawn);
			}
		}
	}
}
