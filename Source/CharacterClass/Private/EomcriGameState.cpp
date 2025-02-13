#include "EomcriGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "EomcriGameInstance.h"
#include "EomcriPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AEomcriGameState::AEomcriGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	CurrentWaveIndex = 0;
	MaxWaves = 3;
}

void AEomcriGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AEomcriGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 AEomcriGameState::GetScore() const
{
	return Score;
}

void AEomcriGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UEomcriGameInstance* EomcriGameInstance = Cast<UEomcriGameInstance>(GameInstance);
		if (EomcriGameInstance)
		{
			EomcriGameInstance->AddToScore(Amount);
		}
	}
}

void AEomcriGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AEomcriPlayerController* EomcriPlayerController = Cast<AEomcriPlayerController>(PlayerController))
		{
			EomcriPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UEomcriGameInstance* EomcriGameInstance = Cast<UEomcriGameInstance>(GameInstance);
		if (EomcriGameInstance)
		{
			CurrentLevelIndex = EomcriGameInstance->CurrentLevelIndex;
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AEomcriGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
}

void AEomcriGameState::OnLevelTimeUp()
{
	EndLevel();
}

void AEomcriGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount)

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
	UpdateHUD();
}

void AEomcriGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	CurrentLevelIndex++;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UEomcriGameInstance* EomcriGameInstance = Cast<UEomcriGameInstance>(GameInstance);
		if (EomcriGameInstance)
		{
			AddScore(Score);
			EomcriGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void AEomcriGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AEomcriPlayerController* EomcriPlayerController = Cast<AEomcriPlayerController>(PlayerController))
		{
			EomcriPlayerController->ShowMainMenu(true);
		}
	}
}

void AEomcriGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AEomcriPlayerController* EomcriPlayerController = Cast<AEomcriPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = EomcriPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("Time")))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("Score")))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UEomcriGameInstance* EomcriGameInstance = Cast<UEomcriGameInstance>(GameInstance);
						if (EomcriGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), EomcriGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("Level")))
				{
					ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Level %d-%d"), CurrentLevelIndex+1, CurrentWaveIndex+1)));
				}
			}
		}
	}
}

