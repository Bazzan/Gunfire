// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemieAbstract.h"

// Sets default values
AEnemieAbstract::AEnemieAbstract()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemieAbstract::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemieAbstract::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemieAbstract::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

