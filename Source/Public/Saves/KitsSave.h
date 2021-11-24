

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "_Types/Structs/KitInfo.h"
#include "KitsSave.generated.h"

/**
 * 
 */
UCLASS()
class FPSK_API UKitsSave : public USaveGame
{
	GENERATED_BODY()

public:
	/** Overwrites the currently saved Kits. */
	void UpdateKits(TArray<FKitInfo> InLoadedKits, const FKitInfo& UpdatedKit);
	void AddKit(const FKitInfo& NewKit);

	/** Updates the current kit.
	 *	@param NewCurrentKit The new kit to be saved. */
	void SetCurrentKit(const FKitInfo& NewCurrentKit) { SelectedKit = NewCurrentKit; };
	/** @return The currently saved Kits. */
	TArray<FKitInfo> GetCurrentKits() const { return Kits; }

private:
	UPROPERTY() TArray<FKitInfo> Kits;
	UPROPERTY() FKitInfo SelectedKit;
};
