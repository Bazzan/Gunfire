// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BoonEffectsBaseClass.h"
#include "OnHitDamageAreaEffect.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum FRandomType
{
	NotRandom UMETA(DisplayName = "Use full list (Not Random)"),
	RandomPlayerDot UMETA(DisplayName = "Random Dot from player"),
	RandomPlayerDotChaos UMETA(DisplayName = "Player Dot chaos mode"),
	RandomDOTInList UMETA(DisplayName = "Random Dot from effect"),
	RandomDOTInListChaos UMETA(DisplayName = "Random Dot Chaos mode"),
};
UENUM(BlueprintType)
enum class EOnHitType : uint8
{
	None,
	Lighting,
	Explotion,
};
class UDOTEffect;
UCLASS()
class GUNFIRE_API UOnHitDamageAreaEffect : public UBoonEffectsBaseClass
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	EOnHitType OnHitType = EOnHitType::None;
	
	UFUNCTION(BlueprintCallable)
	float GetTriggerPercentage() { return TriggerPercentage * 0.01f; }

	UFUNCTION(BlueprintCallable)
	float GetDamage() { return Damage; }

	UFUNCTION(BlueprintCallable)
	float GetRadius() { return Radius; }

	UFUNCTION(BlueprintCallable)
	int GetChainLength() { return ChainTargetsAmount; }

	UFUNCTION(BlueprintCallable)
	bool TargetHasTags(TArray<FName> tags)
	{
		if (TargetTags.Num() == 0)
		{
			return true;
		}
		for (size_t i = 0; i < tags.Num(); i++)
		{
			if (TargetTags.Contains(tags[i]))
			{
				return true;
			}
		}
		return false;
	};
	UFUNCTION(BlueprintCallable)
	bool TargetIsImmune(TArray<FName> tags)
	{
		if (ImmuneTags.Num() == 0)
		{
			return false;
		}
		for (size_t i = 0; i < tags.Num(); i++)
		{
			if (ImmuneTags.Contains(tags[i]))
			{
				return true;
			}
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	bool Chaining() { return bChaining; }

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<FRandomType> GetRandomDOT() { return RandomDot; }

	UFUNCTION(BlueprintCallable)
	TArray<UDOTEffect*> OtherEffects() { return DotEffects; }

	UFUNCTION(BlueprintCallable)
	bool Healing() { return bHealing; }

	UFUNCTION(BlueprintCallable)
	bool IncludeOriginalTarget() { return bIncludeOriginalTarget; }

	UFUNCTION(BlueprintCallable)
	bool DisplayDebugSphere() { return bDisplayDebugSphere; }

protected:
	//How much flat damage the effect itself does (if any)
	UPROPERTY(EditAnywhere)
	float Damage = 1;
	//The radius of the effect
	UPROPERTY(EditAnywhere)
	float Radius = 1;
	//The chance the effect triggers at all
	UPROPERTY(EditAnywhere)
	float TriggerPercentage = 100;
	//Leave empty to affect everything in range
	UPROPERTY(EditAnywhere)
	TArray<FName> TargetTags;
	//If anything is iummune to this particular effect
	UPROPERTY(EditAnywhere)
	TArray<FName> ImmuneTags;
	//Does the effect target everything in range or does it chain between targets
	UPROPERTY(EditAnywhere)
	bool bChaining;
	//If chaining how many targets does it chain to?
	UPROPERTY(EditAnywhere)
	int ChainTargetsAmount = 1;
	//Which (if any) DOT effects does this boon apply to whoever is hit?
	UPROPERTY(EditAnywhere)
	TArray<UDOTEffect*> DotEffects;
	//If true, will take a random effect from the list of DOT effects
	UPROPERTY(EditAnywhere)
	TEnumAsByte<FRandomType> RandomDot;
	//Wether or not the effect should heal or not
	UPROPERTY(EditAnywhere)
	bool bHealing;
	//If this is activated will also target the target originaly hit by the projectile, or ignore them if its off
	UPROPERTY(EditAnywhere)
	bool bIncludeOriginalTarget;
	UPROPERTY(EditAnywhere)
	bool bDisplayDebugSphere;

public:
	UPROPERTY(EditAnywhere)
	UParticleSystem* ParticleSystem;
};
