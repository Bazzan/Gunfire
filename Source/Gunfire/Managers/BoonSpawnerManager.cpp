// Fill out your copyright notice in the Description page of Project Settings.


#include "BoonSpawnerManager.h"
#include "../Boons/BoonPickup.h"
#include "../DataAssets/BoonAsset.h"
#include "Math/UnrealMathUtility.h"


TArray<UBoonAsset*> ABoonSpawnerManager::StaticAvailableBoons;
TArray<UBoonAsset*> ABoonSpawnerManager::StaticAllBoons;
TArray<UBoonAsset*> ABoonSpawnerManager::UnavailableBoons;

// Sets default values
ABoonSpawnerManager::ABoonSpawnerManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

TArray< ABoonPickup*> ABoonSpawnerManager::SpawnBoons(TArray<FVector> positions, FRotator Rotator)
{
	TArray<ABoonPickup*> boons;
	for (size_t i = 0; i < positions.Num(); i++)
	{

		boons.Add(SpawnRandomSingleBoon(positions[i], Rotator));
	}
	return boons;
}

ABoonPickup* ABoonSpawnerManager::SpawnSingleBoon(FVector location, ABoonPickup* boonToSpawn)
{
	FActorSpawnParameters spawn;
	return Cast<ABoonPickup>(GetWorld()->SpawnActor(boonToSpawn->GetClass(), &location));
}

ABoonPickup* ABoonSpawnerManager::SpawnRandomSingleBoon(FVector location, FRotator Rotator)
{
	return GetWorld()->SpawnActor<ABoonPickup>(RandomBoonActor, location, Rotator);
}

UBoonAsset* ABoonSpawnerManager::GetRandomBoon()
{
	if (StaticAvailableBoons.Num() == 0)
	{
		return nullptr;
	}
	int boon;
	if (UnavailableBoons.Num() >= StaticAvailableBoons.Num())
	{
		return nullptr;
	}
	TArray<UBoonAsset*> availableList;
	for (size_t i = 0; i < StaticAvailableBoons.Num(); i++)
	{
		if (!UnavailableBoons.Contains(StaticAvailableBoons[i]))
		{
			availableList.Add(StaticAvailableBoons[i]);
		}
	}
	boon = FMath::RandRange(0, availableList.Num() - 1);
	UnavailableBoons.Add(availableList[boon]);
	return availableList[boon];
}

void ABoonSpawnerManager::RemoveBoon(UBoonAsset* boonToRemove)
{
	if (IsValid(boonToRemove) && !UnavailableBoons.Contains(boonToRemove))
	{
		UnavailableBoons.Add(boonToRemove);

	}
	if (IsValid(boonToRemove) && StaticAvailableBoons.Contains(boonToRemove))
	{
		StaticAvailableBoons.Remove(boonToRemove);

	}

}


void ABoonSpawnerManager::ResetBoonList()
{
	StaticAvailableBoons = StaticAllBoons;
	UnavailableBoons.Empty();
}

void ABoonSpawnerManager::UnlockBoons()
{
	UnavailableBoons.Empty();
}

void ABoonSpawnerManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ResetBoonList();
}

// Called when the game starts or when spawned
void ABoonSpawnerManager::BeginPlay()
{
	StaticAllBoons = AvailableBoons;
	ResetBoonList();
	Super::BeginPlay();

}