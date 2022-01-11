
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GunfireCharacter.generated.h"

class UGunComponent;
class UCharacterMovementComponent;
class UDashComponent;
class UDifficultyComponent;
class UAttributesComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftMouseButtonClick);


UCLASS()
class GUNFIRE_API AGunfireCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	float DefaultGroundFriction;
	float PitchAngel;
public:
	// Sets default values for this character's properties
	AGunfireCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGunComponent* GunComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDifficultyComponent* DifficultyComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDashComponent * DashComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UActorComponent* AttributesComponent;
	UCharacterMovementComponent* CharacterMovementComponent;
	
public:


	bool isPressingMouse = false;
	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnLeftMouseButtonClick();
	void OnLeftMouseButtonRelease();
	void UpdateSpeed();
	FOnLeftMouseButtonClick OnLeftMouseClick;
	UPROPERTY(EditAnywhere)
	float OriginalSpeed = 0;
	
	UPROPERTY(EditAnywhere) float PlayerSpeed = 400.0f;
	UPROPERTY(EditAnywhere) float ControlInAir = 1.0f;
	UPROPERTY(EditAnywhere) float JumpHight = 300;
	UPROPERTY(EditAnywhere) float MouseSensitivity = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USceneComponent* GunMuzzle;
		
	UPROPERTY(EditAnywhere,BlueprintReadWrite) UCameraComponent* Camera;
	
	void MoveHorizontal(float Value);
	void MoveVertical(float Value);
	void LookHorizontal(float Value);
	void LookVertical(float Value);
	void PlayerJump();
};
