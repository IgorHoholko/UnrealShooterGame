// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STULauncherWeapon.h"


void ASTULauncherWeapon::StartFire() {
    MakeShot();
}

void ASTULauncherWeapon::MakeShot() {

    if (!GetWorld() || IsAmoEmpty()) return;

    FVector TraceStart;
    FVector TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit && !IsHitBehind(HitResult) ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleLocation()).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleLocation());

    // Don't call Begin play yet.
    auto Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>( ProjectileClass, SpawnTransform);
    if (Projectile) {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }
    // set projectile params

    DecreaseAmo();
    SpawnMuzzleFX();
}

