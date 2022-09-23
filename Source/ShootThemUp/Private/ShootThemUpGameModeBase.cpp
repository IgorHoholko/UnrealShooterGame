// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootThemUpGameModeBase.h"

#include "STUBaseCharacter.h"
#include "STUPlayerController.h"
#include "UI/STUGameHUD.h"


AShootThemUpGameModeBase::AShootThemUpGameModeBase()
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
	HUDClass = ASTUGameHUD::StaticClass();
	
}
