

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "_Types/Structs/AccessoryInfo.h"
#include "_Types/Structs/GunInfo.h"
#include "SelectionSlot.generated.h"

class AMenuPlayerController;
class UImage;
class UTextBlock;
class UButton;
class UKitEditor;

/**
 * An Enum dedicated to the type of the Selection Slot we are creating.
 * If more customization types are added, a new Enum entry must be created bellow.
 */

UENUM()
enum class ESelectionType
{
	Primary,
	Secondary,
	Grenade,
	Backpack,
	Deployable,
	Sights,
	Side,
	Magazine,
	Muzzle
};

/**
 *	A Template UI created by the Kit Editor. Responsible for presenting customization option.
 *	Since the functionality is the same aside from the type of customizing that is being done, a single class suffices.
 *	It however does use an enum declared above to help it know which it is.
 */

UCLASS()
class FPSK_API USelectionSlot final : public UUserWidget
{
	GENERATED_BODY()

public:
	//  InitializeObject initializes this UMG with different things, depending on which version is called.
	/**	@param Type The type of selection it is created for.
	 *	@param ParentWidget The KitEditor that is creating this UI.
	 *	@param SelectionGunInfo The GunInfo this is being created for. */
	void InitializeObject(const ESelectionType& Type, UKitEditor* ParentWidget, const FGunInfo& SelectionGunInfo);
	/**	@param Type The type of selection it is created for.
	 *	@param ParentWidget The KitEditor that is creating this UI.
	 *	@param SelectionAccessoryInfo The AccessoryInfo this is being created for. */
	void InitializeObject(const ESelectionType& Type, UKitEditor* ParentWidget, const FAccessoryInfo& SelectionAccessoryInfo);
	/**	@param Type The type of selection it is created for.
	 *	@param ParentWidget The KitEditor that is creating this UI.
	 *	@param AttachSlot The attachment slot of this Gun modification.
	 *	@param SelectionAttachmentInfo The AttachmentInfo this is being created for.
	 *	@param bIsPrimary Is it modding the primary weapon? If false, it's secondary. */
	void InitializeObject(const ESelectionType& Type, const EAttachmentSlot& AttachSlot, UKitEditor* ParentWidget, const FAttachmentInfo& SelectionAttachmentInfo, bool bIsPrimary);	

private:
	virtual void NativeConstruct() override;

	/** Bound to SelectButton. Updates The KitEditor with selection. */
	UFUNCTION() void OnSelect();
	/** Bound to SelectButton. Updates the Gun Mod preview. */
	UFUNCTION() void OnHovered();
	/** Bound to SelectButton. Removes the Gun Mod preview. */
	UFUNCTION() void OnUnhovered();
	
	UPROPERTY(meta=(BindWidget)) UButton* SelectButton;
	UPROPERTY(meta=(BindWidget)) UTextBlock* Title;
	UPROPERTY(meta=(BindWidget)) UImage* Icon;

	/** Ptr to KitEditor who created this UI slot.*/
	UPROPERTY() UKitEditor* KitEditor;
	/** Ptr to the Menu Controller. Set on created. */
	UPROPERTY() AMenuPlayerController* MenuController;

	// The following holds the data that this slot will be responsible for.
	FGunInfo GunInfo;					// Selecting a new gun.
	FAccessoryInfo AccessoryInfo;		// Selecting an accessory.
	FAttachmentInfo AttachmentInfo;		// Selecting an attachment.
	EAttachmentSlot AttachmentSlot;		// Type of the attachment.
	ESelectionType SelectionType;		// Type of the selection to accordingly update.
	bool bIsModdingPrimary = true;		// If modding the primary of secondary to apply mod to correct slot.
};
