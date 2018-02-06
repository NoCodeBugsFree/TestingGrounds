// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "TP_ThirdPersonCharacter.generated.h"

// Animation BP depends on Mesh
// Look at different meshes in 3rd Person AnimBP and Hero AnimBP
// and change it

UCLASS(config=Game)
class ATP_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FP_Camera;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TP_Camera;

	// @API A component that spawns an Actor when registered, and destroys it when unregistered.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* ChildGunActor;

	// patrol route component contains all patrol points for AI
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UPatrolRoute* PatrolRoute;
	
	/** Pawn Noise Emitter Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UPawnNoiseEmitterComponent* PawnNoiseEmitterComponent;

public:
	
	/** returns the health percent CurrentHealth / InitialHealth  */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	float GetHealthPercent() const;

	/** Apply damage to this actor. */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	/** calls to make some noise during firing, landing, running etc. */
	UFUNCTION(BlueprintCallable, Category = "GamePlay")
	void Noise();

	/** calls to start fire  */
	UFUNCTION(BlueprintCallable, Category = "GamePlay")
	void FireStart();

	/** calls to stop fire  */
	UFUNCTION(BlueprintCallable, Category = "GamePlay")
	void FireStop();

	/** calls to fire AI single shot */
	UFUNCTION(BlueprintCallable, Category = "GamePlay")
	void FireAI();

protected:

	ATP_ThirdPersonCharacter();

	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/** make noise on landed  */
	virtual void Landed(const FHitResult& Hit) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** calls to toggle a crouch state  */
	void ToggleCrouch();

	/** calls to toggle a aiming state  */
	void ToggleAiming();

	/** calls to move to jumping  */
	void StartJump();

	/** calls to back from jumping  */
	void StopJump();

	/** calls to toggle FPS/TPS mode  */
	void ToggleFPS();
	
	/** calls to visualize received damage on HUD per hit */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void VisualizeDamage();
	
	// -----------------------------------------------------------------------------------

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", Meta = (BlueprintProtected = "true"))
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", Meta = (BlueprintProtected = "true"))
	float BaseLookUpRate;

	/** starting health for this character  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", Meta = (BlueprintProtected = "true"))
	float InitialHealth = 100.f;

	/** current health amount  */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config", Meta = (BlueprintProtected = "true"))
	float CurrentHealth;
	
private:

	/** shows whether character Shooting  now or not  */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	uint32 bShooting : 1;

	/** shows whether character Aiming  now or not  */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	uint32 bAiming : 1;

	/** shows whether character Crouching  now or not  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	uint32 bCrouching : 1;

	/** shows whether character Jumping  now or not  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	uint32 bJumping : 1;

	/** shows whether character Dead  now or not  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	uint32 bDead : 1;

	/** shows whether character is in FPS or in TPS mode */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	uint32 bFPS : 1;
	
public:

	/** returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return TP_Camera; }

	/** returns whether character Aiming  now or not **/
	FORCEINLINE bool IsAiming() const { return bAiming; }

	/** returns whether character Crouching  now or not **/
	FORCEINLINE bool IsCrouching() const { return bCrouching; }

	/** returns whether character Jumping  now or not **/
	FORCEINLINE bool IsJumping() const { return bJumping; }

	/** returns whether character is dead  now or not **/
	FORCEINLINE bool IsDead() const { return bDead; }

	/** returns whether player is in FPS or TPS mode  */
	FORCEINLINE bool IsInFPS() const { return bFPS; }

	/** returns current anim instance according FPS/TPS mode  */
	FORCEINLINE class UAnimInstance* GetCurrentAnimInstance() const { return bFPS ? Mesh1P->GetAnimInstance() : GetMesh()->GetAnimInstance(); }
};

