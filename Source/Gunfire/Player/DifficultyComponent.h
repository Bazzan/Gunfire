// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DifficultyComponent.generated.h"

class UHighscoreSave;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUNFIRE_API UDifficultyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDifficultyComponent();
	UFUNCTION(BlueprintCallable)
		static void AddClearedRoom() { RoomsCleared++; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static int ClearedRooms() { return RoomsCleared; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		 UCurveFloat* GetDifficultyHealth() { return DifficultyScalarHealth; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		 UCurveFloat* GetDifficultyDamage() { return DifficultyScalarDamage; }
	UFUNCTION(BlueprintCallable)
		void SetDifficultyDamage(UCurveFloat* curve);
	UFUNCTION(BlueprintCallable)
		void SetDifficultyHealth(UCurveFloat* curve);
	UFUNCTION(BlueprintCallable)
		void SaveScore();
	UFUNCTION(BlueprintCallable)
		UHighscoreSave* LoadScore();
	UFUNCTION(BlueprintCallable)
		static void ResetRoomsCleared() { RoomsCleared = 0; };
protected:
	virtual void BeginPlay() override;
		static int RoomsCleared;
		UPROPERTY()
		 UCurveFloat* DifficultyScalarHealth;
		UPROPERTY()
		 UCurveFloat* DifficultyScalarDamage;
	// Called when the game starts
	
public:	
	// Called every frame

		
};
