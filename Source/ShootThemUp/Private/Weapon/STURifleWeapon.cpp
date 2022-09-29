// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "Engine/World.h"
#include "STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"

ASTURifleWeapon::ASTURifleWeapon() {
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay() {
    Super::BeginPlay();

    check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire(){
    InitMuzzleFX();
	MakeShot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}
void ASTURifleWeapon::StopFire(){
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
    SetMuzzleFXVisibility(false);
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

    FVector TraceFXEnd = TraceEnd;
	if (HitResult.bBlockingHit and !IsHitBehind(HitResult))
	{
	    TraceFXEnd = HitResult.ImpactPoint;
        WeaponFXComponent->PlayImpactFX(HitResult);
		UE_LOG(LogTemp, Error, TEXT("Bome: %s"), *HitResult.BoneName.ToString());

		MakeDamage(HitResult);
	}

    SpawnTraceFX(GetMuzzleLocation(), TraceFXEnd);
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

void ASTURifleWeapon::InitMuzzleFX() {
    if (!MuzzleFXComponent) {
        MuzzleFXComponent = SpawnMuzzleFX();
    }
    SetMuzzleFXVisibility(true);
}
void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible) {
    if (MuzzleFXComponent) {
        MuzzleFXComponent->SetPaused(!Visible);
        MuzzleFXComponent->SetVisibility(Visible, true);
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd) {
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX,TraceStart);
    if (TraceFXComponent) {
        TraceFXComponent->SetNiagaraVariableVec3(TraceFXName, TraceEnd);
    }
}
