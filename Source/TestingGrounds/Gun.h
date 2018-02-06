// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class TESTINGGROUNDS_API AGun : public AActor
{
	GENERATED_BODY()
	
	/* represents gun mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* GunMesh;
	
	/** projectile spawn point */
	UPROPERTY(EditDefaultsOnly, Category = "Conifg", meta = (AllowPrivateAccess = "true"))
	FName MuzzleSocketName = FName("Muzzle");

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conifg", meta = (AllowPrivateAccess = "true"))
	class USoundBase* FireSound;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Conifg", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ATestingGroundsProjectile> ProjectileClass;

	/** AnimMontage to play each time we fire for FP mesh */
	UPROPERTY(EditDefaultsOnly, Category = "Conifg", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FP_FireAnimation;

	/** AnimMontage to play each time we fire for TP mesh */
	UPROPERTY(EditDefaultsOnly, Category = "Conifg", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* TP_FireAnimation;

	/** owner reference  */
	UPROPERTY(Transient)
	class ATP_ThirdPersonCharacter* GunOwner;

public:	
	
	/** calls to start fire a weapon by timer */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void FireStart();

	/** calls to stoop fire a weapon by timer */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void FireStop();

	/** calls to set an owner to this Gun */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetGunOwner(class ATP_ThirdPersonCharacter* NewOwner);

	/** calls to fire AI single shot */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void FireAI();

protected:

	// Sets default values for this actor's properties
	AGun();

private:

	/** calls to fire a single shot */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Fire();

	/** delay between shots */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float FireDelay = 0.2f;

	/** fire timer  */
	UPROPERTY()
	FTimerHandle TimerFire;


public:	
	/** retunrs the transform of the muzzle socket **/
	FORCEINLINE FTransform GetMuzzleTransform() const { return GunMesh->GetSocketTransform(MuzzleSocketName); }
	/** returns weapon gun mesh **/
	FORCEINLINE class USkeletalMeshComponent* GetGunMesh() const { return GunMesh; }
	
};
