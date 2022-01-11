// Fill out your copyright notice in the Description page of Project Settings.


#include "CorridorBase.h"
#include "../Player/DifficultyComponent.h"
#include "LevelManager.h"
#include "Components/SceneCaptureComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACorridorBase::ACorridorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));

	RootComponent = SceneRoot;
	SceneRoot = RootComponent;

	EnterDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enter Door"));
	EnterDoor->SetupAttachment(RootComponent);
	ExitDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Exit Door"));
	ExitDoor->SetupAttachment(RootComponent);

	EnterDoorCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Enter Collider"));
	EnterDoorCollider->SetupAttachment(RootComponent);
	EnterDoorCollider->OnComponentBeginOverlap.AddDynamic(this, &ACorridorBase::OnPlayerEnterCorridor);

	ExitDoorCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Exit Collider"));
	ExitDoorCollider->SetupAttachment(RootComponent);
	ExitDoorCollider->OnComponentBeginOverlap.AddDynamic(this, &ACorridorBase::OnPlayerExitCorridor);

	TeleportPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("Teleport Collider"));
	TeleportPoint->SetupAttachment(RootComponent);
	TeleportPoint->OnComponentBeginOverlap.AddDynamic(this, &ACorridorBase::OnPlayerTeleport);
	
}

// Called when the game starts or when spawned
void ACorridorBase::BeginPlay()
{
	Super::BeginPlay();
	TSubclassOf<ALevelManager> classToFind;
	classToFind = ALevelManager::StaticClass();

	TArray<AActor*> foundLevelManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, foundLevelManagers);

	EnterDoor->SetCollisionProfileName("NoCollision");
	EnterDoor->SetVisibility(false);
	
	LevelManager = Cast<ALevelManager>(foundLevelManagers[0]);
}

void ACorridorBase::OnPlayerEnterCorridor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		EnterDoor->SetCollisionProfileName("BlockAllDynamic");
		EnterDoor->SetVisibility(false);
		EnterDoorCollider->SetCollisionProfileName("NoCollision");
	}
}

void ACorridorBase::OnPlayerTeleport(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	//Check if the player is interacting with the collider
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		TeleportPoint->SetCollisionProfileName("NoCollision");
		
		LevelManager->Player->FindComponentByClass<UMovementComponent>()->Velocity = FVector(0,0,0);

		//Is there a position to teleport to?
		if (EnterPositions.Max() > 0 && ExitPositions.Max() > 0)
		{
			RandomLevelIndex = FMath::RandRange(0, EnterPositions.Num() - 1);
			RandomLevelIndex = CheckBannedLevelNames(RandomLevelIndex);


			//Move Corridor
			SetActorRotation(EnterPositions[RandomLevelIndex].GetRotation());
			SetActorLocation(EnterPositions[RandomLevelIndex].GetLocation());

			//Load in new level
			LevelManager->LoadNewLevel(RandomLevelIndex);
			CurrentLevel = RandomLevelIndex;

			ExitDoor->SetCollisionProfileName("NoCollision");
			ExitDoor->SetVisibility(false);
			ExitDoor->GetChildComponent(0)->DestroyComponent();

			EnterDoor->SetCollisionProfileName("BlockAllDynamic");
			EnterDoor->SetVisibility(true);
			EnterDoorCollider->SetCollisionProfileName("BlockAllDynamic");
			
			//Find the local level tracker and resets the room
			ResetRoom();

			
			//Moves player 
			Cast<APawn>(LevelManager->Player)->GetController()->SetControlRotation(
	EnterPositions[RandomLevelIndex].Rotator() - FRotator(0, 90, 0));
			LevelManager->Player->SetActorLocation(EnterPositions[RandomLevelIndex].GetLocation());

			LevelManager->Player->FindComponentByClass<UDifficultyComponent>()->AddClearedRoom();
		}
	}
}

int32 ACorridorBase::CheckBannedLevelNames(int32 LevelIndex)
{
	RandomLevelIndex = LevelIndex;
	UniqeCheck.SetNum(3);
	UniqeCheck[0] = false;
	UniqeCheck[1] = false;
	UniqeCheck[2] = false;

	for (int i = 0; i < LevelManager->BannedLevelNames.Num(); i++)
	{
		if (LevelManager->BannedLevelNames[i] != LevelManager->LevelNames[LevelIndex])
			//If the random name is not a banned name
		{
			UniqeCheck[i] = true;
		}
		else
		{
			UniqeCheck[i] = false;
		}
	}

	if (UniqeCheck[0] == true && UniqeCheck[1] == true && UniqeCheck[2] == true)
	{
		LevelManager->BannedLevelNames.RemoveAt(0);
		LevelManager->BannedLevelNames.Add(LevelManager->LevelNames[LevelIndex]);
	}
	else
	{
		RandomLevelIndex = FMath::RandRange(0, EnterPositions.Num() - 1);
		CheckBannedLevelNames(RandomLevelIndex);
	}
	return RandomLevelIndex;
}


void ACorridorBase::OnPlayerExitCorridor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (ExitPositions.Max() > 0)
		{

			LevelManager->SpawnExit(ExitPositions[CurrentLevel]);
		}
	}
}
