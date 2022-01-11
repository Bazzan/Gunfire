// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "LevelManager.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LocalLevelTracker.h"
#include "CorridorBase.generated.h"

class UGameplayStatics;
class UGunfireCharacter;

UCLASS()
class GUNFIRE_API ACorridorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACorridorBase();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USceneComponent* SceneRoot;

	UPROPERTY() ALevelManager* LevelManager;
	UPROPERTY(EditDefaultsOnly) TArray<ALocalLevelTracker*> LocalLevelTracker;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UBoxComponent* EnterDoorCollider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UBoxComponent* ExitDoorCollider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UBoxComponent* TeleportPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UStaticMeshComponent* EnterDoor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UStaticMeshComponent* ExitDoor;
	
	UPROPERTY(EditAnywhere) TArray<FTransform> EnterPositions;
	UPROPERTY(EditAnywhere) TArray<FTransform> ExitPositions;
	UPROPERTY(BlueprintReadOnly) int32 CurrentLevel = 0;
	TArray<bool> UniqeCheck;
	int32 RandomLevelIndex;
	
	UFUNCTION()
	void OnPlayerEnterCorridor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPlayerExitCorridor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPlayerTeleport(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent) void DestroyCorridor();
	UFUNCTION(BlueprintImplementableEvent) void ResetRoom();

	int32 CheckBannedLevelNames(int32 LevelName);
};

