// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Bullet.h"
#include "Components/ActorComponent.h"
#include "../DataAssets/DOTEffect.h"
#include "GunComponent.generated.h"

class AGunfireCharacter;
class UGunAttributes;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShootDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

class GUNFIRE_API UGunComponent : public UActorComponent
{
	GENERATED_BODY()

	
public:
	//Gun Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGunAttributes* GunAttributes;

	//Gun Variables
	int RemainingBullets;
	UPROPERTY(EditAnywhere)
	int MagasineSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeBetweenShots;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float ReloadSpeed;



	//Player Variables
	UPROPERTY(EditAnywhere)
	int PlayerDamage;
	UPROPERTY(EditAnywhere)
	int PlayerHealth;
	UPROPERTY(EditAnywhere)
	int PlayerMovementSpeed;
	UPROPERTY(EditAnywhere)
	int PlayerFlatDamageResistance; //Not a percentage, 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FTimerHandle BetweenShotsTimerHandle;
	FTimerHandle ReloadTimerHandel;
	// FTimerDelegate TimerDelegate;
	FTimerDynamicDelegate BetweenShotsTimerDelegate;
	FTimerDynamicDelegate ReloadTimerDelegate;
	// FTimerDynamicDelegate ReloadTimerDelegate;

	bool CanShoot = true;

	// int RemainingBullets;
	// int MagasineSize;
	// UPROPERTY( BlueprintReadOnly)
	// float TimeBetweenShots;
	// UPROPERTY(BlueprintReadOnly)
	// float ReloadSpeed;
public:
	// Sets default values for this component's properties
	UGunComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(BlueprintAssignable)
	FShootDelegate OnShoot;
	UPROPERTY(BlueprintAssignable)
	FReloadDelegate OnReload;
	AGunfireCharacter* Player;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABullet> Bullet;

	UPROPERTY(EditAnywhere)
	TArray<UDOTEffect*> EffectBlueprints;


	UFUNCTION()
	void Shoot();
	UFUNCTION()
	void SetCanShoot();
	UFUNCTION()
	void Reload();
	UFUNCTION()
	void HasReloadedTimer();
	// UFUNCTION(BlueprintImplementableEvent)
	// void OnReload();
	UFUNCTION(BlueprintCallable)
	void SetDefaultGunAttributes();
	//Gun Variables	//Gun Variables
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTimeBetweenShots() { return TimeBetweenShots; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetReloadSpeed() { return ReloadSpeed; }



};
