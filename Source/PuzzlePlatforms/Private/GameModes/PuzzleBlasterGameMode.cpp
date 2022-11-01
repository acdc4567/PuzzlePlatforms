// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/PuzzleBlasterGameMode.h"
#include "PuzzlePlatforms/PuzzlePlatformsCharacter.h"
#include "UObject/ConstructorHelpers.h"



APuzzleBlasterGameMode::APuzzleBlasterGameMode() {
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


}
