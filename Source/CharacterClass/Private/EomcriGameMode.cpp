// Fill out your copyright notice in the Description page of Project Settings.


#include "EomcriGameMode.h"
#include "EomcriCharacter.h"
#include "EomcriPlayerController.h"

AEomcriGameMode::AEomcriGameMode()
{
	DefaultPawnClass = AEomcriCharacter::StaticClass();
	PlayerControllerClass = AEomcriPlayerController::StaticClass();
}

