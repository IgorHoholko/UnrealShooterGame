#pragma once
#include "STUCoreTypes.generated.h"

class ASTUBaseWeapon;

// weapon
DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmpty, ASTUBaseWeapon*)

// health
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float)


USTRUCT(BlueprintType)
struct FAmoData {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon", meta=(EditCondition="!Infinite"))
    int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    bool Infinite;
};


USTRUCT(BlueprintType)
struct FWeaponData {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    UAnimMontage* ReloadAniMontage;
};


USTRUCT(BlueprintType)
struct FWeaponUIData {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    UTexture2D* MainIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    UTexture2D* CrossHairIcon;
};

// VFX

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDecalData {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    UMaterialInterface* Material;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    FVector Size = FVector(10.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    float LifeTime = 60.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    float FadeOutTime = 0.7f;
};


USTRUCT(BlueprintType)
struct FImpactData {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    UNiagaraSystem* NiagaraEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    FDecalData DecalData;
};