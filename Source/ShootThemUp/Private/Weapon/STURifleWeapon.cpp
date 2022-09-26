// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"


void ASTURifleWeapon::StartFire(){
	MakeShot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}
void ASTURifleWeapon::StopFire(){
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{
    if (BlockShot) return;
    
	if (!GetWorld() || IsAmoEmpty()) {
	    StopFire();
	    return;
	}

	FVector TraceStart;
	FVector TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) {
	    return;
	}
	
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FVector SocketLocation = WeaponMesh->GetSocketTransform(MuzzleSocketName).GetLocation();
	
	if (HitResult.bBlockingHit and !IsHitBehind(HitResult))
	{
		DrawDebugLine(GetWorld(), SocketLocation, HitResult.ImpactPoint, FColor::Red, false, 3, 0, 3.f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 24, FColor::Red, false, 5);

		UE_LOG(LogTemp, Error, TEXT("Bome: %s"), *HitResult.BoneName.ToString());

		MakeDamage(HitResult);
	}
	else
	{
		DrawDebugLine(GetWorld(), SocketLocation, TraceEnd, FColor::Red, false, 3, 0, 3.f);
	}

    DecreaseAmo();
	
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	const float HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShotDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);	

	TraceStart = ViewLocation;					
	TraceEnd = TraceStart + ShotDirection * TraceMaxDistance;

	return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor) return;

    DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}
