#include "EomcriGameInstance.h"

UEomcriGameInstance::UEomcriGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
	CurrentWaveIndex = 0;
}

void UEomcriGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}