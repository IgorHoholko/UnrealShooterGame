// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerHUDWidget.generated.h"


UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category="UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool GetWeaponUIData(FWeaponUIData& UIData) const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool GetAmoData(FAmoData& AmoData) const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool IsPlayerSpectating() const;
};

