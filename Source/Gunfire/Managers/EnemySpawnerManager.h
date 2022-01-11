// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "EnemySpawnerManager.generated.h"
class UBoonEffectsBaseClass;
class UAttributesComponent;
class UDifficultyComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveCleared);
USTRUCT(BlueprintType)
struct FEnemySpawn {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText EnemyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBoonEffectsBaseClass*> powerups;
};
USTRUCT(BlueprintType)
struct FEnemyWave {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText WaveName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty = "EnemyName"))
	TArray<FEnemySpawn> enemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float enemiesRemainingToNextWave = 0;
};
UCLASS()
class GUNFIRE_API AEnemySpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnerManager();
	void SetupSpawnedEnemy(UAttributesComponent* enemyAttributes, size_t Index);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty = "WaveName"))
		TArray<FEnemyWave> Waves;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int currentWave = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* BaseSpawnPoint;
	UFUNCTION(BlueprintImplementableEvent, Category = Spawn)
		void OnWaveClear();
	UFUNCTION(BlueprintImplementableEvent, Category = Spawn)
		void OnSpawnerCleared();
	UFUNCTION(BlueprintImplementableEvent, Category = Spawn)
		void OnEnemySpawned();
	UFUNCTION(BlueprintImplementableEvent, Category = Kill)
		void OnEnemyDeath();
	UPROPERTY()
		FOnWaveCleared OnSpawnerClearedDelegate;
	UPROPERTY(BlueprintAssignable)
		FOnWaveCleared OnEnemyDeathDelegate;
	UFUNCTION(BlueprintCallable)
		void SpawnWave();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int CurrentActiveEnemies() { return ActiveEnemies; };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray< UBillboardComponent*> SpawnPoints;
	TArray<FVector> SpawnPositions;
	UPROPERTY()
	int ActiveEnemies = 0;
	UFUNCTION()
	void RemoveEnemy(float overkill);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool RandomSpawn;
	UDifficultyComponent* difficulty;
public:	

};
