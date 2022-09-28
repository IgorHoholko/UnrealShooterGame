
#include "Components/STUWeaponComponent.h"

#include "Animations/AnimUtils.h"
#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();

    InitAnimation();

    CurrentWeaponIndex = 0;
    EquipWeapon(CurrentWeaponIndex);
	
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    CurrentWeapon = nullptr;
    for (auto Weapon: Weapons) {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}


void USTUWeaponComponent::SpawnWeapons()
{
    const auto Character = Cast<ACharacter>(GetOwner());
    
	if (!GetWorld() || !Character) return;

    for (auto OneWeaponData: WeaponData) {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        Weapon->SetOwner(Character);
        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::Reload);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void USTUWeaponComponent::InitAnimation() {
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify) {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    } else {
        UE_LOG(LogTemp, Error, TEXT("Equip Anim Notify forgetten to be set!"));
        checkNoEntry();
    }

    for (auto& OneWeaponData: WeaponData) {
        auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAniMontage);
        if (ReloadFinishedNotify) {
            ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
        }else {
            UE_LOG(LogTemp, Error, TEXT("Reload Anim Notify forgetten to be set!"));
            checkNoEntry();
        }
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName) {
    if (!Weapon || !SceneComponent) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex) {
    if (WeaponIndex < 0 || WeaponIndex > Weapons.Num()) return;
    
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Character) return;

    if (CurrentWeapon) {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {
        return Data.WeaponClass == CurrentWeapon->GetClass();
    });
    CurrentReloadAnimMontage = CurrentWeaponData? CurrentWeaponData->ReloadAniMontage : nullptr;
    
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    EquipInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) {
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || !Animation) return;

    Character->PlayAnimMontage(Animation);
}


void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent) {
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComponent) return;

    EquipInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent) {
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComponent) return;

    ReloadInProgress = false;
    CurrentWeapon->SetBlockShot(false);
}

bool USTUWeaponComponent::CanReload() const {
    return CurrentWeapon && !EquipInProgress && !ReloadInProgress;;
}

bool USTUWeaponComponent::CanFire() const {
    return CurrentWeapon && !EquipInProgress && !ReloadInProgress;
}

bool USTUWeaponComponent::CanEquip() const {
    return !EquipInProgress && !ReloadInProgress;
}

void USTUWeaponComponent::StartFire()
{
    if (!CanFire()) return;
	CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
	CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon() {
    if (!CanEquip()) return;
    
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::Reload() {
    if (!CanReload()) return;
    CurrentWeapon->SetBlockShot(true);
    CurrentWeapon->ChangeClip();
    ReloadInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const {
    if (CurrentWeapon) {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetAmoData(FAmoData& AmoData) const {
    if (CurrentWeapon) {
        AmoData = CurrentWeapon->GetAmoData();
        return true;
    }
    return false;
}
