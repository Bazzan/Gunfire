// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectManager.h"
#include "../DataAssets/DOTEffect.h"
#include "../DataAssets/DefaultAttributes.h"
#include "../DataAssets/OnHitDamageAreaEffect.h"
#include "../Character/AttributesComponent.h"
#include "../Player/GunfireCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystemManager.h"
AEffectManager::AEffectManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEffectManager::BeginPlay()
{
	ACharacter* playerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerChar)
	{
		player = playerChar->FindComponentByClass<UAttributesComponent>();
		player->SetManager(this);
	}
	Super::BeginPlay();
}

void AEffectManager::SpawnChainLightningEffect(UOnHitDamageAreaEffect* OnHitEffect, UAttributesComponent* AffectedChar,
                                               TArray<UAttributesComponent*> targets, size_t i)
{
	if (i <= targets.Num() - 1)
	{
		UParticleSystemComponent* particleSystem = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), OnHitEffect->ParticleSystem, AffectedChar->GetOwner()->GetTransform());
		if (i == 0)
		{
			particleSystem->SetBeamSourcePoint(0, AffectedChar->GetOwner()->GetActorLocation(), 0);
			particleSystem->SetBeamTargetPoint(0, targets[i]->GetOwner()->GetActorLocation(), 0);
		}
		else
		{
			particleSystem->SetBeamSourcePoint(0, targets[i - 1]->GetOwner()->GetActorLocation(), 0);
			particleSystem->SetBeamTargetPoint(0, targets[i]->GetOwner()->GetActorLocation(), 0);
		}
	}
}
// kanske göra om till en delegate som man kan subscriba till?
void AEffectManager::TriggerAOEEffectOnActor(UOnHitDamageAreaEffect* OnHitEffect, UAttributesComponent* AffectedChar) 
{
	TArray<AActor*> overlappedActors;
	TArray<AActor*> ignoredActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> value;
	if (!OnHitEffect->IncludeOriginalTarget())
	{
		ignoredActors.Add(AffectedChar->GetOwner());
	}
	if (OnHitEffect->DisplayDebugSphere())
	{
		DrawDebugSphere(GetWorld(), AffectedChar->GetOwner()->GetActorLocation(), OnHitEffect->GetRadius(), 32,
		                FColor::Blue, true, 4);
	}


	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), AffectedChar->GetOwner()->GetActorLocation(),
	                                              OnHitEffect->GetRadius(), value, nullptr, ignoredActors,
	                                              overlappedActors))
	{
		TArray<UAttributesComponent*> targets;
		for (size_t i = 0; i < overlappedActors.Num(); i++)
		{
			if (OnHitEffect->Chaining() && targets.Num() >= OnHitEffect->GetChainLength())
			{
				i = overlappedActors.Num() + 42;
				continue;
			}
			UAttributesComponent* Attributes = overlappedActors[i]->FindComponentByClass<UAttributesComponent>();
			if (IsValid(Attributes) && OnHitEffect->TargetHasTags(overlappedActors[i]->Tags) &&
				!OnHitEffect->TargetIsImmune(overlappedActors[i]->Tags))
			{
				targets.Add(Attributes);
			}
		}

		TEnumAsByte<FRandomType> random = OnHitEffect->GetRandomDOT();
		int rand = 0;
		int effects = OnHitEffect->OtherEffects().Num();

		if (random == FRandomType::RandomDOTInList)
		{
			rand = FMath::FRandRange(0, effects - 1);
		}
		if (random == FRandomType::RandomPlayerDot)
		{
			effects = player->GetDotBoons().Num();
			rand = FMath::FRandRange(0, effects - 1);
			//Todo:: rand = random player dot index
		}
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OnHitEffect->BoonParticleEffect(),
		                                               AffectedChar->GetOwner()->GetActorLocation(),
		                                               AffectedChar->GetOwner()->GetActorRotation());
		for (size_t i = 0; i < targets.Num(); i++)
		{
			targets[i]->ChangeHealth(OnHitEffect->GetDamage(), false, !OnHitEffect->Healing());
			targets[i]->OnDamageTick.Broadcast(OnHitEffect->GetDamage(), EDotType::None, OnHitEffect->OnHitType);

			if (IsValid(OnHitEffect->ParticleSystem))
				SpawnChainLightningEffect(OnHitEffect, AffectedChar, targets, i);
			if (random == FRandomType::NotRandom)
			{
				for (size_t a = 0; a < effects; a++)
				{
					if (FMath::FRand() < OnHitEffect->OtherEffects()[a]->Percentage())
					{
						ApplyEffect(OnHitEffect->OtherEffects()[a], targets[i]);
					}
				}
			}
			else if (random == FRandomType::RandomDOTInList || random == FRandomType::RandomDOTInListChaos)
			{
				if (random == FRandomType::RandomDOTInListChaos)
				{
					rand = FMath::FRandRange(0, effects - 1);
				}
				if (FMath::FRand() < OnHitEffect->OtherEffects()[rand]->Percentage())
				{
					ApplyEffect(OnHitEffect->OtherEffects()[rand], targets[i]);
				}
			}
			else
			{
				if (player->GetDotBoons().Num() == 0)
				{
					return;
				}
				if (random == FRandomType::RandomPlayerDotChaos)
				{
					rand = FMath::FRandRange(0, effects - 1);
				}
				if (FMath::FRand() < player->GetDotBoons()[rand]->Percentage())
				{
					ApplyEffect(player->GetDotBoons()[rand], targets[i]);
				}
			}
		}
	}
}

void AEffectManager::CheckAndTriggerAOEEffectOnDOTTick(UDOTEffect* DotEffect, UAttributesComponent* AffectedChar, int index)
{
	TArray<FAoeTick> aoes = DotEffect->GetAOE();
	for (size_t i = 0; i < aoes.Num(); i++)
	{
		FTickTime ticktime = aoes[i].tickTime;
		bool triggerAOE = false;
		if (ticktime == FTickTime::Every_Tick)
		{
			triggerAOE = true;
		}
		else if (ticktime == FTickTime::Start && TimerCyclesDotEffects[index] == 0)
		{
			triggerAOE = true;
		}
		else if (ticktime == FTickTime::End && TimerCyclesDotEffects[index] == TimerCyclesDotEffects[index] == DotEffect
			->GetCycles() - 1)
		{
			triggerAOE = true;
		}
		else if (ticktime == FTickTime::OnDeath && AffectedChar->Health() <= 0)
		{
			triggerAOE = true;
		}
		if (triggerAOE && FMath::FRand() < aoes[i].TickEffect->GetTriggerPercentage())
		{
			TriggerAOEEffectOnActor(aoes[i].TickEffect, AffectedChar);
		}
	}
}


void AEffectManager::UpdateDOTTickEffect(UDOTEffect* DotEffect, UAttributesComponent* AffectedChar, int index)
{
	if (!IsValid(AffectedChar) || TimerCyclesDotEffects[index] >= DotEffect->GetCycles()) // clear timer?
		{
			RemoveDOTEffect(DotEffect, AffectedChar, index);
			return;
		}
	if (!IsValid(AffectedChar->GetOwner()))return;

	int damage = DotEffect->GetDamage();

	if (damage > 0)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DotEffect->BoonParticleEffect(),
		                                               AffectedChar->GetOwner()->GetActorLocation(),
		                                               AffectedChar->GetOwner()->GetActorRotation());
		
		AffectedChar->OnDamageTick.Broadcast(damage, DotEffect->DotType, EOnHitType::None);
		AffectedChar->ChangeHealth(damage, !DotEffect->Healing());
	}
	
	ApplySlow(DotEffect, AffectedChar);
	
	CheckAndTriggerAOEEffectOnDOTTick(DotEffect, AffectedChar, index);

	if (AffectedChar->Health() <= 0)
	{
		TimerCyclesDotEffects[index] += DotEffect->GetCycles() + 1;
	}
	TimerCyclesDotEffects[index]++;
}

void AEffectManager::RemoveDOTEffect(UDOTEffect* DotEffect, UAttributesComponent* AffectedChar, int index)
{
	GetWorld()->GetTimerManager().ClearTimer(DOTEffects[index]);
	TimerCyclesDotEffects[index] = 0;
	if (IsValid(AffectedChar) && AffectedChar->Health() > 0 && IsValid(AffectedChar->GetOwner())) // remove DOTEffect?
		{
		if (DotEffect->Stacking())
			AffectedChar->RemoveStatusEffect(DotEffect);

		UDefaultAttributes* defaults = AffectedChar->GetDefaults();
		if (AffectedChar->Speed() != defaults->Speed())
			AffectedChar->SetSpeed(defaults->Speed());
		}
}

// kanske göra om till en delegate som man kan subscriba till?
void AEffectManager::ApplySlow(UDOTEffect* DotEffect, UAttributesComponent* AffectedChar)
{
	float slowPercent = DotEffect->GetSlowPercentage();
	if (slowPercent != 0 && AffectedChar->Speed() > AffectedChar->GetDefaults()->Speed() * slowPercent)
	{
		AffectedChar->SetSpeed(AffectedChar->GetDefaults()->Speed() * slowPercent);
	}
}

void AEffectManager::ApplyEffect(UDOTEffect* DotEffect, UAttributesComponent* AffectedChar)
{
	if (!IsValid(DotEffect) || !IsValid(AffectedChar)) return;
	if (!DotEffect->Stacking() && AffectedChar->IsStatusEffectActive(DotEffect)) return;
	
	FTimerHandle newEffectTimerHandle;
	FTimerDelegate newTimerDelegate;
	int index = DOTEffects.Num();
	bool foundOldTimer = false;
	for (size_t i = 0; i < DOTEffects.Num(); i++) // find old timer to reuse
	{
		if (foundOldTimer) continue;
		
		if (GetWorldTimerManager().IsTimerActive(DOTEffects[i]) == false)
		{
			index = i;
			newEffectTimerHandle = DOTEffects[i];
			TimerDelegates[i] = FTimerDelegate::CreateUObject(this, &AEffectManager::UpdateDOTTickEffect, DotEffect,
			                                                  AffectedChar, index);
			foundOldTimer = true;
		}
	}
	if (!foundOldTimer) // create new timer if is not old one found
	{
		AffectedChar->OnApplyEffect.Broadcast(DotEffect->DotType);
		TimerCyclesDotEffects.Add(0);
		DOTEffects.Add(newEffectTimerHandle);
		TimerDelegates.Add(newTimerDelegate);
		TimerDelegates[index] = FTimerDelegate::CreateUObject(this, &AEffectManager::UpdateDOTTickEffect, DotEffect,
		                                                      AffectedChar, index);
		if (!DotEffect->Stacking())
		{
			AffectedChar->AddStatusEffect(DotEffect);
		}
	}
	GetWorldTimerManager().SetTimer(DOTEffects[index], TimerDelegates[index], DotEffect->GetTickRate(), true,
	                                DotEffect->Delay());
}



void AEffectManager::TriggerOnHitEffectByActor(UOnHitDamageAreaEffect* OnHitEffect, UAttributesComponent* AffectedChar,
                                               UAttributesComponent* attackingCharacter)
{
	TriggerAOEEffectOnActor(OnHitEffect, AffectedChar);
}

// void AEffectManager::TriggerOnHitEffectByLocation(UOnHitDamageAreaEffect* OnHitEffect, FVector position,
//                                                   UAttributesComponent* attackingCharacter)
// {
// }

// void AEffectManager::ApplyBuff(UAttributesComponent* BuffTarget, float duration)// not used
// {
// 	FTimerHandle newBuff;
// 	FTimerDelegate newDelegate;
// 	bool foundOldTimer = false;
// 	for (size_t i = 0; i < BuffEffects.Num(); i++)
// 	{
// 		if (foundOldTimer)
// 		{
// 			continue;
// 		}
// 		if (GetWorldTimerManager().IsTimerActive(BuffEffects[i]) == false)
// 		{
// 			newBuff = BuffEffects[i];
// 			foundOldTimer = true;
// 		}
// 	}
// 	if (!foundOldTimer)
// 	{
// 		TimerCyclesBuffs.Add(0);
// 		BuffEffects.Add(newBuff);
// 		BuffTimerDelegates.Add(newDelegate);
// 	}
// }