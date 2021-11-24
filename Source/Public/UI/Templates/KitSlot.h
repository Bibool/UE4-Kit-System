

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "_Types/Structs/KitInfo.h"
#include "KitSlot.generated.h"

class UButton;
class UKitSelection;
class UImage;
class UTextBlock;

/**
 * A template UMG to create a clickable slot in the KitSelector.
 */

UCLASS()
class FPSK_API UKitSlot final : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Initializes this Object.
	 *	@param KitNumber The Index of this Kit.
	 *	@param Kit The Kit we are creating this template UI from.
	 *	@param ParentWidget Ptr to the KitSelection creating this template UI. */
	void InitializeObject( const int KitNumber, const FKitInfo& Kit, UKitSelection* ParentWidget );

private:
	virtual void NativeConstruct() override;
	
	/** Bound to SelectKitButton. Tells the KitSelection that a new kit has been selected. */
	UFUNCTION() void OnPressedKitButton();
	
	UPROPERTY(meta=(BindWidget)) UTextBlock* KitName;
	UPROPERTY(meta=(BindWidget)) UButton* SelectKitButton;

	/** The KitInfo that this slot will be responsible for. */
	UPROPERTY() FKitInfo KitInfo;
	/** Ptr to the KitSelection UI. */
	UPROPERTY() UKitSelection* KitSelectionPtr;
};