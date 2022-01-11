// Fill out your copyright notice in the Description page of Project Settings.


#include "DifficultyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../HighscoreSave.h"

// Sets default values for this component's properties
UDifficultyComponent::UDifficultyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

int UDifficultyComponent::RoomsCleared;

void UDifficultyComponent::SetDifficultyDamage(UCurveFloat* curve)
{
	UHighscoreSave* save;
	save = Cast<UHighscoreSave>(UGameplayStatics::LoadGameFromSlot("SaveGame1", 0));
	if (!save) {
		save = Cast<UHighscoreSave>(UGameplayStatics::CreateSaveGameObject(UHighscoreSave::StaticClass()));
	}
	save->SetDamage(curve);
	UGameplayStatics::SaveGameToSlot(save, "SaveGame1", 0);
}

void UDifficultyComponent::SetDifficultyHealth(UCurveFloat* curve)
{
	UHighscoreSave* save;
	save = Cast<UHighscoreSave>(UGameplayStatics::LoadGameFromSlot("SaveGame1", 0));
	if (!save) {
		save = Cast<UHighscoreSave>(UGameplayStatics::CreateSaveGameObject(UHighscoreSave::StaticClass()));
	}
	save->SetHealth(curve);
	UGameplayStatics::SaveGameToSlot(save, "SaveGame1", 0);
}

void UDifficultyComponent::SaveScore()
{
	UHighscoreSave* save;
	save = Cast<UHighscoreSave>(UGameplayStatics::LoadGameFromSlot("SaveGame1", 0));
	if (!save) {
		save = Cast<UHighscoreSave>(UGameplayStatics::CreateSaveGameObject(UHighscoreSave::StaticClass()));
	}
	save->SetHighScore(RoomsCleared);
	UGameplayStatics::SaveGameToSlot(save, "SaveGame1", 0);
}

UHighscoreSave* UDifficultyComponent::LoadScore() {
	return Cast<UHighscoreSave>(UGameplayStatics::LoadGameFromSlot("SaveGame1", 0));
}

void UDifficultyComponent::BeginPlay()
{
	UHighscoreSave* save;
	save = Cast<UHighscoreSave>(UGameplayStatics::LoadGameFromSlot("SaveGame1", 0));
	if (save) {
		DifficultyScalarHealth = save->GetHealth();
		DifficultyScalarDamage = save->GetDamage();
	}
}
