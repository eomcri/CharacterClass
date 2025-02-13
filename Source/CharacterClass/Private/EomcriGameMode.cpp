// Fill out your copyright notice in the Description page of Project Settings.


#include "EomcriGameMode.h"
#include "EomcriCharacter.h"
#include "EomcriGameState.h"
#include "EomcriPlayerController.h"

AEomcriGameMode::AEomcriGameMode()
{
	DefaultPawnClass = AEomcriCharacter::StaticClass();
	PlayerControllerClass = AEomcriPlayerController::StaticClass();
	// 우리가 만든 GameState로 설정
	GameStateClass = AEomcriGameState::StaticClass();
}

