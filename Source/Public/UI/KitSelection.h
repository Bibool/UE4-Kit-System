

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "_Types/Structs/KitInfo.h"
#include "KitSelection.generated.h"

class UKitEditor;
class UTextBlock;
class UImage;
class UDataTable;
class UButton;
class UScrollBox;


/**
 * This class draws a UI for selecting kits.
 * It is flexible in the way that it can allow modification of kits or not,
 * depending on how creation parameters.
 */


UCLASS()
class FPSK_API UKitSelection final : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Initializes this UI.
	 *	@param InGame Toggles the edit kit button. */
	void InitializeObject		(bool InGame = false);
	
	/** Updates all UI content within this UMG.
	 *	@param NewKitInfo The new kit that we will read the data from. */
	void UpdateContent			(const FKitInfo& NewKitInfo);
	
	/** Closes the Kit Editor. Required to avoid drawing multiple Editors.
	 *	@param InWidget The Widget we are closing. */
	void CloseKitEditor			(UKitEditor* InWidget);

	/**@return The Kits save slot name.*/
	FString& GetKitsSaveName() { return KitDataSaveSlotName; }
private:
	/** Initializes the content on */
	void InitializeContent();

	/** Bound to EditKitButtons. Opens the KitEditor screen.*/
	UFUNCTION() void OnClickedEditKitButton();

#pragma region UMG_BOUND	
	UPROPERTY(meta=(BindWidget)) UScrollBox* KitsBox;
	UPROPERTY(meta=(BindWidget)) UButton* EditKitButton;	
	UPROPERTY(meta=(BindWidget)) UTextBlock* PrimaryWeaponName;
	UPROPERTY(meta=(BindWidget)) UTextBlock* SecondaryWeaponName;
	UPROPERTY(meta=(BindWidget)) UTextBlock* GrenadeAccessoryName;
	UPROPERTY(meta=(BindWidget)) UTextBlock* BackpackAccessoryName;
	UPROPERTY(meta=(BindWidget)) UTextBlock* DeployableAccessoryName;	
	UPROPERTY(meta=(BindWidget)) UImage* PrimaryWeaponIcon;
	UPROPERTY(meta=(BindWidget)) UImage* SecondaryWeaponIcon;
	UPROPERTY(meta=(BindWidget)) UImage* GrenadeAccessoryIcon;
	UPROPERTY(meta=(BindWidget)) UImage* BackpackAccessoryIcon;
	UPROPERTY(meta=(BindWidget)) UImage* DeployableAccessoryIcon;
#pragma endregion 	

	/** The default kits to load in if no save is present. */
	UPROPERTY(EditDefaultsOnly) UDataTable* KitDataTable;
	
	/** The Widget BP for the KitSlot (template). */
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> BP_KitSlot;
	
	/** The Widget BP for the KitEditor. */
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> BP_KitEditor;
	
	/** The pointer to the KitEditor Widget Object.*/
	UPROPERTY() UKitEditor* KitEditor;
	
	/** The currently selected kit. */
	FKitInfo SelectedKit;
	
	/** Is this opened from in-game or from the main menu. If true, the KitEditor button will not show. */
	bool bInGame = false;
	
	/** Bool to enable changing defaults, leave false if unsure! */
	UPROPERTY(EditDefaultsOnly) bool bChangeDefaults = false;
	/** The maximum amount of kits available to the player. This is a byte of a range 0 - 255. */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bChangeDefaults"), meta = (UIMin = "0", UIMax = "255"))
	uint8 MaxKits = 4;
	/** The default selected kit. Defaulted at 0 (first kit). This is a byte of a range 0 - 255.  */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bChangeDefaults"), meta = (UIMin = "0", UIMax = "255"))
	uint8 StartingKit = 0;
	/** The name of the Kit Data save slot. Changing here will reflect in anything accessing Kits. */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bChangeDefaults")) FString KitDataSaveSlotName = "KitDataSlot";
};
