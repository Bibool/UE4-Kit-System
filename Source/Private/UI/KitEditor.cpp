


#include "UI/KitEditor.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "FPSK/Debug.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Saves/KitsSave.h"
#include "UI/Templates/SelectionSlot.h"

#ifndef MAX_ITERATION
	#define MAX_ITERATION MaxRows
#endif

void UKitEditor::NativeConstruct()
{
	Super::NativeConstruct();

	// We bind all our functions here, checking first if they haven't been bound to avoid UE4 freezes/crash.
	if (!ChooseNewPrimaryWeapon->OnPressed.IsBound()) ChooseNewPrimaryWeapon->OnPressed.AddDynamic(this, &UKitEditor::OnPressedChooseNewPrimary);
	if (!ChooseNewSecondaryWeapon->OnPressed.IsBound()) ChooseNewSecondaryWeapon->OnPressed.AddDynamic(this, &UKitEditor::OnPressedChooseNewSecondary);
	if (!ChooseNewBackpackAccessory->OnPressed.IsBound()) ChooseNewBackpackAccessory->OnPressed.AddDynamic(this, &UKitEditor::OnPressedChooseNewBackpack);
	if (!ChooseNewGrenadeAccessory->OnPressed.IsBound()) ChooseNewGrenadeAccessory->OnPressed.AddDynamic(this, &UKitEditor::OnPressedChooseNewGrenade);
	if (!ChooseNewDeployableAccessory->OnPressed.IsBound()) ChooseNewDeployableAccessory->OnPressed.AddDynamic(this, &UKitEditor::OnPressedChooseNewDeployable);
	if (!SaveKit->OnPressed.IsBound()) SaveKit->OnPressed.AddDynamic(this, &UKitEditor::OnPressedSaveKit);
	if (!CustomizePrimaryWeapon->OnPressed.IsBound()) CustomizePrimaryWeapon->OnPressed.AddDynamic(this, &UKitEditor::OnPressedCustomizePrimary);
	if (!CustomizeSecondaryWeapon->OnPressed.IsBound()) CustomizeSecondaryWeapon->OnPressed.AddDynamic(this, &UKitEditor::OnPressedCustomizeSecondary);
	if (!BackToEditor->OnPressed.IsBound()) BackToEditor->OnPressed.AddDynamic(this, &UKitEditor::OnPressedBackToEditor);
	if (!BackToSelection->OnPressed.IsBound()) BackToSelection->OnPressed.AddDynamic(this, &UKitEditor::OnPressedBackToSelection);
	if (!SightsFilter->OnPressed.IsBound()) SightsFilter->OnPressed.AddDynamic(this, &UKitEditor::OnPressedSightsFilter);
	if (!SideFilter->OnPressed.IsBound()) SideFilter->OnPressed.AddDynamic(this, &UKitEditor::OnPressedSideFilter);
	if (!MagazineFilter->OnPressed.IsBound()) MagazineFilter->OnPressed.AddDynamic(this, &UKitEditor::OnPressedMagazineFilter);
	if (!MuzzleFilter->OnPressed.IsBound()) MuzzleFilter->OnPressed.AddDynamic(this, &UKitEditor::OnPressedMuzzleFilter);

	// Empty the array (just in-case) then adds the filter buttons to it.
	FilterButtons.Empty();
	FilterButtons.Add(SightsFilter);
	FilterButtons.Add(SideFilter);
	FilterButtons.Add(MagazineFilter);
	FilterButtons.Add(MuzzleFilter);
}

void UKitEditor::InitializeObject(UKitSelection* KitSelection, const FKitInfo& KitInfo)
{
	// Stores the ptr to the parent and the KitInfo to our variable.
	CurrentKit = KitInfo;
	KitSelectionParent = KitSelection;
	// And updates all UI content.
	UpdateContent(CurrentKit);
}

void UKitEditor::OnPressedChooseNewPrimary()
{
	// Clear any children that might be in the SelectionBox as we use a single one.
	// Update the text reflecting the current selection category.
	SelectionBox->ClearChildren();
	CurrentSelectionName->SetText(PrimaryGunCategoryText);
	PopulateGuns(ESelectionType::Primary);
	SelectionCanvas->SetVisibility(ESlateVisibility::Visible);
}

void UKitEditor::OnPressedChooseNewSecondary()
{
	SelectionBox->ClearChildren();
	CurrentSelectionName->SetText(SecondaryGunCategoryText);
	PopulateGuns(ESelectionType::Secondary);
	SelectionCanvas->SetVisibility(ESlateVisibility::Visible);
}

void UKitEditor::OnPressedChooseNewGrenade()
{
	SelectionBox->ClearChildren();
	CurrentSelectionName->SetText(GrenadeCategoryText);
	PopulateAccessories(Grenade);
	SelectionCanvas->SetVisibility(ESlateVisibility::Visible);
}

void UKitEditor::OnPressedChooseNewBackpack()
{
	SelectionBox->ClearChildren();
	CurrentSelectionName->SetText(BackpackCategoryText);
	PopulateAccessories(Backpack);
	SelectionCanvas->SetVisibility(ESlateVisibility::Visible);
}

void UKitEditor::OnPressedChooseNewDeployable()
{
	SelectionBox->ClearChildren();
	CurrentSelectionName->SetText(DeployableCategoryText);
	PopulateAccessories(Deployable);
	SelectionCanvas->SetVisibility(ESlateVisibility::Visible);
}

void UKitEditor::OnPressedSaveKit()
{
	// We load from slot to get previously set Kits
	const UKitsSave* Load = Cast<UKitsSave>(UGameplayStatics::LoadGameFromSlot(KitSelectionParent->GetKitsSaveName(), 0));

	// Now we create a save game instance.
	if ( UKitsSave* Save = Cast<UKitsSave>(UGameplayStatics::CreateSaveGameObject(UKitsSave::StaticClass())))
	{
		// We set the save Kits to the altered copy of the Kit with the updated Kit.
		Save->UpdateKits(Load->GetCurrentKits(), CurrentKit);
	}
}

void UKitEditor::OnPressedCustomizePrimary()
{
	SelectionBox->ClearChildren();
	ToggleButtons(SightsFilter);
	bEditingPrimary = true;	
	CurrentSelectionName->SetText(AttachmentsCategoryText);
	PopulateAttachments();
	
	SelectionCanvas->SetVisibility(ESlateVisibility::Visible);
	AttachmentFilterBox->SetVisibility(ESlateVisibility::Visible);
	GunPreviewImage->SetVisibility(ESlateVisibility::Visible);	
}

void UKitEditor::OnPressedCustomizeSecondary()
{
	SelectionBox->ClearChildren();
	ToggleButtons(SightsFilter);
	bEditingPrimary = false;	
	CurrentSelectionName->SetText(AttachmentsCategoryText);
	PopulateAttachments();
	
	SelectionCanvas->SetVisibility(ESlateVisibility::Visible);
	AttachmentFilterBox->SetVisibility(ESlateVisibility::Visible);
	GunPreviewImage->SetVisibility(ESlateVisibility::Visible);	
}

void UKitEditor::OnPressedBackToEditor()
{
	// Hides all selection UI elements then clear all children.
	SelectionCanvas->SetVisibility(ESlateVisibility::Hidden);
	AttachmentFilterBox->SetVisibility(ESlateVisibility::Hidden);
	GunPreviewImage->SetVisibility(ESlateVisibility::Hidden);
	SelectionBox->ClearChildren();
}

void UKitEditor::OnPressedBackToSelection()
{
	// TODO: ADD CONFIRMATION
	KitSelectionParent->CloseKitEditor(this);
}

void UKitEditor::OnPressedSightsFilter()
{
	SelectionBox->ClearChildren();
	ToggleButtons(SightsFilter);
	PopulateAttachments(EAttachmentSlot::Sights);
}

void UKitEditor::OnPressedSideFilter()
{
	SelectionBox->ClearChildren();
	ToggleButtons(SideFilter);
	PopulateAttachments(EAttachmentSlot::Side);
}

void UKitEditor::OnPressedMagazineFilter()
{
	SelectionBox->ClearChildren();
	ToggleButtons(MagazineFilter);
	PopulateAttachments(EAttachmentSlot::Magazine);
}

void UKitEditor::OnPressedMuzzleFilter()
{
	SelectionBox->ClearChildren();
	ToggleButtons(MuzzleFilter);
	PopulateAttachments(EAttachmentSlot::Muzzle);
}

void UKitEditor::PopulateAttachments(const EAttachmentSlot& InAttachmentSlot)
{
	// Local var to feed in the correct SelectionType without making another parameter.
	// Since InAttachmentSlots is defaulted to sights, we initialize it with the same.
	ESelectionType Selection = ESelectionType::Sights;
	
	switch (InAttachmentSlot)
	{		
	case EAttachmentSlot::Invalid: break;
	case EAttachmentSlot::Sights:	Selection = ESelectionType::Sights; break;
	case EAttachmentSlot::Side:		Selection = ESelectionType::Side; break;
	case EAttachmentSlot::Magazine: Selection = ESelectionType::Magazine; break;
	case EAttachmentSlot::Muzzle:	Selection = ESelectionType::Muzzle; break;
	}

	// Begin our loop capped at MAX_ITERATION.
	for (int32 Index = 0; Index != MAX_ITERATION; ++Index)
	{
		// Was the DataTable set in the Blueprint?
		if (AttachmentsTable)
		{
			// Is the row at this index valid (our row name are numerical and ascending to allow us to iterate through the table.
			// If this fails, then it's either wrong Struct type or Index out of bounds.
			if (const auto AttachmentInfoPtr = AttachmentsTable->FindRow<FAttachmentInfo>(*FString::FromInt(Index), ""))
			{
				// Is it of the type we want?
				if (AttachmentInfoPtr->Slot == InAttachmentSlot)
				{
					// Was the Widget set in the Blueprint?
					if (BP_Selection)
					{
						// Create the Selection UI Template.
						USelectionSlot* Widget = CreateWidget<USelectionSlot>(GetOwningPlayer(), BP_Selection);
						Widget->InitializeObject(Selection, InAttachmentSlot, this, *AttachmentInfoPtr, bEditingPrimary);
						SelectionBox->AddChild(Widget);
					}
					else
						WRITE_DEBUG_LOG(TEXT("KitEditor -> BP_Selection Not Set!"))
				}
			}
			// If it's not valid, we break. We have no need to iterate over invalid rows.
			else
				break;
		}
		else
			WRITE_DEBUG_LOG(TEXT("KitEditor -> AttachmentsTable Not Set!"))
	}
}

void UKitEditor::PopulateAccessories(const EAccessoryType& InAccessoryType)
{
	// Local var to feed in the correct SelectionType without making another parameter.
	ESelectionType Selection = ESelectionType::Sights;
	
	switch (InAccessoryType)
	{
	case Invalid: break;
	case Grenade:	 Selection = ESelectionType::Grenade;	break;
	case Backpack:	 Selection = ESelectionType::Backpack;	break;
	case Deployable: Selection = ESelectionType::Deployable;break;
	}
	
	for (int32 Index = 0; Index != MAX_ITERATION; ++Index)
	{
		if ( const auto DeployableInfoPtr = AccessoryTable->FindRow<FAccessoryInfo>(*FString::FromInt(Index), ""))
		{
			if (DeployableInfoPtr)
			{
				if (DeployableInfoPtr->ID != CurrentKit.Deployable.ID)
				{
					if (DeployableInfoPtr->Type == InAccessoryType)
					{
						if (BP_Selection)
						{
							USelectionSlot* Widget = CreateWidget<USelectionSlot>(GetOwningPlayer(), BP_Selection);
							Widget->InitializeObject(Selection, this, *DeployableInfoPtr);
							SelectionBox->AddChild(Widget);
						}
						else
							WRITE_DEBUG_LOG(TEXT("KitEditor -> BP_Selection Not Set!"))
					}
				}
			}
			else
				break;
		}
		else
			WRITE_DEBUG_LOG(TEXT("KitEditor -> AccessoryTable Not Set!"))
	}
}

void UKitEditor::PopulateGuns(const ESelectionType& InSelectionType)
{
	for (int32 Index = 0; Index != MAX_ITERATION; ++Index)
	{
		if (GunsTable)
		{
			if (const auto GunInfoPtr = GunsTable->FindRow<FGunInfo>(*FString::FromInt(Index), ""))
			{
				// Only find the ones that we don't currently have equipped in our Primary and Secondary slot.
				if (GunInfoPtr->ID != CurrentKit.Secondary.ID && GunInfoPtr->ID != CurrentKit.Primary.ID )
				{
					if (BP_Selection)
					{
						USelectionSlot* Widget = CreateWidget<USelectionSlot>(GetOwningPlayer(), BP_Selection);
						Widget->InitializeObject(InSelectionType, this, *GunInfoPtr);
						SelectionBox->AddChild(Widget);
					}
					else
						WRITE_DEBUG_LOG(TEXT("KitEditor -> BP_Selection Not Set!"))
				}
			}
			else
				break;
		}
		else
			WRITE_DEBUG_LOG(TEXT("KitEditor -> GunsTable Not Set!"))
	}
}

void UKitEditor::ToggleButtons(UButton* ButtonToDisable)
{
	// Iterate through our Button array and enable all that aren't the button to disable.
	for (UButton* Element : FilterButtons)
	{
		if (Element != ButtonToDisable)
			Element->SetIsEnabled(true);
	}
	// Then disable the wanted button.
	ButtonToDisable->SetIsEnabled(false);
}

void UKitEditor::UpdateContent(const FKitInfo& NewKitInfo) const
{
	// Updates ALL UI ELEMENTS with the new Kit data.
	PrimaryWeaponName->SetText(NewKitInfo.Primary.Name);
	SecondaryWeaponName->SetText(NewKitInfo.Secondary.Name);	
	GrenadeAccessoryName->SetText(NewKitInfo.Grenade.Name);
	BackpackAccessoryName->SetText(NewKitInfo.Backpack.Name);
	DeployableAccessoryName->SetText(NewKitInfo.Deployable.Name);
	CurrentSightsAttachedPrimary->SetText(NewKitInfo.Primary.SightsAttachment.Name);
	CurrentSideAttachedPrimary->SetText(NewKitInfo.Primary.SideAttachment.Name);
	CurrentMagazineAttachedPrimary->SetText(NewKitInfo.Primary.MagazineAttachment.Name);
	CurrentMuzzleAttachedPrimary->SetText(NewKitInfo.Primary.MuzzleAttachment.Name);
	CurrentSightsAttachedSecondary->SetText(NewKitInfo.Secondary.SightsAttachment.Name);
	CurrentSideAttachedSecondary->SetText(NewKitInfo.Secondary.SideAttachment.Name);
	CurrentMagazineAttachedSecondary->SetText(NewKitInfo.Secondary.MagazineAttachment.Name);
	CurrentMuzzleAttachedSecondary->SetText(NewKitInfo.Secondary.MuzzleAttachment.Name);
	PrimaryWeaponIcon->SetBrushFromTexture(NewKitInfo.Primary.Icon, true);
	SecondaryWeaponIcon->SetBrushFromTexture(NewKitInfo.Secondary.Icon, true);
	GrenadeAccessoryIcon->SetBrushFromTexture(NewKitInfo.Grenade.Icon, true);
	BackpackAccessoryIcon->SetBrushFromTexture(NewKitInfo.Backpack.Icon, true);
	DeployableAccessoryIcon->SetBrushFromTexture(NewKitInfo.Deployable.Icon, true);
}

void UKitEditor::UpdateContent(const FGunInfo& NewGun, const bool bIsPrimary)
{
	// Update ONLY GUN UI CONTENT, either primary or secondary.
	if (bIsPrimary)
	{
		CurrentKit.Primary = NewGun;
		PrimaryWeaponName->SetText(CurrentKit.Primary.Name);
		PrimaryWeaponIcon->SetBrushFromTexture(CurrentKit.Primary.Icon, true);
	}
	else
	{
		CurrentKit.Secondary = NewGun;
		SecondaryWeaponName->SetText(CurrentKit.Secondary.Name);
		SecondaryWeaponIcon->SetBrushFromTexture(CurrentKit.Secondary.Icon, true);
	}
}

void UKitEditor::UpdateContent(const FAccessoryInfo& NewAccessory, const EAccessoryType& NewAccessoryType)
{
	// UPDATE ONLY UI ACCESSORY CONTENT ON ENUM SWITCH
	switch (NewAccessoryType)
	{
	case Invalid:
		break;
	case Grenade:
		CurrentKit.Grenade = NewAccessory;
		GrenadeAccessoryName->SetText(CurrentKit.Grenade.Name);
		GrenadeAccessoryIcon->SetBrushFromTexture(CurrentKit.Grenade.Icon, true);
		break;
	case Backpack:
		CurrentKit.Backpack = NewAccessory;
		BackpackAccessoryName->SetText(CurrentKit.Backpack.Name);
		BackpackAccessoryIcon->SetBrushFromTexture(CurrentKit.Backpack.Icon, true);
		break;
	case Deployable:
		CurrentKit.Deployable = NewAccessory;
		DeployableAccessoryName->SetText(CurrentKit.Deployable.Name);
		DeployableAccessoryIcon->SetBrushFromTexture(CurrentKit.Deployable.Icon, true);
		break;
	}
}

void UKitEditor::UpdateContent(const FAttachmentInfo& NewAttachment, const EAttachmentSlot& InSlot, bool bIsPrimary)
{
	// UPDATE ONLY UI ATTACHMENT CONTENT ON ENUM SWITCH
	switch (InSlot)
	{
	case EAttachmentSlot::Sights:
		if (bIsPrimary)
		{
			CurrentSightsAttachedPrimary->SetText(NewAttachment.Name);
			CurrentKit.Primary.SightsAttachment = NewAttachment;
		}
		else
		{
			CurrentSightsAttachedSecondary->SetText(NewAttachment.Name);
			CurrentKit.Secondary.SightsAttachment = NewAttachment;
		}
		break;
	case EAttachmentSlot::Side:
		if (bIsPrimary)
		{
			CurrentSideAttachedPrimary->SetText(NewAttachment.Name);
			CurrentKit.Primary.SideAttachment = NewAttachment;
		}
		else
		{
			CurrentSideAttachedSecondary->SetText(NewAttachment.Name);
			CurrentKit.Secondary.SideAttachment = NewAttachment;
		}
		break;
	case EAttachmentSlot::Magazine:
		if (bIsPrimary)
		{
			CurrentMagazineAttachedPrimary->SetText(NewAttachment.Name);
			CurrentKit.Primary.MagazineAttachment = NewAttachment;
		}
		else
		{
			CurrentMagazineAttachedSecondary->SetText(NewAttachment.Name);
			CurrentKit.Secondary.MagazineAttachment = NewAttachment;
		}
		break;
	case EAttachmentSlot::Muzzle:
		if (bIsPrimary)
		{
			CurrentMuzzleAttachedPrimary->SetText(NewAttachment.Name);
			CurrentKit.Primary.MuzzleAttachment = NewAttachment;
		}
		else
		{
			CurrentMuzzleAttachedSecondary->SetText(NewAttachment.Name);
			CurrentKit.Secondary.MuzzleAttachment = NewAttachment;
		}
		break;
	default: ;
	}
}

void UKitEditor::CloseSelection()
{
	SelectionBox->ClearChildren();
	SelectionCanvas->SetVisibility(ESlateVisibility::Hidden);
}