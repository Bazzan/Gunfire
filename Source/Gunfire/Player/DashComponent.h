// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashComponent.generated.h"

class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashDeligate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GUNFIRE_API UDashComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	float DefaultGroundFriction;
public:
	// Sets default values for this component's properties
	UDashComponent();
	virtual void BeginPlay() override;

protected:
	// Called when the game starts
	FTimerHandle DashTimerHandle;
	FTimerHandle DashCooldownTimerHandel;
	bool IsDashing = false;
	bool CanDash = true;
	bool HasBeenGrounded = true;
	float ElapsedDashTime;
	FVector DashInput;
	ACharacter* player;
	UCharacterMovementComponent* CharacterMovementComponent;
FTimerDynamicDelegate StopDashingDelegate;
	FTimerDynamicDelegate SetCanDashDelegate;
public:
	FTimerDynamicDelegate PlayerDashDelegate;

	UPROPERTY(BlueprintAssignable)
	FDashDeligate OnDash;

	UPROPERTY(EditAnywhere)
	UCurveFloat* FieldOfViewCurve;
	UPROPERTY(EditAnywhere)
	UCurveFloat* DashCurve;
	UPROPERTY(EditAnywhere)
	float DashTime = 0.25f;
	UPROPERTY(EditAnywhere)
	float DashCooldown = 1.f;
	UPROPERTY(EditAnywhere)
	float DashForce = 2000;

	void Dash(float DeltaTime);
	UFUNCTION()
	void PlayerDash();

	UFUNCTION()
	void StopDashing();
	UFUNCTION()
	void SetCanDash();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
