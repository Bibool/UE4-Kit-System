
#include "UI/KitSelection.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "FPSK/Debug.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Saves/KitsSave.h"
#include "UI/KitEditor.h"
#include "UI/Templates/KitSlot.h"
#include "_Types/Structs/KitInfo.h"

#ifndef MAX_KITS
	#define MAX_KITS MaxKits
#endif

#ifndef STARTING_KIT_INDEX
	#define STARTING_KIT_INDEX StartingKit
#endif


void UKitSelection::InitializeObject(const bool InGame)
{
	// This widget is shared by both the Pause menu and the Main menu.
	// Therefore we want to hide the Edit Kit button if it's opened while in-game.
	// This also allows us to optimize the delegate by NOT binding if it's in-game.
	bInGame = InGame;
	
	if (InGame)
		EditKitButton->SetVisibility(ESlateVisibility::Hidden);
	else
	{
		if (!EditKitButton->OnPressed.IsBound())
			EditKitButton->OnPressed.AddDynamic(this, &UKitSelection::OnClickedEditKitButton);
	}

	// Does the save game not exist? If so, we create it. This happens once.
	// After creating the save object, we update it's data to the DataTable structs ( defaults 4 kits ).
	if (!UGameplayStatics::DoesSaveGameExist(KitDataSaveSlotName, 0))
	{
		const auto Save = Cast<UKitsSave>(UGameplayStatics::CreateSaveGameObject(UKitsSave::StaticClass()));		
		for (int32 Index = 0; Index != MAX_KITS; ++Index)
		{
			if (const auto Kit = KitDataTable->FindRow<FKitInfo>(*FString::FromInt(Index), ""))
			{
				Save->AddKit(*Kit);
			}
			else
				WRITE_DEBUG_LOG(TEXT("KitSelection -> KitDataTable Not Set!"))
		}
		// The current kit is the first (for kit selection purposes).
		Save->SetCurrentKit(Save->GetCurrentKits()[STARTING_KIT_INDEX]);
		
		// We then save it to the slot, finishing our process.
		UGameplayStatics::SaveGameToSlot(Save, KitDataSaveSlotName, 0);
	}
	// Initialize the UI content.
	InitializeContent();
}

void UKitSelection::InitializeContent()
{
	// We load the data from the slot (if it's valid, only way for it not to be valid is the user deleting .sav)
	if ( const auto Load = Cast<UKitsSave>(UGameplayStatics::LoadGameFromSlot(KitDataSaveSlotName, 0)))
	{
		// In case this is being called while coming back from the editor, we clear the children.
		KitsBox->ClearChildren();

		// We create a widget for each kit slots (maximum of 4 - unless specified otherwise)
		for (int32 Index = 0; Index != 4; ++Index)
		{
			if ( UKitSlot* Widget = CreateWidget<UKitSlot>(GetOwningPlayer(), BP_KitSlot ))
			{
				Widget->InitializeObject(Index + 1, Load->GetCurrentKits()[Index], this);
				KitsBox->AddChild(Widget);
			}
			else
				WRITE_DEBUG_LOG(TEXT("KitSelection -> BP_KitSlot Not Set!"))
		}
		// Set our starting kit to first in array (0)
		SelectedKit = Load->GetCurrentKits()[STARTING_KIT_INDEX];
		UpdateContent(SelectedKit);		
	}
}

void UKitSelection::OnClickedEditKitButton()
{
	// Open our kit editor (if not open already) for the currently kit selected.
	if (!KitEditor)
	{
		if (BP_KitEditor)
		{
			KitEditor = CreateWidget<UKitEditor>(GetOwningPlayer(), BP_KitEditor);
			KitEditor->InitializeObject(this, SelectedKit);
			KitEditor->AddToViewport();
		}
		else
			WRITE_DEBUG_LOG(TEXT("KitSelection -> BP_KitEditor Not Set!"))
	}
}

void UKitSelection::UpdateContent(const FKitInfo& NewKitInfo)
{
	// Stores the currently selected kit.
	SelectedKit = NewKitInfo;
	// Then Updates all UI elements with the new kit info.
	// TODO: Add current mods info here somewhere?
	PrimaryWeaponName->SetText(NewKitInfo.Primary.Name);
	SecondaryWeaponName->SetText(NewKitInfo.Secondary.Name);	
	GrenadeAccessoryName->SetText(NewKitInfo.Grenade.Name);
	BackpackAccessoryName->SetText(NewKitInfo.Backpack.Name);
	DeployableAccessoryName->SetText(NewKitInfo.Deployable.Name);
	PrimaryWeaponIcon->SetBrushFromTexture(NewKitInfo.Primary.Icon, true);
	SecondaryWeaponIcon->SetBrushFromTexture(NewKitInfo.Secondary.Icon, true);
	GrenadeAccessoryIcon->SetBrushFromTexture(NewKitInfo.Grenade.Icon, true);
	BackpackAccessoryIcon->SetBrushFromTexture(NewKitInfo.Backpack.Icon, true);
	DeployableAccessoryIcon->SetBrushFromTexture(NewKitInfo.Deployable.Icon, true);

	// If we are in-game, sets the new selected kit. Currently it sets it within the save.
	// TODO: Call on player to update it's kit when the functionality is there.
	if (bInGame)
	{
		const auto Save = Cast<UKitsSave>(UGameplayStatics::CreateSaveGameObject(UKitsSave::StaticClass()));
		Save->SetCurrentKit(SelectedKit);
		UGameplayStatics::SaveGameToSlot(Save, KitDataSaveSlotName, 0);
	}
}

void UKitSelection::CloseKitEditor(UKitEditor* InWidget)
{
	// Updates the content while the Editor is still open then close the editor.
	InitializeContent();
	InWidget->RemoveFromParent();
	KitEditor = nullptr;
}