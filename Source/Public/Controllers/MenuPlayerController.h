

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

class AGunPreview;
class UKitSelection;

/**
 *	This Player Controller is responsible for input in the main menu.
 *	Current uses is to feed input into the Gun Preview but any other input required should be added here too.
 */

UCLASS()
class FPSK_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Gets the current GunPreview Actor. */
	AGunPreview* GetGunPreview() const { return GunPreviewActor; }
	
private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/** Bound to Pause event. Pauses the game. WIP. */
	UFUNCTION() void OnPause();
	/** Bound to RotatePreviewWeapon axis. Rotates the actor. */
	UFUNCTION() void RotatePreviewWeapon(float Val);
	/** Bound to RegisterClick axis. Required as the LMB MUST be pressed in order to rotate the gun. */
	UFUNCTION() void RegisterClick(float Val);
	/** @return Whether or not the key is being pressed. */
	static bool IsPressingKey(float InAxisValue );

	
	// Stores if we are pressing the LMB or not.
	bool bIsPressingClick = false;
	
	// The Widget BP for KitSelection.
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> BPW_KitSelection;
	UPROPERTY() UKitSelection* KitSelection;	// and it's ptr.
	// The BP for the GunPreview Actor.
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> BP_GunPreview;
	UPROPERTY() AGunPreview* GunPreviewActor;	// and it's ptr.

	// The rotation speed multiplier for the Preview rotation.
	UPROPERTY() float GunPreviewRotationSpeed = 20.f;
};
