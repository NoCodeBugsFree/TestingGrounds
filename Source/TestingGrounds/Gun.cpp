// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "Gun.h"
#include "ConstructorHelpers.h"
#include "TestingGroundsProjectile.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Animation/AnimInstance.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	/** gum mesh  */
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(GunMesh);
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

#pragma region ConstructorHelpers

	/**  Projectile BP Class */
	static ConstructorHelpers::FClassFinder<ATestingGroundsProjectile> ProjectileClassTemplate(TEXT("/Game/Dynamic/BP/BP_Projectile"));
	if (ProjectileClassTemplate.Class != NULL)
	{
		ProjectileClass = ProjectileClassTemplate.Class;
	}

	/** Fire Sound  */
	static ConstructorHelpers::FObjectFinder<USoundBase> FireSoundTemplate(TEXT("/Game/Static/Player/FirstPerson/Audio/FirstPersonTemplateWeaponFire02_Cue"));
	if (FireSoundTemplate.Object)
	{
		FireSound = FireSoundTemplate.Object;
	}

	/** set the FP fire animation template  */
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FP_FireAnimationTemplate(TEXT("/Game/Dynamic/BP/Animations/AM_FPS_Fire"));
	if (FP_FireAnimationTemplate.Succeeded())
	{
		FP_FireAnimation = FP_FireAnimationTemplate.Object;
	}

	/** set the TP fire animation template  */
	static ConstructorHelpers::FObjectFinder<UAnimMontage> TP_FireAnimationTemplate(TEXT("/Game/Dynamic/AnimStarterPack/AM_TPFire"));
	if (TP_FireAnimationTemplate.Succeeded())
	{
		TP_FireAnimation = TP_FireAnimationTemplate.Object;
	}

#pragma endregion

}

void AGun::FireStart()
{
	bool bTimerActive = GetWorldTimerManager().IsTimerActive(TimerFire);
	if (bTimerActive)
	{
		return;
	}

	/** Set Timer */
	GetWorldTimerManager().SetTimer(TimerFire, this, &AGun::Fire, FireDelay, true, 0.f);
}

void AGun::FireStop()
{
	GetWorldTimerManager().ClearTimer(TimerFire);
}

void AGun::SetGunOwner(class ATP_ThirdPersonCharacter* NewOwner)
{
	GunOwner = NewOwner;
	SetOwner(NewOwner);
}

void AGun::Fire()
{
	// try to spawn a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<ATestingGroundsProjectile>(ProjectileClass, GetMuzzleTransform());
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	/** play montage according current anim instance */
	if (GunOwner)
	{
		bool bOwnerInFPS = GunOwner->IsInFPS();
		UAnimInstance* AnimInstance = GunOwner->GetCurrentAnimInstance();

		if (bOwnerInFPS)
		{
			/** try and play the animation if specified  */
			if (FP_FireAnimation)
			{
				if (AnimInstance != NULL)
				{
					AnimInstance->Montage_Play(FP_FireAnimation, 1.f);
				}
			}
		}
		else
		{
			/** try and play the animation if specified  */
			if (TP_FireAnimation)
			{
				if (AnimInstance != NULL)
				{
					AnimInstance->Montage_Play(TP_FireAnimation, 1.f);
				}
			}
		}
	}
		
	

	/** human makes noise  */
	if (GunOwner && GunOwner->IsPlayerControlled())
	{
		GunOwner->Noise();
	}
}

void AGun::FireAI()
{
	Fire();
}
