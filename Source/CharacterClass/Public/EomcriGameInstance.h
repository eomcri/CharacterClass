#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EomcriGameInstance.generated.h"

UCLASS()
class CHARACTERCLASS_API UEomcriGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UEomcriGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentWaveIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
	
};
