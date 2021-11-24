#pragma once

#include "CoreMinimal.h"
#include "AccessoryInfo.h"
#include "GunInfo.h"
#include "Engine/DataTable.h"

#include "KitInfo.generated.h"

/**
 * This holds all data related to a kit and inherits from FTableRowBase to be able to be used in DataTables.
 * It holds the following: 2x GunInfo (Primary and Secondary), 3x AccessoryInfo (Grenade, Backpack and Deployable) as well as an ID.
 */

USTRUCT(BlueprintType)
struct FKitInfo : public FTableRowBase 
{
	GENERATED_BODY()

	FKitInfo()
		: ID		(0)
		, Primary	()
		, Secondary	()
		, Grenade	()
		, Backpack	()
		, Deployable()
	{}
	
	FKitInfo(const uint8 KitID, const FGunInfo& PrimaryGun, const FGunInfo& SecondaryGun, const FAccessoryInfo& GrenadeAccessory, const FAccessoryInfo& BackpackAccessory, const FAccessoryInfo& DeployableAccessory )
		: ID		(KitID)
		, Primary	(PrimaryGun)
		, Secondary	(SecondaryGun)
		, Grenade	(GrenadeAccessory)
		, Backpack	(BackpackAccessory)
		, Deployable(DeployableAccessory)
	{}

	/** The ID of this Kit, for logic purposes. */
	UPROPERTY(EditDefaultsOnly)
	uint8 ID;

	/** The Primary Gun slot. */
	UPROPERTY(EditDefaultsOnly)
	FGunInfo Primary;

	/** The Secondary Gun slot. */
	UPROPERTY(EditDefaultsOnly)
	FGunInfo Secondary;

	/** The Grenade Accessory slot. */
	UPROPERTY(EditDefaultsOnly)
	FAccessoryInfo Grenade;

	/** The Backpack Accessory slot. */
	UPROPERTY(EditDefaultsOnly)
	FAccessoryInfo Backpack;

	/** The Deployable Accessory slot. */
	UPROPERTY(EditDefaultsOnly)
	FAccessoryInfo Deployable;
};