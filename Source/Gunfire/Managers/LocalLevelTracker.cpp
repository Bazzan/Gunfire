// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalLevelTracker.h"
#include "../WorldItems/Door.h"
#include "Kismet/GameplayStatics.h"
#include "../Boons/BoonPickup.h"
#include "../Character/AttributesComponent.h"
#include "../Enemies/EnemyInterface.h"
#include "EnemySpawnerManager.h"
#include "BoonSpawnerManager.h"
// Sets default values
ALocalLevelTracker::ALocalLevelTracker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Boon1Spawn = CreateDefaultSubobject<UBillboardComponent>(TEXT("First Boon Spawn"));
	Boon2Spawn = CreateDefaultSubobject<UBillboardComponent>(TEXT("Second Boon Spawn"));
	Boon2Spawn->SetupAttachment(Boon1Spawn);
	Boon3Spawn = CreateDefaultSubobject<UBillboardComponent>(TEXT("Third Boon Spawn"));
	Boon3Spawn->SetupAttachment(Boon1Spawn);
}

void ALocalLevelTracker::FinishRoom()
{

	BoonPositions.Empty();
	BoonPositions.Add(Boon1Spawn->GetComponentLocation());
	BoonPositions.Add(Boon2Spawn->GetComponentLocation());
	//Fix this thingy spawning the third
	BoonPositions.Add(Boon3Spawn->GetComponentLocation());
	
	TArray<ABoonPickup*> boons = BoonSpawner->SpawnBoons(BoonPositions, GetActorRotation());
	for (size_t i = 0; i < boons.Num(); i++)
	{
		if (boons[i]) {
			boons[i]->OnPickup.AddDynamic(this, &ALocalLevelTracker::RemoveOtherBoons);
		}
	}
	if (LastDoor) {
		LastDoor->OpenDoor();
	}
	OnRoomFinished.Broadcast();
	OnRoomFinishedEvent();
	
}

void ALocalLevelTracker::ResetRoom()
{
	LastDoor->CloseDoor();
	
	for (size_t i = 0; i < levelDoors.Num(); i++)
	{
		
		if (levelDoors[i]) {
			levelDoors[i]->CloseDoor();
		}
	}
	for (size_t i = 0; i < InitialSpawners.Num(); i++)
	{
		InitialSpawners[i]->SpawnWave();
	}
}


void ALocalLevelTracker::RemoveOtherBoons(AActor* pickedUpBoon)
{
	for (size_t i = 0; i < SpawnedBoons.Num(); i++)
	{
		if (SpawnedBoons[i] != pickedUpBoon) {
			SpawnedBoons[i]->Destroy();
		}
	}
	BoonSpawner->UnlockBoons();
}

// Called when the game starts or when spawned
void ALocalLevelTracker::BeginPlay()
{
	if (LastSpawner) {
		LastSpawner->OnSpawnerClearedDelegate.AddDynamic(this, &ALocalLevelTracker::FinishRoom);
	}
	
	ResetRoom();
	AActor* spawner = UGameplayStatics::GetActorOfClass(GetWorld(), ABoonSpawnerManager::StaticClass());
	BoonSpawner = Cast< ABoonSpawnerManager>(spawner);
	Super::BeginPlay();
}

void ALocalLevelTracker::RemoveEnemy(float overkill)
{
	enemiesRemaining--;
	if (enemiesRemaining == 0) {
		FinishRoom();
	}
}


