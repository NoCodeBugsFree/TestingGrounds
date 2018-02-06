// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "TestingGroundsGameMode.generated.h"

UCLASS(minimalapi)
class ATestingGroundsGameMode : public AGameMode
{
	GENERATED_BODY()

	/** Actor pool  component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UActorPool* ActorPool;

public:

	/** calls to find all NavMeshBoundsVolumes to store their references in the pool */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void PopulateBoundsVolume();

	/** calls to spawn new tile */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "AAA")
	void SpawnNewTile();

protected:

	ATestingGroundsGameMode();

	virtual void BeginPlay() override;
	
private:



	/** calls to spawn new tile */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "AAA")
	void SpawnInitialTiles(int32 TilesToSpawn);

	/** find all nav meshes in the world and add them to pool  */
	void AddToPool(class ANavMeshBoundsVolume* NewNavMeshBoundsVolume);

	/** next tile attach point  */
	UPROPERTY()
	FTransform NextTileTransform;

	/** tile template class  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ATile> TileTemplate;

};