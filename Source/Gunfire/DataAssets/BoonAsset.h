// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BoonEffectsBaseClass.h"
#include "BoonAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPrimaryAndSecondaryBoons {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear)
		UBoonEffectsBaseClass* PrimaryBoon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray< UBoonEffectsBaseClass*> SecondaryEffects;
};
UCLASS()
class GUNFIRE_API UBoonAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		TArray< FPrimaryAndSecondaryBoons> BoonLevels() { return BoonsAndLevels; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray< FPrimaryAndSecondaryBoons> BoonsAndLevels;
	
};
