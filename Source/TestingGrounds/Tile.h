// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Tile.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FVector SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float Scale = 1.f;
};

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
	/* root component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ROOT;

	/** wall mesh 01  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Wall01;
	
	/** wall mesh 02  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Wall02;

	/** floor mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Floor;

	/** barrier mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Barrier;
	
	/*  next tile spawn point */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* AttachPoint;

	/*  Lock Collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LockBoxCollision;
	
	/* Clean Collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CleanBoxCollision;
	
	/* red material */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface* RedMaterial;

	/* red material */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface* GreenMaterial;
	
	// --------- Crash the editor after each compiling  ----------------------------------------

	///** grass01 component  */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	//class UGrassComponent* Grass01;

	///** grass02 component  */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	//class UGrassComponent* Grass02;

	///** grass03 component  */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	//class UGrassComponent* Grass03;

public:	

	/** calls to set actor pool reference  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetActorPoolRef(class UActorPool* PoolToSet);
	
	/** returns attach transform for next tile to spawn */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AAA")
	FTransform GetAttachTransform() { return AttachPoint->GetComponentTransform(); }
	
protected:

	// Sets default values for this actor's properties
	ATile();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PostInitializeComponents() override;

	/** calls to lock previous sector  */
	UFUNCTION()
	void OnLockCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** calls to delete previous tile  */
	UFUNCTION()
	void OnCleanCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** calls to lock previous sector */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Lock();

	/** calls to unlock next sector */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Unlock();

private:

	/** calls to randomly place actors or pawns  */
	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, float SpawnRadius = 500.f);

	/** calls to test whether we can spawn at this location or not  */
	bool CanSpawnAtLocation(FVector Location);

	/** calls to generate some actors at tile  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void PlaceActors(TSubclassOf<class AActor> ActorToSpawn = nullptr, int32 MinSpawn = 1, int32 MaxSpawn = 1, float SpawnRadius = 500.f);

	/** calls to spawn AI  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void PlaceAI(TSubclassOf<class APawn> AIPawnToSpawn = nullptr, int32 MinSpawn = 1, int32 MaxSpawn = 1, float SpawnRadius = 500.f);
	
	/** AI pawn template to spawn on this tile  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APawn> AI_PawnTemplate;

	/** offset for valid Nav Mesh replacement - center of the tile */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector NavMeshOffset = FVector(2000.f, 0.f, 0.f);

	/** min tile extent  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector MinExtent = FVector(0.f, -2000.f, 0.f);

	/**  max tile extent */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector MaxExtent = FVector(4000.f, 2000.f, 0.f);

	/** reference to level Nav Mesh  */
	UPROPERTY()
	AActor* NavMeshBoundsVolume;

	/** all spawned actors on this tile   */
	UPROPERTY()
	TSet<class AActor*> SpawnedActors;

	/** actor pool's reference  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UActorPool* ActorPoolReference;

	/** radius to check if there is an obstacle to spawn a prop */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float Radius = 500.f;

	/** the list of actors to spawn on this tile  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class AActor>> ActorsToSpawn;
	
	/** minimum amount of actors to spawn on this tile  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 MinActorsToSpawn = 3;

	/** maximum amount of actors to spawn on this tile  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32  MaxActorsToSpawn = 10;

};
