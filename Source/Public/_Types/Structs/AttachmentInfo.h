#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AttachmentInfo.generated.h"


UENUM(BlueprintType)
enum class EAttachmentSlot : uint8
{
	Invalid UMETA(Hidden),
	Sights,
	Side,
	Magazine,
	Muzzle
};

USTRUCT(BlueprintType)
struct FAttachmentInfo : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	uint8 ID;
	
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* Model;

	UPROPERTY(EditDefaultsOnly)
	EAttachmentSlot Slot;
	
};