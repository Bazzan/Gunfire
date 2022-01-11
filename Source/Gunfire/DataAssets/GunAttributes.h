// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GunAttributes.generated.h"

/**
 * 
 */
UCLASS()
class GUNFIRE_API UGunAttributes : public UDataAsset
{
	GENERATED_BODY()
	
	protected:

	UPROPERTY(EditAnywhere)
	float TimeBetweenShots= 0.33f;
	UPROPERTY(EditAnywhere)
	float TimeToReload = 1.5f;
	UPROPERTY(EditAnywhere)
	int MagazineSize = 6;
	
	public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTimeBetweenShots() {return TimeBetweenShots;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetReloadSpeed() {return TimeToReload;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetMagazineSize() {return MagazineSize;}
};
