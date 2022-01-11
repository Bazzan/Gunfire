// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoonEffectsBaseClass.h"
#include "Math/UnrealMathUtility.h"
#include "StatModifyingBoon.generated.h"

/**
 * 
 */
UCLASS()
class GUNFIRE_API UStatModifyingBoon : public UBoonEffectsBaseClass
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int ExtraHealth() { return ExtraHitPoints; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ExtraSpeed() { return ExtraMoveSpeed; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int AddFlatDamage() { return FlatDamageIncrease; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTimeBetweenShotsBoon() { return TimeBetweenShots; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetAddMagazineSizeBoon() { return MagazineSize; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetReloadSpeedBoon() { return ReloadSpeed; }
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetExtraCritDamageProxent(){return ExtraCritDamageProcent * 0.01;}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ExtraHitPoints = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExtraMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBetweenShots = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MagazineSize = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FlatDamageIncrease = 0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ExtraCritDamageProcent;
	

};
// UFUNCTION(BlueprintCallable, BlueprintPure)
// float DamageMult() { return DamageMultiplier; }
//
// UFUNCTION(BlueprintCallable, BlueprintPure)
// float HighestMult() { return HighestMultiplier; }
//
// UFUNCTION(BlueprintCallable, BlueprintPure)
// bool OnHeadShot() { return TriggerRepeatedlyOnHeadShot; }
//
// UFUNCTION(BlueprintCallable, BlueprintPure)
// bool Stacking() { return DoesBoonStack; }
//
// UFUNCTION(BlueprintCallable, BlueprintPure)
// float BoonDuration() { return Duration; }

// UPROPERTY(EditAnywhere, BlueprintReadWrite)
// float DamageMultiplier = 0.f;
// UPROPERTY(EditAnywhere, BlueprintReadWrite)
// float HighestMultiplier = 0;
// //Does the boon apply stat change on headshot?
// UPROPERTY(EditAnywhere, BlueprintReadWrite)
// bool TriggerRepeatedlyOnHeadShot = false;
// //If applied on headshot, does it stack?
// UPROPERTY(EditAnywhere, BlueprintReadWrite)
// bool DoesBoonStack = false;
// //If repeatable, how long is the duration? -1 for until miss
// UPROPERTY(EditAnywhere, BlueprintReadWrite)
// float Duration = -1;