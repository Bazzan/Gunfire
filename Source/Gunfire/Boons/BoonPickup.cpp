// Fill out your copyright notice in the Description page of Project Settings.


#include "BoonPickup.h"
#include "../Character/AttributesComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "../DataAssets/BoonEffectsBaseClass.h"

#include "../DataAssets/DOTEffect.h"
#include "../DataAssets/StatModifyingBoon.h"
#include "../DataAssets/OnHitDamageAreaEffect.h"
#include "../Managers/BoonSpawnerManager.h"
// Sets default values
ABoonPickup::ABoonPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}
// Called when the game starts or when spawned
void ABoonPickup::BeginPlay()
{
	if (!IsValid(Player))
	{
		Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UAttributesComponent>();
		if (!IsValid(Player))
		{
			Destroy();
			return;
		}
	}
	if (bRandomBoon)
	{
		boon = ABoonSpawnerManager::GetRandomBoon();
	}
	UpdateBoon();
	if (!IsValid(boon) || boon->BoonLevels().Num() == 0)
	{
		Destroy();
		return;
	}
	Super::BeginPlay();

}


void ABoonPickup::PickupBoon()
{
	
	UpdateBoon();

	RemovePreviousBoon();

	AddBoonToPlayer();

	OnPickup.Broadcast(this);
	OnBoonPickup();

	if (ActiveBoonIndex == boon->BoonLevels().Num() - 1) {
		ABoonSpawnerManager::RemoveBoon(boon);
		
	}
	Destroy();
	K2_DestroyActor();

	
}

void ABoonPickup::AddBoonToPlayer()
{
	Player->AddBoon(boon->BoonLevels()[ActiveBoonIndex].PrimaryBoon);
	int secondaries = boon->BoonLevels()[ActiveBoonIndex].SecondaryEffects.Num();
	for (size_t i = 0; i < secondaries; i++)
	{
		Player->AddBoon(boon->BoonLevels()[ActiveBoonIndex].SecondaryEffects[i]);
	}
}

void ABoonPickup::RemovePreviousBoon()
{
	if (ActiveBoonIndex > 0)
	{
		Player->RemoveBoon(boon->BoonLevels()[ActiveBoonIndex - 1].PrimaryBoon);
		int secondaries = boon->BoonLevels()[ActiveBoonIndex - 1].SecondaryEffects.Num();
		for (size_t i = 0; i < secondaries; i++)
		{
			Player->RemoveBoon(boon->BoonLevels()[ActiveBoonIndex - 1].SecondaryEffects[i]);
		}
	}
}

void ABoonPickup::UpdateBoon()
{
	UBoonEffectsBaseClass* boonEffect;
	
	if (!IsValid(boon)) {
		Destroy();
		return;
	}
	ActiveBoonIndex = 0;
	bool hasFoundBoon = false;
	int tempIndex = 0;
	while (!hasFoundBoon) {
		if (tempIndex >= boon->BoonLevels().Num()) {
			ActiveBoonIndex = 0;
			hasFoundBoon = true;
			break;
		}
		boonEffect = Cast<UBoonEffectsBaseClass>(boon->BoonLevels()[tempIndex].PrimaryBoon);
		if (IsValid(boonEffect) && Player->HasBoon(boonEffect)) {
			ActiveBoonIndex = tempIndex +1;
			if (ActiveBoonIndex >= boon->BoonLevels().Num()) {
				Destroy();
				return;
			}
			hasFoundBoon = true;
			break;
		}
		tempIndex++;
	}
}



