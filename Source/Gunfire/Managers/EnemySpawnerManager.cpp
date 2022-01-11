// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnerManager.h"
#include "../Character/AttributesComponent.h"
#include "../DataAssets/DOTEffect.h"
#include "../Player/DifficultyComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveFloat.h"

// Sets default values
AEnemySpawnerManager::AEnemySpawnerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BaseSpawnPoint = CreateDefaultSubobject<UBillboardComponent>(TEXT("BaseSpawnPoint"));
	RootComponent = BaseSpawnPoint;

}

void AEnemySpawnerManager::SetupSpawnedEnemy(UAttributesComponent* enemyAttributes, size_t Index)
{
	enemyAttributes->OnDeath.AddDynamic(this, &AEnemySpawnerManager::RemoveEnemy);
	OnEnemySpawned();
	int ClearedRooms = UDifficultyComponent::ClearedRooms();
	if (IsValid(difficulty->GetDifficultyHealth())) {
		int currentHealth = enemyAttributes->GetCurrentHealth();
		UCurveFloat* DifficultyHealth = difficulty->GetDifficultyHealth();
		enemyAttributes->SetHealth(currentHealth *
			DifficultyHealth->GetFloatValue(ClearedRooms));
	}
	if (IsValid(difficulty->GetDifficultyDamage())) {
		int currentDamage = enemyAttributes->DamageAmount();
		
		UCurveFloat* DifficultyDamage = difficulty->GetDifficultyDamage();
		enemyAttributes->SetDamage(currentDamage *
			DifficultyDamage->GetFloatValue(ClearedRooms));
	}

	for (size_t g = 0; g < Waves[currentWave].enemies[Index].powerups.Num(); g++)
	{
		enemyAttributes->AddBoon(Waves[currentWave].enemies[Index].powerups[Index]);
	}
}

void AEnemySpawnerManager::SpawnWave()
{
	int enemyTypes = Waves[currentWave].enemies.Num();
	int spawnIndex = 0;
	GetComponents<UBillboardComponent>(SpawnPoints);
	difficulty = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UDifficultyComponent>();
	for (size_t i = 0; i < enemyTypes; i++)
	{
		int enemies = Waves[currentWave].enemies[i].amount;
		for (size_t a = 0; a < enemies; a++)
		{
			FVector SpawnPos;
			if (RandomSpawn) {
				SpawnPos = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)]->GetComponentLocation();
			}
			else {
				SpawnPos = SpawnPoints[spawnIndex]->GetComponentLocation();
				spawnIndex++;
				if (spawnIndex >= SpawnPoints.Num()) {
					spawnIndex = 0;
				}
			}
			FActorSpawnParameters spawnParam;
			spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AActor* enemy = GetWorld()->SpawnActor<AActor>(Waves[currentWave].enemies[i].enemy, SpawnPos, GetActorRotation(), spawnParam);
			UAttributesComponent* enemyAttributes = nullptr;
			if (enemy) {
				 enemyAttributes = enemy->FindComponentByClass<UAttributesComponent>();
			}
			
			if (enemyAttributes) {
				ActiveEnemies++;

				SetupSpawnedEnemy(enemyAttributes, i);
			}
		}
	}
	
}

// Called when the game starts or when spawned
void AEnemySpawnerManager::BeginPlay()
{
	 
	Super::BeginPlay();
	
	
	
}

void AEnemySpawnerManager::RemoveEnemy(float overkill)
{
	if (ActiveEnemies != 0) {
		ActiveEnemies--;
		
		if (ActiveEnemies <= Waves[currentWave].enemiesRemainingToNextWave) {
			currentWave++;
			OnWaveClear();
			if (currentWave >= Waves.Num()) {
				currentWave = 0;
				ActiveEnemies = 0;
				OnSpawnerClearedDelegate.Broadcast();
				OnSpawnerCleared();
				return;
			}
			else {
				
				SpawnWave();
			}

		}
		OnEnemyDeathDelegate.Broadcast();
		OnEnemyDeath();
	}
	
}


