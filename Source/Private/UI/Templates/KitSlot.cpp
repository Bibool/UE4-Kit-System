#include "UI/Templates/KitSlot.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/KitSelection.h"

void UKitSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the OnPressed if it hasn't been done so before (This check stops UE4 from hanging)
	if (!SelectKitButton->OnPressed.IsBound()) SelectKitButton->OnPressed.AddDynamic(this, &UKitSlot::OnPressedKitButton);
}

void UKitSlot::InitializeObject(const int KitNumber, const FKitInfo& Kit, UKitSelection* ParentWidget)
{
	// Adds the number after KIT like the original First Assault. E.g KIT 2
	// Then stores the Kit and set our Ptr to the widget creating this slot.
	KitName->SetText(FText::FromString("KIT " + FString::FromInt(KitNumber)));
	KitInfo = Kit;
	KitSelectionPtr = ParentWidget;
}

void UKitSlot::OnPressedKitButton()
{
	// Tells the KitSelection a new Kit has been selected.
	// TODO: This could be translated into an Event broadcast.
	if (KitSelectionPtr)
	{
		KitSelectionPtr->UpdateContent(KitInfo);
	}
}