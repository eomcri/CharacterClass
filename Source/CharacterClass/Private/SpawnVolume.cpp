#include "SpawnVolume.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass * ActualClass = SelectedRow->ItemClass.Get()) // soft class
		{
			// 여기서 SpawnItem()을 호출하고, 스폰된 AActor 포인터를 리턴
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if (ItemDataTable)
	{
		TArray<FItemSpawnRow*> AllRows;
		static const FString ContextString(TEXT("ASpawnVolume::GetRandomItem"));
		ItemDataTable->GetAllRows(ContextString, AllRows);

		if (AllRows.IsEmpty()) return nullptr;

		float TotalChance = 0.0f;
		for (const FItemSpawnRow* Row : AllRows)
		{
			if (Row)
			{
				TotalChance += Row->SpawnChance;
			}
		}

		const float RandomValue = FMath::FRandRange(0.0f, TotalChance);
		float AccumulateChance = 0.0f;

		for (FItemSpawnRow* Row : AllRows)
		{
			if (Row)
			{
				AccumulateChance += Row->SpawnChance;
				if (RandomValue <= AccumulateChance)
				{
					return Row;
				}
			}
		}
		return nullptr;
	}
	return nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxOrigin = SpawningBox->GetComponentLocation();
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	return BoxOrigin + FVector(
		FMath::RandRange(-BoxExtent.X, BoxExtent.X),
		FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::RandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (ItemClass)
	{
		AActor* SpwanedActor = GetWorld()->SpawnActor<AActor>(
			ItemClass, 
			
			GetRandomPointInVolume(), 
			FRotator::ZeroRotator
		);
		return SpwanedActor;
	}
	return nullptr;
}
