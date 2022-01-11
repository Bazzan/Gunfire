// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../DataAssets/BoonAsset.h"
#include "BoonPickup.generated.h"
class UAttributesComponent;
class UBoonEffectsBaseClass;
class ABoonSpawnerManager;
class UBoonAsset;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickup, AActor*, boon);
UCLASS()
class GUNFIRE_API ABoonPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	 UAttributesComponent* Player;
	// Sets default values for this actor's properties
	ABoonPickup();
	UFUNCTION(BlueprintCallable)
	void PickupBoon();
	void AddBoonToPlayer();
	void RemovePreviousBoon();
	UFUNCTION(BlueprintCallable)
		void UpdateBoon();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int BoonLevel() { return ActiveBoonIndex + 1; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FPrimaryAndSecondaryBoons BoonLevelEffect() { 
		if (boon) {
			return boon->BoonLevels()[ActiveBoonIndex];
		}
		FPrimaryAndSecondaryBoons empty;
		return empty;
		 };
	UPROPERTY(BlueprintAssignable)
	FOnPickup OnPickup;
	UFUNCTION(BlueprintImplementableEvent)
		void OnBoonPickup();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bRandomBoon = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoonAsset* boon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ActiveBoonIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bInitialize = false;
public:	
	// Called every frame

};
