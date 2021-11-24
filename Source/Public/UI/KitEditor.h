

#pragma once

#include "CoreMinimal.h"
#include "KitSelection.h"
#include "Blueprint/UserWidget.h"
#include "Templates/SelectionSlot.h"
#include "KitEditor.generated.h"

class UHorizontalBox;
class UCanvasPanel;

/**
 *	This class is in charge of editing kits in UMG.
 *	It acts as a window where the whole kit can be changed and is responsible for one kit (passed on creation).
 */

UCLASS()
class FPSK_API UKitEditor final : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Initializes this class with custom parameters.
	 *	@param KitSelection The UI parent creating this.
	 *	@param KitInfo The Kit this UMG is responsible to edit.	 */
	void InitializeObject(UKitSelection* KitSelection, const FKitInfo& KitInfo);

	// -- UPDATE CONTENT -- Overloaded function that updates UI elements depending on which overloaded function is called.
	/** Called when the KitEditor is first opened, updates all UI from the given kit data. */
	void UpdateContent(const FKitInfo& NewKitInfo) const;
	/** Called when a new gun is selected, updates the UI elements related to the Gun  based on bIsPrimary. */
	void UpdateContent(const FGunInfo& NewGun, bool bIsPrimary = true);
	/** Called when a new accessory is selected, updates the UI elements based on the enum given.*/
	void UpdateContent(const FAccessoryInfo& NewAccessory, const EAccessoryType& NewAccessoryType);
	/** Called when a new attachment is selected, updates the UI elements based on the enum given. */
	void UpdateContent(const FAttachmentInfo& NewAttachment, const EAttachmentSlot& InSlot, bool bIsPrimary = true);

	/** Closes the selection screen. */
	void CloseSelection();

private:
	virtual void NativeConstruct() override;

	// UMG BOUND FUNCTIONS - Naming Convention: DelegateUMGName. e.g: OnClickedPlayButton()
	UFUNCTION() void OnPressedChooseNewPrimary();
	UFUNCTION() void OnPressedChooseNewSecondary();
	UFUNCTION() void OnPressedChooseNewGrenade();
	UFUNCTION() void OnPressedChooseNewBackpack();
	UFUNCTION() void OnPressedChooseNewDeployable();
	UFUNCTION() void OnPressedSaveKit();
	UFUNCTION() void OnPressedCustomizePrimary();
	UFUNCTION() void OnPressedCustomizeSecondary();
	UFUNCTION() void OnPressedBackToEditor();
	UFUNCTION() void OnPressedBackToSelection();
	UFUNCTION() void OnPressedSightsFilter();
	UFUNCTION() void OnPressedSideFilter();
	UFUNCTION() void OnPressedMagazineFilter();
	UFUNCTION() void OnPressedMuzzleFilter();

	/** Toggles the filter buttons.
	 *	@param ButtonToDisable The Button to disable. */
	void ToggleButtons(UButton* ButtonToDisable);

	// The following functions share a similar functionality and PopulateAttachments is documented thoroughly.
	
	/** Populates the attachments selection window.
	 *	@param InAttachmentSlot The type of attachment we want to find and populate the list with. */
	void PopulateAttachments(const EAttachmentSlot& InAttachmentSlot = EAttachmentSlot::Sights);

	/** Populates the Accessory selection window.
	 *	@param InAccessoryType The type of accessory we want to find and populate the list with. */
	void PopulateAccessories(const EAccessoryType& InAccessoryType);

	/** Populates the Accessory selection window.
	 *	@param InSelectionType The type of gun we want to find and populate the list with. */
	void PopulateGuns(const ESelectionType& InSelectionType);

#pragma region UMG_BOUND	
	UPROPERTY(meta=(BindWidget)) UCanvasPanel* SelectionCanvas;
	UPROPERTY(meta=(BindWidget)) UHorizontalBox* AttachmentFilterBox;	
	UPROPERTY(meta=(BindWidget)) UScrollBox* SelectionBox;	
	UPROPERTY(meta=(BindWidget)) UButton* ChooseNewPrimaryWeapon;
	UPROPERTY(meta=(BindWidget)) UButton* ChooseNewSecondaryWeapon;
	UPROPERTY(meta=(BindWidget)) UButton* ChooseNewGrenadeAccessory;
	UPROPERTY(meta=(BindWidget)) UButton* ChooseNewBackpackAccessory;
	UPROPERTY(meta=(BindWidget)) UButton* ChooseNewDeployableAccessory;
	UPROPERTY(meta=(BindWidget)) UButton* SaveKit;
	UPROPERTY(meta=(BindWidget)) UButton* CustomizePrimaryWeapon;
	UPROPERTY(meta=(BindWidget)) UButton* CustomizeSecondaryWeapon;
	UPROPERTY(meta=(BindWidget)) UButton* BackToEditor;
	UPROPERTY(meta=(BindWidget)) UButton* BackToSelection;
	UPROPERTY(meta=(BindWidget)) UButton* SightsFilter;
	UPROPERTY(meta=(BindWidget)) UButton* SideFilter;
	UPROPERTY(meta=(BindWidget)) UButton* MagazineFilter;
	UPROPERTY(meta=(BindWidget)) UButton* MuzzleFilter;	
	UPROPERTY(meta=(BindWidget)) UTextBlock* PrimaryWeaponName;
	UPROPERTY(meta=(BindWidget)) UTextBlock* SecondaryWeaponName;
	UPROPERTY(meta=(BindWidget)) UTextBlock* GrenadeAccessoryName;
	UPROPERTY(meta=(BindWidget)) UTextBlock* BackpackAccessoryName;
	UPROPERTY(meta=(BindWidget)) UTextBlock* DeployableAccessoryName;
	UPROPERTY(meta=(BindWidget)) UTextBlock* CurrentSelectionName;
	UPROPERTY(meta=(BindWidget)) UTextBlock* CurrentSightsAttachedPrimary;
	UPROPERTY(meta=(BindWidget)) UTextBlock* CurrentSideAttachedPrimary;
	UPROPERTY(meta=(BindWidget)) UTextBlock* CurrentMagazineAttachedPrimary;
	UPROPERTY(meta=(BindWidget)) UTextBlock* CurrentMuzzleAttachedPrimary;
	UPROPERTY(meta=(BindWidget)) UTextBlock* CurrentSightsAttachedSecondary;
	UPROPERTY(meta=(BindWidget)) UTextBlock* CurrentSideAttachedSecondary;
	UPROPERTY(meta=(BindWidget)) UTextBlock* CurrentMagazineAttachedSecondary;
	UPROPERTY(meta=(BindWidget)) UTextBlock* CurrentMuzzleAttachedSecondary;	
	UPROPERTY(meta=(BindWidget)) UImage* PrimaryWeaponIcon;
	UPROPERTY(meta=(BindWidget)) UImage* SecondaryWeaponIcon;
	UPROPERTY(meta=(BindWidget)) UImage* GrenadeAccessoryIcon;
	UPROPERTY(meta=(BindWidget)) UImage* BackpackAccessoryIcon;
	UPROPERTY(meta=(BindWidget)) UImage* DeployableAccessoryIcon;
	UPROPERTY(meta=(BindWidget)) UImage* GunPreviewImage;
#pragma endregion 	

	/** The Kit this class is in charge of editing. All changes are made to this variable and ONLY writes to Save when the button is pressed. */
	FKitInfo CurrentKit;
	/** Is the primary being edited? If false, then secondary. */
	bool bEditingPrimary;
	/** The max amount of rows to read from the DataTable. */
	const int MaxRows = 20;
	/** The KitSelection that created this editor. */
	UPROPERTY() UKitSelection* KitSelectionParent;
	/** Holds the array of filter buttons. Used to enable/disable them. */
	UPROPERTY() TArray<UButton*> FilterButtons;
	/** The DataTable holding the data of all guns. */
	UPROPERTY(EditDefaultsOnly) UDataTable* GunsTable;
	/** The DataTable holding the data of all accessories. */
	UPROPERTY(EditDefaultsOnly) UDataTable* AccessoryTable;
	/** The DataTable holding the data of all gun attachments. */
	UPROPERTY(EditDefaultsOnly) UDataTable* AttachmentsTable;
	/** The Widget Blueprint for the UMG Template of the SelectionSlot. */
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> BP_Selection;
	// The following is displayed when the selection window is opened, depending on the choice.
	// E.G Selecting a new Primary, it will be "Choose" + PrimaryGunCategoryText "PRIMARY" to merge into "Choose PRIMARY"
	UPROPERTY(EditDefaultsOnly) FText PrimaryGunCategoryText;
	UPROPERTY(EditDefaultsOnly) FText SecondaryGunCategoryText;
	UPROPERTY(EditDefaultsOnly) FText BackpackCategoryText;
	UPROPERTY(EditDefaultsOnly) FText GrenadeCategoryText;
	UPROPERTY(EditDefaultsOnly) FText DeployableCategoryText;
	UPROPERTY(EditDefaultsOnly) FText AttachmentsCategoryText;
};