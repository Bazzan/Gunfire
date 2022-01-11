
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gunfire/Player/GunfireCharacter.h"

#include "Bullet.generated.h"

class UStaticMeshComponent;
class UAttributesComponent;
UCLASS()
class GUNFIRE_API ABullet : public AActor
{
	GENERATED_BODY()
	private:
	AActor* OwningActor;
	UAttributesComponent* OwnerAttributes;
	UPROPERTY(EditAnywhere)
	float BulletSpeed = 500.f;
	UPROPERTY()
	FVector startPosition;
	
	public:	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;
	UFUNCTION(BlueprintCallable)
		void Setup(AActor* owningActor);
	UFUNCTION(BlueprintCallable)
		void Attack(AActor* target,FHitResult HitResult);
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	public:	
	
	// Sets default values for this actor's properties
	ABullet();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};


