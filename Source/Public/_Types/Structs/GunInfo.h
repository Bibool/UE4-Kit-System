#pragma once

#include "CoreMinimal.h"
#include "AttachmentInfo.h"
#include "Engine/DataTable.h"

#include "GunInfo.generated.h"

USTRUCT(BlueprintType)
struct FGunInfo : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	uint8 ID;
	
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> GunBP;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* Model;
		
	UPROPERTY(EditDefaultsOnly)
	FAttachmentInfo SightsAttachment;

	UPROPERTY(EditDefaultsOnly)
	FAttachmentInfo MagazineAttachment;

	UPROPERTY(EditDefaultsOnly)
	FAttachmentInfo SideAttachment;
	
	UPROPERTY(EditDefaultsOnly)
	FAttachmentInfo MuzzleAttachment;
	
};