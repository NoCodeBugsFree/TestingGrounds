// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TestingGrounds.h"
#include "TP_ThirdPersonCharacter.h"
#include "ConstructorHelpers.h"
#include "PatrolRoute.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/ChildActorComponent.h"
#include "Gun.h"

//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter() 
{
	/** Pawn Noise Emitter Component  */
	PawnNoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComponent"));

	// Create a CameraComponent	
	FP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FP_Camera->SetupAttachment(GetCapsuleComponent());
	FP_Camera->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FP_Camera->bUsePawnControlRotation = true;
	FP_Camera->bAutoActivate = false;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FP_Camera);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	Mesh1P->bOnlyOwnerSee = true;

	/** Patrol Route  */
	PatrolRoute = CreateDefaultSubobject<UPatrolRoute>(TEXT("Patrol Route Comp"));

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(45.f, 100.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 270.f;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
	GetCharacterMovement()->NavAgentProps.AgentRadius = 45.f;
	GetCharacterMovement()->NavAgentProps.AgentHeight = 200.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// enable crouching
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true; 
	GetCharacterMovement()->CrouchedHalfHeight = 50.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 120.f; 
	

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	TP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	TP_Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	TP_Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	TP_Camera->bAutoActivate = true;

	/** post process settings  */
	TP_Camera->PostProcessBlendWeight = 0.f;
	TP_Camera->PostProcessSettings.bOverride_SceneColorTint = true;
	TP_Camera->PostProcessSettings.SceneColorTint = FLinearColor::Red;
	TP_Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
	TP_Camera->PostProcessSettings.VignetteIntensity = 1.f;
	TP_Camera->PostProcessSettings.bOverride_SceneFringeIntensity = true;
	TP_Camera->PostProcessSettings.SceneFringeIntensity = 3.f;
	
	/** post process settings  */
	FP_Camera->PostProcessBlendWeight = 0.f;
	FP_Camera->PostProcessSettings.bOverride_SceneColorTint = true;
	FP_Camera->PostProcessSettings.SceneColorTint = FLinearColor::Red;
	FP_Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
	FP_Camera->PostProcessSettings.VignetteIntensity = 1.f;
	FP_Camera->PostProcessSettings.bOverride_SceneFringeIntensity = true;
	FP_Camera->PostProcessSettings.SceneFringeIntensity = 3.f;
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	/** child gun actor  */
	ChildGunActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Gun"));
	ChildGunActor->SetupAttachment(GetMesh(), FName("GripPoint"));

#pragma region ConstructorHelpers

	/**  set gun bp template */
	static ConstructorHelpers::FClassFinder<AGun> GunTemplate(TEXT("/Game/Dynamic/BP/BP_Gun"));
	if (GunTemplate.Class != NULL)
	{
		ChildGunActor->SetChildActorClass(GunTemplate.Class);
	}

#pragma endregion

}

void ATP_ThirdPersonCharacter::BeginPlay()
{
	/** set owner to gun   */
	if(AGun* Gun = Cast<AGun>(ChildGunActor->GetChildActor()))
	{
		Gun->SetGunOwner(this);
	}
	
	/** init health   */
	CurrentHealth = InitialHealth;

	/** start as TPS  */
	FP_Camera->Deactivate();
	TP_Camera->Activate();
	Mesh1P->SetOwnerNoSee(true);
	GetMesh()->SetOwnerNoSee(false);

	Super::BeginPlay();
}

float ATP_ThirdPersonCharacter::GetHealthPercent() const
{
	return CurrentHealth / InitialHealth;
}

float ATP_ThirdPersonCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		/** HUD FX  */
		if (IsPlayerControlled())
		{
			VisualizeDamage();
		}

		CurrentHealth -= ActualDamage;
		// If the damage depletes our health
		if (CurrentHealth <= 0.f)
		{
			bDead = true;

			if (AGun* Gun = Cast<AGun>(ChildGunActor->GetChildActor()))
			{
				Gun->FireStop();
				bShooting = false;
				
				// disable movement
				GetCharacterMovement()->StopMovementImmediately();
				GetCharacterMovement()->DisableMovement();
				GetCharacterMovement()->SetComponentTickEnabled(false);
				
				// disable collisions on the capsule
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

				/** ragdoll  */
				if (bFPS)
				{
					SetActorEnableCollision(true);
					static FName CollisionProfileName(TEXT("Ragdoll"));

					Gun->GetGunMesh()->SetCollisionProfileName(CollisionProfileName);
					Gun->GetGunMesh()->SetAllBodiesSimulatePhysics(true);
					Gun->GetGunMesh()->SetSimulatePhysics(true);
					Gun->GetGunMesh()->WakeAllRigidBodies();
					Gun->GetGunMesh()->bBlendPhysics = true;

					Mesh1P->SetCollisionProfileName(CollisionProfileName);
					Mesh1P->SetAllBodiesSimulatePhysics(true);
					Mesh1P->SetSimulatePhysics(true);
					Mesh1P->WakeAllRigidBodies();
					Mesh1P->bBlendPhysics = true;
				}
			}

			/** Start Spectating  */
			if(APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				PC->StartSpectatingOnly();
			}
		}
	}

	return ActualDamage;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	/** jump  */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATP_ThirdPersonCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATP_ThirdPersonCharacter::StopJump);

	/** fire  */
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATP_ThirdPersonCharacter::FireStart);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATP_ThirdPersonCharacter::FireStop);

	/** toggle FPS / TPS  */
	PlayerInputComponent->BindAction("ToggleFPS", IE_Pressed, this, &ATP_ThirdPersonCharacter::ToggleFPS);

	/** crouch  */
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATP_ThirdPersonCharacter::ToggleCrouch);

	/** aim  */
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATP_ThirdPersonCharacter::ToggleAiming);

	/** movement   */
	PlayerInputComponent->BindAxis("MoveForward", this, &ATP_ThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATP_ThirdPersonCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATP_ThirdPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATP_ThirdPersonCharacter::LookUpAtRate);
}

void ATP_ThirdPersonCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	Noise();
}

void ATP_ThirdPersonCharacter::ToggleCrouch()
{
	bCrouching = !bCrouching;
	
	if (bCrouching)
	{
		Crouch(true);
	} 
	else
	{
		UnCrouch(true);
	}
}

void ATP_ThirdPersonCharacter::ToggleAiming()
{
	bAiming = !bAiming;

	if (bAiming)
	{
		TP_Camera->SetFieldOfView(30.f);
		FP_Camera->SetFieldOfView(30.f);
	} 
	else
	{
		TP_Camera->SetFieldOfView(90.f);
		FP_Camera->SetFieldOfView(90.f);
	}
}

void ATP_ThirdPersonCharacter::StartJump()
{
	if (bCrouching)
	{
		ToggleCrouch();
	}

	Jump();
	bJumping = true;
}

void ATP_ThirdPersonCharacter::StopJump()
{
	StopJumping();
	bJumping = false;
}

void ATP_ThirdPersonCharacter::ToggleFPS()
{
	bFPS = !bFPS;

	if (bFPS)
	{
		TP_Camera->Deactivate();
		FP_Camera->Activate();
		Mesh1P->SetOwnerNoSee(false);
		GetMesh()->SetOwnerNoSee(true);
		ChildGunActor->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GripPoint"));
	} 
	else
	{
		FP_Camera->Deactivate();
		TP_Camera->Activate();
		Mesh1P->SetOwnerNoSee(true);
		GetMesh()->SetOwnerNoSee(false);
		ChildGunActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GripPoint"));
	}
}

void ATP_ThirdPersonCharacter::FireStart()
{
	if (AGun* Gun = Cast<AGun>(ChildGunActor->GetChildActor()))
	{
		Gun->FireStart();
		bShooting = true;
	}
}

void ATP_ThirdPersonCharacter::FireStop()
{
	if (AGun* Gun = Cast<AGun>(ChildGunActor->GetChildActor()))
	{
		Gun->FireStop();
		bShooting = false;
	}
}

void ATP_ThirdPersonCharacter::FireAI()
{
	if (AGun* Gun = Cast<AGun>(ChildGunActor->GetChildActor()))
	{
		Gun->FireAI();
	}
}

void ATP_ThirdPersonCharacter::Noise()
{
	/** make noise  */
	PawnNoiseEmitterComponent->MakeNoise(this, 1.f, GetActorLocation());
}

void ATP_ThirdPersonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATP_ThirdPersonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATP_ThirdPersonCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATP_ThirdPersonCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

