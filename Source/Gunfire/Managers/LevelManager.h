// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

class UGameplayStatics;
class AGunfireCharacter;

UCLASS()
class GUNFIRE_API ALevelManager : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ALevelManager();
	UPROPERTY() AActor* Player;
	UPROPERTY(EditAnywhere) TArray<FName> LevelNames;
	UPROPERTY(EditAnywhere) TArray<TSubclassOf<AActor>> CorridorTypes;
	UPROPERTY() FName CurrentLevelName;
	UPROPERTY() TArray<FName> BannedLevelNames;

	virtual void BeginPlay() override;
	int32 LastSavedLevelIndex = 0;
	void LoadNewLevel(int32 NewLevelIndex);
	void SpawnExit(FTransform SpawnPoint);
	bool ExitHasSpawned;

	UFUNCTION(BlueprintImplementableEvent) void UnloadLevel(FName LevelToUnload);
	UFUNCTION(BlueprintImplementableEvent) void PlayerEnterLevel();
};
