// Fill out your copyright notice in the Description page of Project Settings.


#include "STUWeaponComponent.h"
#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

bool USTUPlayerHUDWidget::Initialize() {
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    if (HealthComponent) {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    };
    return Super::Initialize();
}

float USTUPlayerHUDWidget::GetHealthPercent() const {
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const {
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;
    
    return WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetAmoData(FAmoData& AmoData) const {
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;
    
    return WeaponComponent->GetAmoData(AmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const {
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const {
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}


void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta) {
    if (HealthDelta < 0.0f) {
        OnTakeDamage();
    }
    
}
