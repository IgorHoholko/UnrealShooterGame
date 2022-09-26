// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUProjectile.h"
#include "Weapon/STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()

public:

    virtual void StartFire() override;

protected:

    virtual void MakeShot() override;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    TSubclassOf<ASTUProjectile> ProjectileClass;

    
    
};
