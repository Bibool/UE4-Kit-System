


#include "Controllers/MenuPlayerController.h"

#include "Actors/GunPreview.h"
#include "Blueprint/UserWidget.h"
#include "FPSK/Debug.h"
#include "UI/KitSelection.h"

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Creates the gun preview out of the world for transparent background.
	// TODO: Create an actor that is placed in the world and get it with GetActor()
	if (BP_GunPreview)
	{
		GunPreviewActor = GetWorld()->SpawnActor<AGunPreview>(BP_GunPreview, FVector(-398120.f,2066170.f,1201450.f), FRotator(0.f, 90.f, 0.f));
		GunPreviewActor->InitializeObject(this);
	}
	else
		WRITE_DEBUG_LOG(TEXT("MenuPlayerController -> BP_GunPreview Not Set!"))
}

void AMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	// Bind our Functions to the inputs.
	InputComponent->BindAction("Pause", IE_Pressed, this, &AMenuPlayerController::OnPause );
	InputComponent->BindAxis("RotateGun", this, &AMenuPlayerController::RotatePreviewWeapon);
	InputComponent->BindAxis("RotateGunHold", this, &AMenuPlayerController::RegisterClick);
}

void AMenuPlayerController::OnPause()
{
	if ( !KitSelection )
	{
		if (BPW_KitSelection)
		{
			// Create the widget when pressing pause.
			KitSelection = CreateWidget<UKitSelection>(this, BPW_KitSelection);
			KitSelection->InitializeObject(false);		
			KitSelection->AddToViewport();
			bShowMouseCursor = true;
			FInputModeUIOnly InputModeUIOnly;
			InputModeUIOnly.SetWidgetToFocus(KitSelection->TakeWidget());
			SetInputMode(InputModeUIOnly);
		}
		else
			WRITE_DEBUG_LOG(TEXT("MenuPlayerController -> BPW_KitSelection Not Set!"))
	}
}

void AMenuPlayerController::RotatePreviewWeapon(const float Val)
{
	if (IsPressingKey(Val))
	{
		// If LMB is being pressed and the GunPreviewActor is valid, rotate it.
		// TODO: Add a state that is changed when the Gun Mod screen is open instead of checking.
		if (bIsPressingClick && GunPreviewActor)
		{
			GunPreviewActor->Rotate(Val * GunPreviewRotationSpeed);
		}
	}
}

void AMenuPlayerController::RegisterClick(const float Val)
{
	bIsPressingClick = IsPressingKey(Val);
}

bool AMenuPlayerController::IsPressingKey(const float InAxisValue)
{
	return (InAxisValue) ? 1 : 0; 
}
