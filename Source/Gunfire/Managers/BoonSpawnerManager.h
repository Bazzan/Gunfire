// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoonSpawnerManager.generated.h"
class UBoonAsset;
class ABoonPickup;
UCLASS()
class GUNFIRE_API ABoonSpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoonSpawnerManager();
	UFUNCTION(BlueprintCallable)
		TArray< ABoonPickup*> SpawnBoons(TArray<FVector> positions,FRotator rotation);
	UFUNCTION(BlueprintCallable)
		ABoonPickup* SpawnSingleBoon(FVector location, ABoonPickup* boonToSpawn);
	UFUNCTION(BlueprintCallable)
		ABoonPickup* SpawnRandomSingleBoon(FVector location,FRotator Rotation);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static UBoonAsset* GetRandomBoon();
	UFUNCTION(BlueprintCallable)
		static void RemoveBoon(UBoonAsset* boonToRemove);
	UFUNCTION(BlueprintCallable)
		static void ResetBoonList();
	UFUNCTION(BlueprintCallable)
		static void UnlockBoons();
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ABoonPickup> RandomBoonActor;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBoonAsset*> AvailableBoons;
	/*UFUNCTION(BlueprintCallable, Category = "Sweet|Utilities")
		static TArray<UObject*> DynamicLoadContentFromPath(UClass* AssetClass = nullptr, bool LoadFromSubfolders = false, FString PathFromContentFolder = "Blueprints/Boons/CompleteBoonAssets");*/

	static TArray<UBoonAsset*> StaticAllBoons;
	static TArray<UBoonAsset*> StaticAvailableBoons;
	static TArray<UBoonAsset*> UnavailableBoons;
public:	

};
