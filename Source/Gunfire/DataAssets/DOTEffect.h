// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BoonEffectsBaseClass.h"
#include "OnHitDamageAreaEffect.h"
#include "DOTEffect.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum FTickTime
{
	Start     UMETA(DisplayName = "Start"),
	End      UMETA(DisplayName = "End"),
	Every_Tick   UMETA(DisplayName = "Every Tick"),
	OnDeath   UMETA(DisplayName = "On Death"),
};
USTRUCT(BlueprintType)
struct FAoeTick {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UOnHitDamageAreaEffect* TickEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte < FTickTime> tickTime;
};


UENUM(BlueprintType)
enum class EDotType : uint8
{
	None,
	Fire,
	Poison,
	Slow,
	Doom,
};
UCLASS()
class GUNFIRE_API UDOTEffect : public UBoonEffectsBaseClass
{
	GENERATED_BODY()

protected:


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int damagePerTick;
	//How often the effect triggers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float tickRate;
	//How many times the effect triggers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int tickCycles;
	//If the effect slows and if so how much
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float slowPercent;
	//Delay before the effect starts working after applied
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float initialDelay;
	//The chance the effect has to activate
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float applicationPercentage = 100;
	//Can you stack this dot multiple times? (Slow percentage will never stack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bStacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray< FAoeTick> AOEticks;
	//Does this effect heal its target instead of damage it?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bHealing;
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EDotType DotType = EDotType::None;

	UFUNCTION(BlueprintCallable)
		float Delay() { return initialDelay; };
	UFUNCTION(BlueprintCallable)
		bool Stacking() { return bStacking; };
	UFUNCTION(BlueprintCallable)
		float Percentage() { return applicationPercentage * 0.01f; };
	UFUNCTION(BlueprintCallable)
		float GetTickRate() { return tickRate; };
	UFUNCTION(BlueprintCallable)
		int GetDamage() { return damagePerTick; };
	UFUNCTION(BlueprintCallable)
		float GetSlowPercentage() { return slowPercent * 0.01; };
	UFUNCTION(BlueprintCallable)
		int GetCycles() { return tickCycles; };
	UFUNCTION(BlueprintCallable)
		TArray< FAoeTick> GetAOE() { return AOEticks; };
	UFUNCTION(BlueprintCallable)
		bool Healing() { return bHealing; };
};
