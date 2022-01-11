// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HighscoreSave.generated.h"

/**
 * 
 */
UCLASS()
class GUNFIRE_API UHighscoreSave : public USaveGame
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetHighScore() { return HighScore; }
	UFUNCTION(BlueprintCallable)
		void SetHighScore(int newScore) { if (newScore > HighScore) { HighScore = newScore; } }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		UCurveFloat* GetDamage() { return Damage; }
	UFUNCTION(BlueprintCallable)
		void SetDamage(UCurveFloat* newDamage) { if (newDamage) { Damage = newDamage; } }
	UFUNCTION(BlueprintCallable)
		UCurveFloat* GetHealth() { return Health; }
	UFUNCTION(BlueprintCallable)
		void SetHealth(UCurveFloat* newHealth) { if (newHealth) { Health = newHealth; } }
protected:
	UPROPERTY()
	int HighScore;
	UPROPERTY()
		UCurveFloat* Damage;
	UPROPERTY()
		UCurveFloat* Health;
};
