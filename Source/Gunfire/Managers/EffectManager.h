// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "EffectManager.generated.h"
class UDOTEffect;
class UAttributesComponent;
class UOnHitDamageAreaEffect;


UCLASS()
class GUNFIRE_API AEffectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectManager();

	UFUNCTION(BlueprintCallable)
		void ApplyEffect(UDOTEffect* DotEffect, UAttributesComponent* AffectedChar);

	UFUNCTION(BlueprintCallable)
		 void TriggerOnHitEffectByActor(UOnHitDamageAreaEffect* OnHitEffect, UAttributesComponent* AffectedChar, UAttributesComponent* attackingCharacter);
	// UFUNCTION(BlueprintCallable)
	// 	 void TriggerOnHitEffectByLocation(UOnHitDamageAreaEffect* OnHitEffect, FVector position, UAttributesComponent* attackingCharacter);
	// UFUNCTION(BlueprintCallable)
	// 	void ApplyBuff(UAttributesComponent* BuffTarget, float duration = -1);

protected:
	void TriggerAOEEffectOnActor(UOnHitDamageAreaEffect* OnHitEffect, UAttributesComponent* AffectedChar);
	void CheckAndTriggerAOEEffectOnDOTTick(UDOTEffect* DotEffect, UAttributesComponent* AffectedChar, int index);
	void ApplySlow(UDOTEffect* DotEffect, UAttributesComponent* AffectedChar);
	void RemoveDOTEffect(UDOTEffect* DotEffect, UAttributesComponent* AffectedChar, int index);
	// bool RemoveDOTEffect(UDOTEffect* DotEffect, UAttributesComponent* AffectedChar, int index);
	void SpawnChainLightningEffect(UOnHitDamageAreaEffect* OnHitEffect, UAttributesComponent* AffectedChar, TArray<UAttributesComponent*> targets, size_t i);
	void UpdateDOTTickEffect(UDOTEffect* DotEffect, UAttributesComponent* AffectedChar, int index);
	// Called when the game starts
	virtual void BeginPlay() override;
	UAttributesComponent* player;
	 TArray<FTimerHandle> DOTEffects;
	 TArray<FTimerDelegate> TimerDelegates;
	 TArray<int> TimerCyclesDotEffects;


	 TArray<int> TimerCyclesBuffs;
	 TArray<FTimerHandle> BuffEffects;
	 TArray<FTimerDelegate> BuffTimerDelegates;
};
