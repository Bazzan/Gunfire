// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "LocalLevelTracker.generated.h"
class ADoor;
class ABoonSpawnerManager;
class AEnemySpawnerManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoomFinished);
UCLASS()
class GUNFIRE_API ALocalLevelTracker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALocalLevelTracker();
	UFUNCTION(BlueprintCallable)
		void FinishRoom();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int RemainingEnemies() { return enemiesRemaining; }
	UFUNCTION(BlueprintCallable)
		void ResetRoom();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UBillboardComponent* Boon1Spawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UBillboardComponent* Boon2Spawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UBillboardComponent* Boon3Spawn;
	UPROPERTY(BlueprintAssignable)
		FOnRoomFinished OnRoomFinished;
	UFUNCTION(BlueprintImplementableEvent)
		void OnRoomFinishedEvent();
protected:
	// Called when the game starts or when spawned
	UFUNCTION()
		void RemoveOtherBoons(AActor* pickedUpBoon);
	TArray<AActor*> SpawnedBoons;
	TArray<FVector> BoonPositions;
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear)
	ADoor* LastDoor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear)
		AEnemySpawnerManager* LastSpawner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AEnemySpawnerManager*> InitialSpawners;
	UPROPERTY()
		ABoonSpawnerManager* BoonSpawner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ADoor*> levelDoors;
		UPROPERTY()
			int enemiesRemaining;
		UFUNCTION()
			void RemoveEnemy(float overkill);
public:	
	// Called every frame

};
