// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Math/Color.h"
//#include "NiagaraSystem.h"
#include "BoonEffectsBaseClass.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GUNFIRE_API UBoonEffectsBaseClass : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FText GetTitle() { return Title; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FText GetDescription() { return Description; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		UTexture2D* GetPicture() { return Picture; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FColor GetColor() { return PictureColor; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		UNiagaraSystem* BoonParticleEffect() { return bBoonParticleEffect; };
protected:
	//The name of the boon (Could be good to include level of boon here as well)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Picture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor PictureColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* bBoonParticleEffect;
};
