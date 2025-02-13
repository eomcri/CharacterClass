#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EomcriGameState.generated.h"

UCLASS()
class CHARACTERCLASS_API AEomcriGameState : public AGameState
{
	GENERATED_BODY()

public:
	AEomcriGameState();

	virtual void BeginPlay() override;

	// 전역 점수를 저장하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWaves;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	// 현재 점수를 읽는 함수
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	// 점수를 추가해주는 함수
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void StartLevel();
	void OnLevelTimeUp();
	void OnCoinCollected();
	void EndLevel();
	void UpdateHUD();
};
