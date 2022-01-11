// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyInterface.generated.h"

//UINTERFACE(MinimalAPI, Blueprintable)
UINTERFACE(BlueprintType)
class GUNFIRE_API UEnemyInterface : public UInterface {
	GENERATED_UINTERFACE_BODY()
};

class GUNFIRE_API IEnemyInterface {
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy")
		bool IsEnemy();
};