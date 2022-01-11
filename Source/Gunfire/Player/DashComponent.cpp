// Fill out your copyright notice in the Description page of Project Settings.

#include "DashComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GunfireCharacter.h"

UDashComponent::UDashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDashComponent::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<AGunfireCharacter>(GetOwner());
	CharacterMovementComponent = player->GetCharacterMovement();
	DefaultGroundFriction = CharacterMovementComponent->GroundFriction;

	StopDashingDelegate.BindDynamic(this, &UDashComponent::StopDashing);
	PlayerDashDelegate.BindDynamic(this, &UDashComponent::PlayerDash);
	SetCanDashDelegate.BindDynamic(this, &UDashComponent::SetCanDash);
}


void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsDashing)
		Dash(DeltaTime);
}

void UDashComponent::Dash(float DeltaTime)
{
	ElapsedDashTime += DeltaTime;
	float normalizedDashTime = ElapsedDashTime / DashTime;
	if (IsDashing && player->GetWorldTimerManager().IsTimerActive(DashTimerHandle))
	{
		CharacterMovementComponent->Velocity = DashInput * DashForce * DashCurve->GetFloatValue(normalizedDashTime);
		GetOwner()->FindComponentByClass<UCameraComponent>()->SetFieldOfView(90 * (FieldOfViewCurve->GetFloatValue(normalizedDashTime) + 1));


	}
	else if (CanDash)
	{
		CharacterMovementComponent->GroundFriction = 0;
		CharacterMovementComponent->Velocity = DashInput * DashForce * DashCurve->GetFloatValue(normalizedDashTime);
		player->GetWorldTimerManager().SetTimer(DashTimerHandle, StopDashingDelegate, DashTime, false);
		CanDash = false;
		IsDashing = true;
		OnDash.Broadcast();

		
	}
	if (!CharacterMovementComponent->IsFalling())
		HasBeenGrounded = true;
}


void UDashComponent::StopDashing()
{
	UE_LOG(LogTemp, Display, TEXT("StopDashin()"));
	GetOwner()->FindComponentByClass<UCameraComponent>()->SetFieldOfView(90);

	IsDashing = false;
	CharacterMovementComponent->GroundFriction = DefaultGroundFriction;
	CharacterMovementComponent->Velocity.Set(0, 0, 0);
	HasBeenGrounded = !CharacterMovementComponent->IsFalling();
	player->GetWorldTimerManager().SetTimer(DashCooldownTimerHandel, SetCanDashDelegate, DashCooldown, false);
}

void UDashComponent::SetCanDash()
{
	if (CharacterMovementComponent->IsFalling())
	{
		player->GetWorldTimerManager().SetTimer(DashCooldownTimerHandel, SetCanDashDelegate, 0.1f, false);
	}
	else
	{
		HasBeenGrounded = !CharacterMovementComponent->IsFalling();
		CanDash = true;
	}
}

void UDashComponent::PlayerDash()
{
	if (CanDash)
	{
		IsDashing = true;
		ElapsedDashTime = 0.f;
		if (player->GetLastMovementInputVector() != FVector(0, 0, 0))
			DashInput = player->GetLastMovementInputVector();
		else
			DashInput = player->GetActorForwardVector();
	}
}
