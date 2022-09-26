// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STUProjectile.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 2000.f;
    
}

void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();

    check(MovementComponent);
    check(CollisionComponent);
    
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;

    CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);

    SetLifeSpan(LifeSpan);
}


void ASTUProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit ){
    if (!GetWorld()) return;

    MovementComponent->StopMovementImmediately();

    // make damage
    UGameplayStatics::ApplyRadialDamage(GetWorld(),        
                                        DamageAmount,
                                        GetActorLocation(),
                                        DamageRadius,
                                        UDamageType::StaticClass(),
                                        {},
                                        this,
                                        GetController(),
                                        DoFullDamage);

    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, LifeSpan);
    
    Destroy();
}

AController* ASTUProjectile::GetController() const {
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn? Pawn->GetController() : nullptr;
}


