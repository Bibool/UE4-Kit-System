


#include "UI/Templates/SelectionSlot.h"

#include "Actors/GunPreview.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Controllers/MenuPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/KitEditor.h"

void USelectionSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// We bind the functions to the appropriate delegate.
	// We check if not bound already to avoid UE4 hanging or freezing.
	if (!SelectButton->OnPressed.IsBound()) SelectButton->OnPressed.AddDynamic(this, &USelectionSlot::OnSelect);
	if (!SelectButton->OnHovered.IsBound()) SelectButton->OnHovered.AddDynamic(this, &USelectionSlot::OnHovered);
	if (!SelectButton->OnUnhovered.IsBound()) SelectButton->OnUnhovered.AddDynamic(this, &USelectionSlot::OnUnhovered);
}

void USelectionSlot::InitializeObject(const ESelectionType& Type, UKitEditor* ParentWidget, const FGunInfo& SelectedGunInfo)
{
	// Stores the data and updates the UI.
	KitEditor = ParentWidget;
	GunInfo = SelectedGunInfo;
	SelectionType = Type;
	
	Title->SetText(GunInfo.Name);
	Icon->SetBrushFromTexture(GunInfo.Icon, true);
}

void USelectionSlot::InitializeObject(const ESelectionType& Type, UKitEditor* ParentWidget, const FAccessoryInfo& SelectedAccessoryInfo)
{
	// Stores the data and updates the UI.
	KitEditor = ParentWidget;
	AccessoryInfo = SelectedAccessoryInfo;
	SelectionType = Type;
	
	Title->SetText(SelectedAccessoryInfo.Name);
	Icon->SetBrushFromTexture(SelectedAccessoryInfo.Icon, true);	
}

void USelectionSlot::InitializeObject(const ESelectionType& Type, const EAttachmentSlot& AttachSlot, UKitEditor* ParentWidget,	const FAttachmentInfo& SelectionAccessoryInfo, bool bIsPrimary)
{
	// Stores the data and updates the UI.
	// Furthermore we also get the current player controller as this is a weapon mod and we want to update the preview.
	KitEditor = ParentWidget;
	AttachmentInfo = SelectionAccessoryInfo;
	SelectionType = Type;
	AttachmentSlot = AttachSlot;
	bIsModdingPrimary = bIsPrimary;
	MenuController = Cast<AMenuPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	Title->SetText(SelectionAccessoryInfo.Name);
	Icon->SetBrushFromTexture(SelectionAccessoryInfo.Icon, true);	
}

void USelectionSlot::OnSelect()
{
	// Safety check then switch on the SelectionType (set on creation).
	// Each path will call the appropriate UpdateContent on the KitEditor and provide the data this slot is responsible for.
	if (KitEditor)
	{
		switch (SelectionType)
		{
		case ESelectionType::Primary:
			KitEditor->UpdateContent(GunInfo, true);
			break;
		case ESelectionType::Secondary:
			KitEditor->UpdateContent(GunInfo, false);
			break;
		case ESelectionType::Grenade:
			KitEditor->UpdateContent(AccessoryInfo, Grenade);
			break;
		case ESelectionType::Backpack:
			KitEditor->UpdateContent(AccessoryInfo, Backpack);
			break;
		case ESelectionType::Deployable:
			KitEditor->UpdateContent(AccessoryInfo, Deployable);
			break;
		case ESelectionType::Sights:
			KitEditor->UpdateContent(AttachmentInfo, EAttachmentSlot::Sights, bIsModdingPrimary);
			break;
		case ESelectionType::Side:
			KitEditor->UpdateContent(AttachmentInfo, EAttachmentSlot::Side, bIsModdingPrimary);
			break;
		case ESelectionType::Magazine:
			KitEditor->UpdateContent(AttachmentInfo, EAttachmentSlot::Magazine, bIsModdingPrimary);
			break;
		case ESelectionType::Muzzle:
			KitEditor->UpdateContent(AttachmentInfo, EAttachmentSlot::Muzzle, bIsModdingPrimary);
			break;
		}

		KitEditor->CloseSelection();
	}
}

void USelectionSlot::OnHovered()
{
	// Safety check then we set the mod on the preview gun.
	if (MenuController)
	{
		MenuController->GetGunPreview()->UpdateAttachment(AttachmentInfo.Model, AttachmentSlot);
	}
}

void USelectionSlot::OnUnhovered()
{
	// Safety check then we remove the mod on the preview gun.
	// TODO: Show current mod after removing it?
	// If so, when kit editor is opened it should store the current mods on the preview gun. Also store when a new mod is selected.
	// Then this function we are calling would simply revert to the stored USkeletalMesh!
	if (MenuController)
	{
		MenuController->GetGunPreview()->ClearAttachment(AttachmentSlot);
	}
}