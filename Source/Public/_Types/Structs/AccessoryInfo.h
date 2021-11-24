#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AccessoryInfo.generated.h"


UENUM(BlueprintType)
enum EAccessoryType
{
	Invalid UMETA(Hidden),
	Grenade,
	Backpack,
	Deployable,
};

USTRUCT(BlueprintType)
struct FAccessoryInfo : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	uint8 ID;
	
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> AccessoryBP;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EAccessoryType> Type;
	
};