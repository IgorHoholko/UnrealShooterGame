// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"


ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
	
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);

    CurrentAmo = DefaultAmo;
}


void ASTUBaseWeapon::StartFire(){
}

void ASTUBaseWeapon::StopFire(){
}

void ASTUBaseWeapon::MakeShot(){
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;
	
	return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetPlayerController();
	if (!Controller) return false;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}


bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;
	
	const FVector ShotDirection = ViewRotation.Vector();
	
	TraceStart = ViewLocation;					
	TraceEnd = TraceStart + ShotDirection * TraceMaxDistance;

	return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	if (!GetWorld()) return;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
}

bool ASTUBaseWeapon::IsHitBehind(const FHitResult& HitResult) const
{
	if (!HitResult.bBlockingHit) return false;
    
	FVector GunDirection = GetMuzzleRotation().GetForwardVector();
	FVector HitVector = ( HitResult.ImpactPoint - GetMuzzleLocation()).GetSafeNormal();

	float AngleBetweenTraceAndHit = FVector::DotProduct(GunDirection, HitVector);
	AngleBetweenTraceAndHit = FMath::RadiansToDegrees(FMath::Acos(AngleBetweenTraceAndHit));

	return AngleBetweenTraceAndHit >= MaxValidAngleBetweenTraceAndHit;
}


FVector  ASTUBaseWeapon::GetMuzzleLocation() const {
    return WeaponMesh->GetSocketTransform(MuzzleSocketName).GetLocation();
}
FQuat ASTUBaseWeapon::GetMuzzleRotation() const {
    return WeaponMesh->GetSocketTransform(MuzzleSocketName).GetRotation();
}

void ASTUBaseWeapon::DecreaseAmo() {
    CurrentAmo.Bullets--;
    LogAmmo();

    if (IsClipEmpty() && !IsAmoEmpty()) {
        OnClipEmpty.Broadcast();
    }
}

bool ASTUBaseWeapon::IsAmoEmpty() const {
    return !CurrentAmo.Infinite && CurrentAmo.Clips == 0 and IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const {
    return CurrentAmo.Bullets == 0;
}

void ASTUBaseWeapon::ChangeClip() {;
    if (!CurrentAmo.Infinite) {
        if (CurrentAmo.Clips == 0) return;
        CurrentAmo.Clips--;
    }
    CurrentAmo.Bullets = DefaultAmo.Bullets;
    
}

bool ASTUBaseWeapon::CanReload() const {
    return CurrentAmo.Bullets < DefaultAmo.Bullets && CurrentAmo.Clips > 0;
}

void ASTUBaseWeapon::LogAmmo() {
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmo.Bullets) + " / ";
    AmmoInfo += CurrentAmo.Infinite? "Infinite" : FString::FromInt(CurrentAmo.Clips);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *AmmoInfo);
}


