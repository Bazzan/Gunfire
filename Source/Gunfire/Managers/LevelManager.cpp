// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelManager::ALevelManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	BannedLevelNames.SetNum(3);
}

void ALevelManager::LoadNewLevel(int32 NewLevelIndex)
{
	FLatentActionInfo LatentActionInfo;

	UGameplayStatics::LoadStreamLevel(GetWorld(), LevelNames[NewLevelIndex], true, false, LatentActionInfo);
	//UE_LOG(LogTemp, Warning, TEXT("Loading level %s"), *LevelNames[NewLevelIndex].ToString());

	if(!CurrentLevelName.IsNone() && CurrentLevelName != LevelNames[NewLevelIndex])
	{
		//UE_LOG(LogTemp, Warning, TEXT("Unloading level %s"), *CurrentLevelName.ToString());
		UnloadLevel(CurrentLevelName);
	}

		CurrentLevelName = LevelNames[NewLevelIndex];
	ExitHasSpawned = false;

}

void ALevelManager::SpawnExit(FTransform SpawnPoint)
{
	if (CorridorTypes.Max() > 0 && CorridorTypes[0] != nullptr && !ExitHasSpawned)
	{
		UWorld* world = GetWorld();
		FActorSpawnParameters spawn;
		FRotator Rotator = SpawnPoint.Rotator();
		FVector SpawnLocation = SpawnPoint.GetLocation();

		
		world->SpawnActor<AActor>(CorridorTypes[0], SpawnLocation, Rotator, spawn);
		PlayerEnterLevel();
		ExitHasSpawned = true;
	}
}
