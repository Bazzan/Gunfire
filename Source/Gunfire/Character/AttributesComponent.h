// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gunfire/DataAssets/DOTEffect.h"
#include "Gunfire/DataAssets/OnHitDamageAreaEffect.h"
#include "AttributesComponent.generated.h"
class UCharacterMovementComponent;
class AEffectManager;
class UDefaultAttributes;
class UOnHitDamageAreaEffect;
class UStatModifyingBoon;
class UBoonEffectsBaseClass;
class UGunComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBroadCastDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApplyEffectDelegate,EDotType, DotType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathDelegate, float, overkill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangeDelegate, float, damage, bool, headShot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHealthChangeTickDelegate, float, damage, EDotType, DotType, EOnHitType,HitType);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GUNFIRE_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttributesComponent();

	//Getters for the values in the component
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int Health() { return HitPoints; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetCurrentHealth() { return HitPoints; }
	UFUNCTION(BlueprintGetter)
	int GetMaxHealth() { return MaxHitPoints; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FName> GetInteractableTags() { return InteractableTags; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float Speed() { return MoveSpeed; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float DamageAmount() { return AttackDamage; }

	UDefaultAttributes* GetDefaults() { return DefaultAttributes; }

	//Setters with added functionality such as valid checks and value checks + some event broadcasting
	UFUNCTION(BlueprintCallable)
	void SetSpeed(float newSpeed);
	UFUNCTION(BlueprintCallable)
	void SetHealth(float amount);
	UFUNCTION(BlueprintCallable)
	void SetDamage(float amount);
	UFUNCTION(BlueprintCallable)
	void ChangeHealth(int amount, bool headShot = false, bool bDamage = true);

	//Functions for doing damage to another component, as well as applying the effects at that target and or location
	UFUNCTION(BlueprintCallable)
	void ExecuteOnHitAttributesAtLocation(FVector position);
	UFUNCTION(BlueprintCallable)
	void ApplyEffectsToCharacter(UAttributesComponent* target);
	UFUNCTION(BlueprintCallable)
	void ApplyOnHitEffectsToCharacter(UAttributesComponent* target);
	UFUNCTION(BlueprintCallable)
	void DamageCharacter(UAttributesComponent* target, FHitResult HitResult, bool ApplyEffects = true,
	                     bool HeadShot = false);


	//Events for available use in blueprinting or binding to in other code if relevant
	UPROPERTY(BlueprintAssignable)
	FDeathDelegate OnDeath;
	UPROPERTY(BlueprintAssignable)
	FBroadCastDelegate OnHit;
	UPROPERTY(BlueprintAssignable)
	FBroadCastDelegate OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FHealthChangeDelegate OnHealthChangeAmount;
	UPROPERTY(BlueprintAssignable)
	FHealthChangeTickDelegate OnDamageTick;
	UPROPERTY(BlueprintAssignable)
	FOnApplyEffectDelegate OnApplyEffect;
	


	//Adds effects to the attributes so that it knows if any effects are active on it (relevant for if the effects should stack or not)
	UFUNCTION(BlueprintCallable)
	void AddStatusEffect(UDOTEffect* statusEffect);
	UFUNCTION(BlueprintCallable)
	void RemoveStatusEffect(UDOTEffect* statusEffect);
	UFUNCTION(BlueprintCallable)
	bool IsStatusEffectActive(UDOTEffect* statusEffect)
	{
		return StatusEffects.Contains(statusEffect);
	};

	//Functions to access the boons this character has (to add or remove them from the outside)
	UFUNCTION(BlueprintCallable)
	bool HasDotBoon(UDOTEffect* boon) { return Effects.Contains(boon); }

	UFUNCTION(BlueprintCallable)
	bool HasStatBoon(UStatModifyingBoon* boon) { return StatModifyingBoons.Contains(boon); }

	UFUNCTION(BlueprintCallable)
	bool HasOnHitBoon(UOnHitDamageAreaEffect* boon) { return OnHitEffects.Contains(boon); }

	UFUNCTION(BlueprintCallable)
	void AddBoon(UBoonEffectsBaseClass* boon);
	UFUNCTION(BlueprintCallable)
	void RemoveBoon(UBoonEffectsBaseClass* boon);
	UFUNCTION(BlueprintCallable)
	bool HasBoon(UBoonEffectsBaseClass* boon);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UBoonEffectsBaseClass*> GetAllBoons();
	void CharacterSetup();
	UFUNCTION(BlueprintCallable)
	TArray<UDOTEffect*> GetDotBoons() { return Effects; };
	UFUNCTION(BlueprintCallable)
	void SetManager(AEffectManager* manager) { if (manager) { StaticManager = manager; } };
	bool bLastShotWasHeadShot;
protected:
	//assigns the manager to a static variable so not every character has to find it when it spawns
	static AEffectManager* StaticManager;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBoonEffectsBaseClass*> AllBoons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDefaultAttributes* DefaultAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> InteractableTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHitPoints = 100;
	//Resets to default max hp on begin play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=Health)
	int HitPoints = 0;
	//The base damage for this character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage;
	UPROPERTY(BlueprintReadOnly)
	float MoveSpeed;
	UPROPERTY(BlueprintReadOnly)
	float CritDamage = 1.5f;


	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UGunComponent* GunComponent;

	void AddOnHitBoon(UOnHitDamageAreaEffect* boon);
	void AddStatBoon(UStatModifyingBoon* boon);
	void AddDotBoon(UDOTEffect* boon);
	void RemoveOnHitBoon(UOnHitDamageAreaEffect* boon);
	void RemoveDotBoon(UDOTEffect* boon);
	void RemoveStatBoon(UStatModifyingBoon* boon);
	UPROPERTY()
	TArray<UDOTEffect*> Effects;
	UPROPERTY()
	TArray<UOnHitDamageAreaEffect*> OnHitEffects;
	UPROPERTY()
	TArray<UStatModifyingBoon*> StatModifyingBoons;
	FName HeadBoneName = "head";
	UPROPERTY()
	TArray<UDOTEffect*> StatusEffects;
	UCharacterMovementComponent* Controller;
public:
};
