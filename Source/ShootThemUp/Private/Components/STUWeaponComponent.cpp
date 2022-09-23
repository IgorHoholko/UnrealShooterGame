
#include "Components/STUWeaponComponent.h"

#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	
}


void USTUWeaponComponent::SpawnWeapon()
{
	if (!GetWorld()) return;
	
	const auto Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;
	
	CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
	if (!CurrentWeapon) return;
	
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
	CurrentWeapon->SetOwner(Character);
}


void USTUWeaponComponent::StartFire()
{
	CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
	CurrentWeapon->StopFire();
}