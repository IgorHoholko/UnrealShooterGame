// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor {
    GENERATED_BODY()
	
public:	
    ASTUBaseWeapon();

    FOnClipEmpty OnClipEmpty;

    virtual void StartFire();
    virtual void StopFire();

    void SetBlockShot(bool flag) { BlockShot = flag;};

    void ChangeClip();
    bool CanReload() const;

    FWeaponUIData GetUIData() const {return UIData; }
    FAmoData GetAmoData() const {return CurrentAmo; }

    bool TryToAddAmmo(int32 ClipsAmount);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FName MuzzleSocketName = "MuzzleFlashSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon", meta=(ClampMin="0.0", ClampMax="180.0"))
	float MaxValidAngleBetweenTraceAndHit = 45.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    FAmoData DefaultAmo{15, 10, false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    UNiagaraSystem* MuzzleFX;

	virtual void MakeShot();

	APlayerController* GetPlayerController() const;

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

	bool IsHitBehind(const FHitResult& HitResult) const;
    
    FVector GetMuzzleLocation() const;
    FQuat GetMuzzleRotation() const;

    void DecreaseAmo();
    bool IsAmoEmpty() const;
    bool IsClipEmpty() const;
    void LogAmmo();
    bool IsAmmoFull() const;

    UNiagaraComponent* SpawnMuzzleFX();

    bool BlockShot = false;

private:
    
    FAmoData CurrentAmo;
};


