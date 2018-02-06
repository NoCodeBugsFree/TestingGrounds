// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "BTT_Shoot.h"
#include "AIController.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Shoot::UBTT_Shoot()
{
	Enemy.SelectedKeyName = "Enemy";
	NodeName = FString("S h o o t");
}

EBTNodeResult::Type UBTT_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		if(ATP_ThirdPersonCharacter* TP_ThirdPersonCharacter = Cast<ATP_ThirdPersonCharacter>(AIController->GetPawn()))
		{
			UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
			if (BB)
			{
				AActor* EnemyPawn = Cast<AActor>(BB->GetValueAsObject(Enemy.SelectedKeyName));
				AIController->SetFocus(EnemyPawn);
			}

			TP_ThirdPersonCharacter->FireAI();
		}
	}
	
	return EBTNodeResult::Succeeded;
}
