// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "STUProjectile.generated.h"

class USTUWeaponFXComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUProjectile();

    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction;}

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    USTUWeaponFXComponent* WeaponFXComponent;
    
    UPROPERTY(VisibleDefaultsOnly, Category="Component")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleDefaultsOnly, Category="Component")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float LifeSpan = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float DamageRadius = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float DamageAmount = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    bool DoFullDamage = false;

    void BeginPlay() override;

private:

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

    FVector ShotDirection;

    AController* GetController() const;
};
