// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TestingGrounds.h"
#include "TestingGroundsGameMode.h"
#include "TestingGroundsHUD.h"
#include "Tile.h"
#include "ConstructorHelpers.h"
#include "EngineUtils.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "ActorPool.h"

ATestingGroundsGameMode::ATestingGroundsGameMode()
{
	/** set tile template class  */
	static ConstructorHelpers::FClassFinder<ATile> TileTemplateClass(TEXT("/Game/Dynamic/BP/BP_Tile"));
	if (TileTemplateClass.Class)
	{
		TileTemplate = TileTemplateClass.Class;
	}

	ActorPool = CreateDefaultSubobject<UActorPool>(TEXT("ActorPool"));
}

void ATestingGroundsGameMode::BeginPlay()
{
	PopulateBoundsVolume();

	/** spawn initial tiles  */
	SpawnInitialTiles(3);
	
	Super::BeginPlay();
}


void ATestingGroundsGameMode::SpawnInitialTiles(int32 TilesToSpawn)
{
	if (TilesToSpawn < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("TilesToSpawn < 1"));
		return;
	}
	for (int32 i = 0; i < TilesToSpawn; i++)
	{
		SpawnNewTile();
	}
}

void ATestingGroundsGameMode::AddToPool(class ANavMeshBoundsVolume* NewNavMeshBoundsVolume)
{
	if (!ensure(NewNavMeshBoundsVolume)) { return; }

	ActorPool->Add(NewNavMeshBoundsVolume);
}

void ATestingGroundsGameMode::PopulateBoundsVolume()
{
	/** find all nav meshes in the world and add them to pool  */
	for (TActorIterator<ANavMeshBoundsVolume> Iter(GetWorld()); Iter; ++Iter)
	{
		if ((*Iter))
		{
			AddToPool(*Iter);
		}
	}
}

void ATestingGroundsGameMode::SpawnNewTile()
{
	if (TileTemplate)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			ATile* SpawnedTile = World->SpawnActor<ATile>(TileTemplate, NextTileTransform, SpawnParams);
			if (SpawnedTile)
			{
				SpawnedTile->SetActorPoolRef(ActorPool);
				NextTileTransform = SpawnedTile->GetAttachTransform();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TileTemplate == NULL"));
	}
}
