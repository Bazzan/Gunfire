// Fill out your copyright notice in the Description page of Project Settings.
#include "Bullet.h"
#include "../Character/AttributesComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(1.5f);
}

void ABullet::Setup(AActor* owningActor)
{
	OwningActor = owningActor;
	if (IsValid(OwningActor))
	{
		OwnerAttributes = OwningActor->FindComponentByClass<UAttributesComponent>();
	}
	startPosition = GetActorLocation();
}

void ABullet::Attack(AActor* target, FHitResult HitResult)
{
	if (!IsValid(target) || target == OwningActor)return;
	if (!IsValid(OwnerAttributes))
	{
		K2_DestroyActor();
		return;
	}

	int InteractableTagsAmount = OwnerAttributes->GetInteractableTags().Num();
	//Checks to see if the actor hit is interactable according to the owners interactabletags list
	bool Interactable = false;
	for (size_t i = 0; i < InteractableTagsAmount; i++)
	{
		if (target->Tags.Contains(OwnerAttributes->GetInteractableTags()[i]))
		{
			Interactable = true;
			break;
		}
	}
	if (!Interactable)
	{
		OwnerAttributes->bLastShotWasHeadShot = true;
		K2_DestroyActor();
		return;
	}
	UAttributesComponent* enemyAttributes = target->FindComponentByClass<UAttributesComponent>();
	if (IsValid(enemyAttributes))
	{
		if (enemyAttributes && OwnerAttributes)
		{
			OwnerAttributes->DamageCharacter(enemyAttributes, HitResult);
			OwnerAttributes->ApplyOnHitEffectsToCharacter(enemyAttributes);
		}
	}
	K2_DestroyActor();
}
// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult HitResult;
	AddActorWorldOffset(GetActorForwardVector() * BulletSpeed * DeltaTime, true, &HitResult);
	if (HitResult.bBlockingHit)
		Attack(HitResult.GetActor(), HitResult);
}
