// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "Tile.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInterface.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "TestingGroundsGameMode.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

#define ECC_Spawn ECC_GameTraceChannel2

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/** Root */
	ROOT = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SetRootComponent(ROOT);

	/** Wall 01  */
	Wall01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall01"));
	Wall01->SetupAttachment(RootComponent);
	Wall01->SetRelativeScale3D(FVector(1.f, 40.f, 6.f));
	Wall01->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	Wall01->SetRelativeLocation(FVector(2000.f, -1950.f, 0.f));

	/** Wall 02 */
	Wall02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall02"));
	Wall02->SetupAttachment(RootComponent);
	Wall02->SetRelativeScale3D(FVector(1.f, 40.f, 6.f));
	Wall02->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	Wall02->SetRelativeLocation(FVector(2000.f, 1950.f, 0.f));

	/** floor mesh  */
	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(RootComponent);
	Floor->SetRelativeLocation(FVector(2000.f, 0.f, 0.f));
	Floor->SetCollisionProfileName("NonSpawnBlocking");

	/** barrier mesh  */
	Barrier = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrier"));
	Barrier->SetupAttachment(RootComponent);
	Barrier->SetRelativeScale3D(FVector(40.f, 1.f, 6.f));
	Barrier->SetRelativeLocation(FVector(4000.f, 0.f, 0.f));
	Barrier->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	/** Attach Point  */
	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point"));
	AttachPoint->SetupAttachment(RootComponent);
	AttachPoint->SetRelativeLocation(FVector(4000.f, 0.f, 0.f));

	/** Lock Collision  */
	LockBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Lock Collision"));
	LockBoxCollision->SetupAttachment(RootComponent);
	LockBoxCollision->SetBoxExtent(FVector(40.f, 40.f, 10.f));
	LockBoxCollision->SetRelativeScale3D(FVector(3.f, 70.f, 20.f));
	LockBoxCollision->SetRelativeLocation(FVector(4250.f, 0.f, 100.f));
	LockBoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	LockBoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	/**  Clean Collision */
	CleanBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Clean Collision"));
	CleanBoxCollision->SetupAttachment(RootComponent);
	CleanBoxCollision->SetBoxExtent(FVector(40.f, 40.f, 10.f));
	CleanBoxCollision->SetRelativeScale3D(FVector(3.f, 70.f, 20.f));
	CleanBoxCollision->SetRelativeLocation(FVector(8550.f, 0.f, 100.f));
	CleanBoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CleanBoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

#pragma region ConstructorHelpers

	/**  set the default static mesh for walls */
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BaseMaterial(TEXT("/Game/Environment/MI_BlockoutSimple_Inst"));
	if (BaseMaterial.Succeeded())
	{
		Wall01->SetMaterial(0, BaseMaterial.Object);
		Wall02->SetMaterial(0, BaseMaterial.Object);
	}

	/**  set the default static mesh for walls */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallTemplate(TEXT("/Game/Static/Geometry/Meshes/1M_Cube"));
	if (WallTemplate.Succeeded())
	{
		Wall01->SetStaticMesh(WallTemplate.Object);
		Wall02->SetStaticMesh(WallTemplate.Object);
		Barrier->SetStaticMesh(WallTemplate.Object);
	}

	/**  set the default static mesh for floor */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FloorTemplate(TEXT("/Game/Environment/SM_Ground"));
	if (FloorTemplate.Succeeded())
	{
		Floor->SetStaticMesh(FloorTemplate.Object);
	}

	/**  set red material */
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> RedMaterialTemplate(TEXT("/Game/Dynamic/Terrain/M_Red"));
	if (RedMaterialTemplate.Succeeded())
	{
		RedMaterial = RedMaterialTemplate.Object;
	}

	/**  set green material */
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> GreenMaterialTemplate(TEXT("/Game/Dynamic/Terrain/M_Green"));
	if (GreenMaterialTemplate.Succeeded())
	{
		GreenMaterial = GreenMaterialTemplate.Object;
	}

	/**  AI Pawn */
	static ConstructorHelpers::FClassFinder<APawn> AIPawnBPClass(TEXT("/Game/Dynamic/BP/BP_TP_Character"));
	if (AIPawnBPClass.Class != NULL)
	{
		AI_PawnTemplate = AIPawnBPClass.Class;
	}

#pragma endregion

#pragma region Grass

	///**  grass01 component */
	//Grass01 = CreateDefaultSubobject<UGrassComponent>("Grass01");
	//Grass01->SetupAttachment(RootComponent);
	//Grass01->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

	///**  find grass 01 asset */
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> GrassMesh01Template(TEXT("/Game/TestingGroundPack/GrassPack/Meshes/grass01"));
	//if (GrassMesh01Template.Succeeded())
	//{
	//	Grass01->SetStaticMesh(GrassMesh01Template.Object);
	//}

	///**  grass01 component */
	//Grass02 = CreateDefaultSubobject<UGrassComponent>("Grass02");
	//Grass02->SetupAttachment(RootComponent);
	//Grass02->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

	///**  find grass 02 asset */
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> GrassMesh02Template(TEXT("/Game/TestingGroundPack/GrassPack/Meshes/grass02"));
	//if (GrassMesh02Template.Succeeded())
	//{
	//	Grass02->SetStaticMesh(GrassMesh02Template.Object);
	//}

	///**  grass01 component */
	//Grass03 = CreateDefaultSubobject<UGrassComponent>("Grass03");
	//Grass03->SetupAttachment(RootComponent);
	//Grass03->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

	///**  find grass 03 asset */
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> GrassMesh03Template(TEXT("/Game/TestingGroundPack/GrassPack/Meshes/grass05"));
	//if (GrassMesh03Template.Succeeded())
	//{
	//	Grass03->SetStaticMesh(GrassMesh03Template.Object);
	//}


#pragma endregion
	
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	Lock();

	PlaceActors();
	
	PlaceAI();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (ActorPoolReference)
	{
		ActorPoolReference->Return(NavMeshBoundsVolume);
	}
}

void ATile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	LockBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATile::OnLockCollisionOverlapBegin);
	CleanBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATile::OnCleanCollisionOverlapBegin);
}

void ATile::OnLockCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	/** if character overlaps me - lock previews sector and spawn next  */
	if (ATP_ThirdPersonCharacter* TP_ThirdPersonCharacter = Cast<ATP_ThirdPersonCharacter>(OtherActor))
	{
		/** AI doesn't trigger anything  */
		if (TP_ThirdPersonCharacter->IsPlayerControlled())
		{
			/** lock once - disable trigger collision  */
			LockBoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			/* lock the previous sector */
			Lock();
		}
	}
}

void ATile::OnCleanCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	/** if character overlaps me - destroy previous sector  */
	if(ATP_ThirdPersonCharacter* TP_ThirdPersonCharacter = Cast<ATP_ThirdPersonCharacter>(OtherActor))
	{
		/** AI doesn't trigger anything  */
		if (TP_ThirdPersonCharacter->IsPlayerControlled())
		{
			/** clean once - disable trigger collision  */
			CleanBoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			/** spawn new tile  */
			if (ATestingGroundsGameMode* TestingGroundsGameMode = Cast<ATestingGroundsGameMode>(GetWorld()->GetAuthGameMode()))
			{
				TestingGroundsGameMode->SpawnNewTile();
			}

			// destroy all spawned actors
			for (AActor* Child : SpawnedActors)
			{
				if (Child && !Child->IsPendingKillPending())
				{
					Child->SetLifeSpan(0.01f);
				}
			}

			SetLifeSpan(0.01f);
		}
	}
}

void ATile::Lock()
{
	Barrier->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Barrier->SetMaterial(0, RedMaterial);
}

void ATile::Unlock()
{
	Barrier->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Barrier->SetMaterial(0, GreenMaterial);
}

bool ATile::CanSpawnAtLocation(FVector Location)
{
	FHitResult OutHit;
	FVector Start = Location + FVector(0.f, 0.f, 100.f);
	FVector End = Location;
	
	bool bHasHitSomethings = GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_Spawn, FCollisionShape::MakeSphere(Radius));
		
	return bHasHitSomethings;
}

void ATile::SetActorPoolRef(class UActorPool* PoolToSet)
{
	if (!ensure(PoolToSet)) { return; }

	ActorPoolReference = PoolToSet;

	/** position nav mesh bounds volume  */
	NavMeshBoundsVolume = ActorPoolReference->CheckOut();
	if (NavMeshBoundsVolume)
	{
		NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavMeshOffset);
	
		if (GetWorld())
		{
			if (GetWorld()->GetNavigationSystem())
			{
				GetWorld()->GetNavigationSystem()->Build();
			}
		}
	}
}

void ATile::PlaceActors(TSubclassOf<class AActor> ActorToSpawn /*= nullptr*/, int32 MinSpawn /*= 1*/, int32 MaxSpawn /*= 1*/, float SpawnRadius /*= 500.f*/)
{
	if (ActorsToSpawn.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Set up actors to spawn!"));
		return;
	}

	int32 ActorsToSpawnAmount = FMath::RandRange(MinActorsToSpawn, MaxActorsToSpawn);
	for (int32 i = 0; i < ActorsToSpawnAmount; i++)
	{
		FBox Box(MinExtent, MaxExtent);

		for (int32 j = 0; j < 10; j++) // ten attempts
		{
			FVector SpawnLocation = FMath::RandPointInBox(Box);

			if (CanSpawnAtLocation(SpawnLocation))
			{
				int32 RandomIndex = FMath::RandRange(0, ActorsToSpawn.Num() - 1);
				if (ActorsToSpawn.IsValidIndex(RandomIndex))
				{
					TSubclassOf<AActor> ActorTypeToSpawn = ActorsToSpawn[RandomIndex];

					if (ActorTypeToSpawn)
					{
						UWorld* const World = GetWorld();
						if (World)
						{
							FActorSpawnParameters SpawnParams;
							SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
							AActor* SpawnedActor = World->SpawnActor<AActor>(ActorTypeToSpawn);
							if (SpawnedActor)
							{
								/** add new actor to spawned list for further garbage collection */
								SpawnedActors.Add(SpawnedActor);

								/** set random rotation  */
								FRotator SpawnedActorRotation = SpawnedActor->GetActorRotation();
								SpawnedActorRotation.Yaw += FMath::FRand() * 360.f;
								SpawnedActor->SetActorRotation(SpawnedActorRotation);

								/** set location  */
								SpawnedActor->SetActorRelativeLocation(SpawnLocation);
								SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
								
								break;
							}
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("ActorTypeToSpawn == NULL"));
					}
				}
			}
		}
	}
}

void ATile::PlaceAI(TSubclassOf<class APawn> AIPawnToSpawn /*= nullptr*/, int32 MinSpawn /*= 1*/, int32 MaxSpawn /*= 1*/, float SpawnRadius /*= 500.f*/)
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			
		FVector SpawnLocation = GetActorLocation() + FVector(2000.f, 0.f, 300.f);
		FRotator SpawnRotation = UKismetMathLibrary::RandomRotator();
		SpawnRotation.Pitch = 0.f;

		/** future AI  */
		APawn* SpawnedPawn = nullptr;

		/** if overrides default AI to spawn class  */
		if (AIPawnToSpawn)
		{
			SpawnedPawn = World->SpawnActor<APawn>(AIPawnToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		}
		else if (AI_PawnTemplate) /** spawn default AI class  */
		{
			SpawnedPawn = World->SpawnActor<APawn>(AI_PawnTemplate, SpawnLocation, SpawnRotation, SpawnParams);
		}
		
		/** spawn AI controller for AI pawn  */
		if (SpawnedPawn)
		{
			SpawnedPawn->SpawnDefaultController();
		}
	}
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int32 MinSpawn /*= 1*/, int32 MaxSpawn /*= 1*/, float SpawnRadius /*= 500.f*/)
{
	
}
