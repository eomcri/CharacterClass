#include "HealingItem.h"
#include "EomcriCharacter.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AEomcriCharacter* PlayerCharacter = Cast<AEomcriCharacter>(Activator))
		{
			// 캐릭터의 체력을 회복
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}

