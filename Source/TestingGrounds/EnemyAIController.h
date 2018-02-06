// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Perception/AIPerceptionTypes.h" 
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	/** AI Perception Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent;

	/** sight sense config template to our AI  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight* Sight;

	/** hearing sense config template to our AI  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Hearing* Hearing;
	
protected:

	AEnemyAIController();

	virtual void Possess(APawn* InPawn) override;
	
	/** calls when AI Perception Component updated */
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:

	/** Behavior Tree  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;
	
	/** Blackboard Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;
	
	/** BehaviorTree Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;
	
};
