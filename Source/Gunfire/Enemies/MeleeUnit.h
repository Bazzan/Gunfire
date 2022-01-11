// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemieAbstract.h"


#include "MeleeUnit.generated.h"

/**
 * 
 */
class UHealthComponent;

UCLASS()
class GUNFIRE_API AMeleeUnit : public AEnemieAbstract
{
	GENERATED_BODY()

public:
	AMeleeUnit();
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;*/
};
