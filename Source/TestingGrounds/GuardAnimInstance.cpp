// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "GuardAnimInstance.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

void UGuardAnimInstance::NativeInitializeAnimation()
{
	if(ATP_ThirdPersonCharacter* TP_ThirdPersonCharacter = Cast<ATP_ThirdPersonCharacter>(TryGetPawnOwner()))
	{
		OwnerPawn = TP_ThirdPersonCharacter;
	}
}

void UGuardAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerPawn)
	{
		/** the velocity of the owner  */
		FVector Velocity = OwnerPawn->GetVelocity();

		/** owner's rotation  */
		FRotator BaseRotation = OwnerPawn->GetActorRotation();

		/** speed  */
		Speed = Velocity.Size();

		/** direction  */
		Direction = CalculateDirection(Velocity, BaseRotation);

		float Pitch = OwnerPawn->GetControlRotation().Pitch;

		/** direction  */
		AimAngle = Pitch > 180.f ? Pitch - 360.f : Pitch;

		/** jump  */
		// bEnableJump = OwnerPawn->IsJumping() && !bEnableJump ? true: false;
		bEnableJump = OwnerPawn->IsJumping();

		/** crouch  */
		bCrouching = OwnerPawn->IsCrouching();

		/** aiming  */
		bAiming = OwnerPawn->IsAiming();

		/** dead  */
		bDead = OwnerPawn->IsDead();
	}
}
