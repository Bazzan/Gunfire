// Fill out your copyright notice in the Description page of Project Settings.
#include "GunComponent.h"
#include "Gunfire/Player/GunfireCharacter.h"
#include "Gunfire/DataAssets/GunAttributes.h"
UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Player = Cast<AGunfireCharacter>(GetOwner());
}
void UGunComponent::BeginPlay()
{
	Super::BeginPlay();
	
	BetweenShotsTimerDelegate.BindDynamic(this, &UGunComponent::SetCanShoot);
	ReloadTimerDelegate.BindDynamic(this, &UGunComponent::HasReloadedTimer);
	if (IsValid(GunAttributes))
	{
		SetDefaultGunAttributes();
	}
}
void UGunComponent::Shoot()
{
	if (!Player->isPressingMouse || !CanShoot) return;
	CanShoot = false;
	Player->GetWorldTimerManager().SetTimer(BetweenShotsTimerHandle, BetweenShotsTimerDelegate, TimeBetweenShots,
	                                        false);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Cast<AGunfireCharacter>(GetOwner());
	GetWorld()->SpawnActor<ABullet>(Bullet, Player->GunMuzzle->GetComponentLocation(),
	                                FRotator(Player->Camera->GetComponentRotation()), SpawnParameters)->Setup(
		GetOwner());

	--RemainingBullets;
	OnShoot.Broadcast();
	if (RemainingBullets <= 0)
	{
		Reload();
		return;
	}
}
void UGunComponent::SetCanShoot()
{
	CanShoot = true;
}
void UGunComponent::Reload()
{
	if (RemainingBullets == MagasineSize)
	{
		CanShoot = true;
		return;
	}
	Player->isPressingMouse = false;
	CanShoot = false;
	Player->GetWorldTimerManager().SetTimer(ReloadTimerHandel, ReloadTimerDelegate, ReloadSpeed, false);
	OnReload.Broadcast();
}
void UGunComponent::HasReloadedTimer()
{
	CanShoot = true;
	RemainingBullets = MagasineSize;
}
void UGunComponent::SetDefaultGunAttributes()
{
	MagasineSize = GunAttributes->GetMagazineSize();
	RemainingBullets = MagasineSize;
	TimeBetweenShots = GunAttributes->GetTimeBetweenShots();
	ReloadSpeed = GunAttributes->GetReloadSpeed();
}
void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CanShoot)
		Shoot();
}
