// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

AEnemyAIController::AEnemyAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	
	/** set the behavior tree asset  */
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeTemplate(TEXT("/Game/Dynamic/AI/GuardBehavior"));
	if (BehaviorTreeTemplate.Object)
	{
		BehaviorTree = BehaviorTreeTemplate.Object;
	}
	
	/** AI Perception Component  */
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	/** sight sense config template to our AI  */
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	Sight->SightRadius = 2000.f;
	Sight->LoseSightRadius = 2500.f;
	Sight->PeripheralVisionAngleDegrees = 60.f;

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*Sight);
	
	/** hearing sense config template to our AI  */
	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	Hearing->HearingRange = 3000.f;
	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectFriendlies = true;
	Hearing->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*Hearing);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
}

void AEnemyAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	// Initialize Blackboard and Start Behavior Tree
	if (InPawn && BehaviorTree && BehaviorTree->BlackboardAsset)
	{
		BlackboardComponent->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
	
	/** set Enemy Tag to avoid attack each other */
	if(ATP_ThirdPersonCharacter* TP_ThirdPersonCharacter = Cast<ATP_ThirdPersonCharacter>(InPawn))
	{
		TP_ThirdPersonCharacter->Tags.Add(FName("Enemy"));
	}
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
}

/** TODO  */
void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor && !Actor->ActorHasTag(FName("Enemy")))
	{
		BlackboardComponent->SetValueAsObject(FName("Enemy"), Actor);
		// UE_LOG(LogTemp, Error, TEXT("Set"));
		return;
	}
	// BlackboardComponent->ClearValue(FName("Enemy"));
	// UE_LOG(LogTemp, Error, TEXT("Clear"));
}
