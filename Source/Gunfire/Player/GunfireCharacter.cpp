// Fill out your copyright notice in the Description page of Project Settings.


#include "GunfireCharacter.h"
#include "Gunfire/Weapon/GunComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gunfire/Player/DashComponent.h"
#include "DifficultyComponent.h"
#include "../Character/AttributesComponent.h"
 
// Sets default values
AGunfireCharacter::AGunfireCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	Camera->SetupAttachment(RootComponent);
	GunMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("GunMuzzle"));
	// GunMuzzle->SetupAttachment(RootComponent);
	DashComponent = CreateDefaultSubobject<UDashComponent>(TEXT("DashComponent"));
	GunComponent = CreateDefaultSubobject<UGunComponent>(TEXT("GunComponent"));
	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("AttributesComponent"));
	DifficultyComponent = CreateDefaultSubobject<UDifficultyComponent>(TEXT("Difficulty"));
	CharacterMovementComponent = GetCharacterMovement();

}

// Called when the game starts or when spawned
void AGunfireCharacter::BeginPlay()
{

	Super::BeginPlay();

	OriginalSpeed = PlayerSpeed;
	UpdateSpeed();
	CharacterMovementComponent->AirControl = ControlInAir;
	DefaultGroundFriction = CharacterMovementComponent->GroundFriction;

}

// Called every frame
void AGunfireCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AGunfireCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGunfireCharacter::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGunfireCharacter::MoveVertical);
	PlayerInputComponent->BindAxis("LookUp", this, &AGunfireCharacter::LookVertical);
	PlayerInputComponent->BindAxis("LookRight", this, &AGunfireCharacter::LookHorizontal);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGunfireCharacter::PlayerJump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGunfireCharacter::OnLeftMouseButtonClick);
	PlayerInputComponent->BindAction("Fire",IE_Released,this,&AGunfireCharacter::OnLeftMouseButtonRelease);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, DashComponent, &UDashComponent::PlayerDash);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, GunComponent, &UGunComponent::Reload);
}

void AGunfireCharacter::OnLeftMouseButtonClick()
{
	isPressingMouse = true;
	OnLeftMouseClick.Broadcast();
	
}

void AGunfireCharacter::OnLeftMouseButtonRelease()
{
	isPressingMouse = false;
}

void AGunfireCharacter::UpdateSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = PlayerSpeed;
}

void AGunfireCharacter::MoveHorizontal(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AGunfireCharacter::MoveVertical(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AGunfireCharacter::PlayerJump()
{
	Jump();
}

void AGunfireCharacter::LookHorizontal(float Value)
{
	AddControllerYawInput(Value * MouseSensitivity);
}

void AGunfireCharacter::LookVertical(float Value)
{
	PitchAngel = FMath::ClampAngle(PitchAngel + (-Value * MouseSensitivity), -89, 89);
		Camera->SetRelativeRotation(FRotator(PitchAngel, 0, 0));
}
