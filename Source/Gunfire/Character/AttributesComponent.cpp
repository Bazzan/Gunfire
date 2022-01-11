// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesComponent.h"
#include "../Managers/EffectManager.h"
#include "../Player/GunfireCharacter.h"
#include "../DataAssets/DefaultAttributes.h"
#include "../DataAssets/DOTEffect.h"
#include "../DataAssets/StatModifyingBoon.h"
#include "../DataAssets/OnHitDamageAreaEffect.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Gunfire/Weapon/GunComponent.h"

AEffectManager* UAttributesComponent::StaticManager;


// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UAttributesComponent::BeginPlay()
{
	CharacterSetup();
	Super::BeginPlay();
}

void UAttributesComponent::ExecuteOnHitAttributesAtLocation(FVector position)
{
}

void UAttributesComponent::SetSpeed(float newSpeed)
{
	if (newSpeed >= 0)
	{
		MoveSpeed = newSpeed;
		if (IsValid(Controller))
		{
			Controller->MaxWalkSpeed = newSpeed;
		}
	}
}

void UAttributesComponent::SetHealth(float amount)
{
	if (amount > 0)
	{
		HitPoints = amount;
		MaxHitPoints = amount;
	}
}

void UAttributesComponent::SetDamage(float amount)
{
	if (amount >= 0)
	{
		AttackDamage = amount;
	}
}

void UAttributesComponent::ChangeHealth(int amount, bool HeadShot, bool bDamage)
{
	if (amount <= 0) return;

	HitPoints += amount * (-1 * bDamage);

	if (DefaultAttributes && !bDamage)
	{
		if (HitPoints > MaxHitPoints)
		{
			HitPoints = MaxHitPoints;
		}
	}

	if (HitPoints <= 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Oh noes! %s is very ded "), *UKismetSystemLibrary::GetObjectName(GetOwner()));

		OnDeath.Broadcast(HitPoints * -1);
	}
}

void UAttributesComponent::ApplyEffectsToCharacter(UAttributesComponent* target)
{
	if (!target || !StaticManager) { return; }


	for (size_t i = 0; i < Effects.Num(); i++)
	{
		if (Effects[i]->Percentage() < 1)
		{
			float random = FMath::FRand();
			if (random > Effects[i]->Percentage())
				continue;
		}
		StaticManager->ApplyEffect(Effects[i], target);
	}
}

void UAttributesComponent::ApplyOnHitEffectsToCharacter(UAttributesComponent* target)
{
	if (!target || !StaticManager) { return; }

	OnHit.Broadcast();
	for (size_t i = 0; i < OnHitEffects.Num(); i++)
	{
		if (OnHitEffects[i]->GetTriggerPercentage() < 1)
		{
			float random = FMath::FRand();
			if (random > OnHitEffects[i]->GetTriggerPercentage())
			{
				continue;
			}
		}

		StaticManager->TriggerOnHitEffectByActor(OnHitEffects[i], target, this);
	}
}

void UAttributesComponent::DamageCharacter(UAttributesComponent* target, FHitResult HitResult, bool ApplyEffects,
                                           bool HeadShot)
{
	if (!IsValid(target)) { return; }
	if (HitResult.BoneName == HeadBoneName)
	{
		target->OnHealthChangeAmount.Broadcast(AttackDamage * CritDamage, true);
		target->OnHealthChanged.Broadcast();
		target->ChangeHealth(AttackDamage * CritDamage, true);
	}
	else
	{
		target->OnHealthChangeAmount.Broadcast(AttackDamage, false);
		target->OnHealthChanged.Broadcast();
		target->ChangeHealth(AttackDamage, false);
	}
	if (ApplyEffects)
	{
		ApplyEffectsToCharacter(target);
	}
}

void UAttributesComponent::AddStatusEffect(UDOTEffect* statusEffect)
{
	if (IsValid(statusEffect))
	{
		StatusEffects.Add(statusEffect);
	}
}

void UAttributesComponent::RemoveStatusEffect(UDOTEffect* statusEffect)
{
	if (!IsValid(statusEffect) && StatusEffects.Contains(statusEffect))
	{
		StatusEffects.Remove(statusEffect);
	}
}

void UAttributesComponent::AddBoon(UBoonEffectsBaseClass* boon)
{
	if (IsValid(boon) && !AllBoons.Contains(boon))
	{
		AllBoons.Add(boon);
		UDOTEffect* dot;
		UOnHitDamageAreaEffect* onHit;
		UStatModifyingBoon* statModifier;
		dot = Cast<UDOTEffect>(boon);
		onHit = Cast<UOnHitDamageAreaEffect>(boon);
		statModifier = Cast<UStatModifyingBoon>(boon);
		if (IsValid(dot))
		{
			AddDotBoon(dot);
		}
		else if (IsValid(onHit))
		{
			AddOnHitBoon(onHit);
		}
		else if (IsValid(statModifier))
		{
			AddStatBoon(statModifier);
		}
	}
}


void UAttributesComponent::RemoveBoon(UBoonEffectsBaseClass* boon)
{
	if (IsValid(boon) && AllBoons.Contains(boon))
	{
		AllBoons.Remove(boon);
		UDOTEffect* dot;
		UOnHitDamageAreaEffect* onHit;
		UStatModifyingBoon* statModifier;
		dot = Cast<UDOTEffect>(boon);
		onHit = Cast<UOnHitDamageAreaEffect>(boon);
		statModifier = Cast<UStatModifyingBoon>(boon);
		if (IsValid(dot))
		{
			RemoveDotBoon(dot);
		}
		else if (IsValid(onHit))
		{
			RemoveOnHitBoon(onHit);
		}
		else if (IsValid(statModifier))
		{
			RemoveStatBoon(statModifier);
		}
	}
}

bool UAttributesComponent::HasBoon(UBoonEffectsBaseClass* boon)
{
	return AllBoons.Contains(boon);
}

void UAttributesComponent::AddOnHitBoon(UOnHitDamageAreaEffect* boon)
{
	if (IsValid(boon) && !HasOnHitBoon(boon))
	{
		OnHitEffects.Add(boon);
	}
}

void UAttributesComponent::AddStatBoon(UStatModifyingBoon* boon)
{
	//Todo: add stat modification if instant
	if (IsValid(boon) && !HasStatBoon(boon))
	{
		GunComponent->MagasineSize += boon->GetAddMagazineSizeBoon();
		GunComponent->ReloadSpeed -= boon->GetReloadSpeedBoon();
		GunComponent->TimeBetweenShots -= boon->GetTimeBetweenShotsBoon();

		AttackDamage += boon->AddFlatDamage();
		HitPoints += boon->ExtraHealth();
		MaxHitPoints += boon->ExtraHealth();
		MoveSpeed += boon->ExtraSpeed();
		SetSpeed(MoveSpeed);


		StatModifyingBoons.Add(boon);
	}
}

void UAttributesComponent::RemoveStatBoon(UStatModifyingBoon* boon)
{
	//Todo: remove stat modifiers
	if (IsValid(boon) && HasStatBoon(boon))
	{
		GunComponent->MagasineSize -= boon->GetAddMagazineSizeBoon();
		GunComponent->ReloadSpeed += boon->GetReloadSpeedBoon();
		GunComponent->TimeBetweenShots += boon->GetTimeBetweenShotsBoon();


		AttackDamage -= boon->AddFlatDamage();
		HitPoints -= boon->ExtraHealth();
		MaxHitPoints -= boon->ExtraHealth();
		MoveSpeed -= boon->ExtraSpeed();
		SetSpeed(MoveSpeed);

		StatModifyingBoons.Remove(boon);
	}
}

void UAttributesComponent::AddDotBoon(UDOTEffect* boon)
{
	if (IsValid(boon) && !HasDotBoon(boon))
	{
		Effects.Add(boon);
	}
}

void UAttributesComponent::RemoveOnHitBoon(UOnHitDamageAreaEffect* boon)
{
	if (IsValid(boon) && HasOnHitBoon(boon))
	{
		OnHitEffects.Remove(boon);
	}
}

void UAttributesComponent::RemoveDotBoon(UDOTEffect* boon)
{
	if (IsValid(boon) && HasDotBoon(boon))
	{
		Effects.Remove(boon);
	}
}


TArray<UBoonEffectsBaseClass*> UAttributesComponent::GetAllBoons()
{
	return AllBoons;
}

void UAttributesComponent::CharacterSetup()
{
	if (IsValid(DefaultAttributes))
	{
		MoveSpeed = DefaultAttributes->Speed();
		HitPoints = DefaultAttributes->Health();
		MaxHitPoints = DefaultAttributes->Health();
	}
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (IsValid(owner))
	{
		Controller = owner->FindComponentByClass<UCharacterMovementComponent>();
		GunComponent = owner->FindComponentByClass<UGunComponent>();
	}
	for (size_t i = 0; i < AllBoons.Num(); i++)
	{
		UDOTEffect* dot;
		UOnHitDamageAreaEffect* onHit;
		UStatModifyingBoon* statModifier;
		dot = Cast<UDOTEffect>(AllBoons[i]);
		onHit = Cast<UOnHitDamageAreaEffect>(AllBoons[i]);
		statModifier = Cast<UStatModifyingBoon>(AllBoons[i]);
		if (IsValid(dot))
		{
			AddDotBoon(dot);
		}
		else if (IsValid(onHit))
		{
			AddOnHitBoon(onHit);
		}
		else if (IsValid(statModifier))
		{
			AddStatBoon(statModifier);
		}
	}
}
