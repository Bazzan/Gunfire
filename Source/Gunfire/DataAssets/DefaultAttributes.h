// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DefaultAttributes.generated.h"

/**
 * 
 */
UCLASS()
class GUNFIRE_API UDefaultAttributes : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) const
		float Speed() { return DefaultMoveSpeed; }
	UFUNCTION(BlueprintCallable) const
		int Health() { return MaxHitpoints; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxHitpoints = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DefaultMoveSpeed = 400;
};
