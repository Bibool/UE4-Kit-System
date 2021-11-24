

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "_Types/Structs/AttachmentInfo.h"
#include "GunPreview.generated.h"

/**
 *	This actor is a simple actor that previews the gun being edited.
 */

class AMenuPlayerController;
UCLASS()
class FPSK_API AGunPreview : public AActor
{
	GENERATED_BODY()
	
public:	
	AGunPreview();

	/** Initializes this object.
	 *	@param ThisOwner The PlayerController spawning this actor. */
	void InitializeObject(AMenuPlayerController* ThisOwner);
	
	/** Updates the main Mesh.
	 *	@param NewMesh The new mesh to show. */
	void UpdateMesh(USkeletalMesh* NewMesh) const;
	
	/** Updates the attachment on the mesh.
	 *	@param NewAttachment The mesh of the new attachment.
	 *	@param NewType and the slot to place it in.	 */
	void UpdateAttachment(USkeletalMesh* NewAttachment, const EAttachmentSlot& NewType ) const;
	
	/** Clears the attachment.
	 *	@param InSlot The slot we are clearing the attachment in. */
	void ClearAttachment(const EAttachmentSlot& InSlot);
	
	/** Rotates the Mesh with the given delta.
	 *	@param Delta Rotation speed. */
	void Rotate(float Delta) const;

private:
	UPROPERTY(EditDefaultsOnly) USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly) USkeletalMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly) USkeletalMeshComponent* SightsMesh;
	UPROPERTY(EditDefaultsOnly) USkeletalMeshComponent* SideMesh;
	UPROPERTY(EditDefaultsOnly) USkeletalMeshComponent* MagazineMesh;
	UPROPERTY(EditDefaultsOnly) USkeletalMeshComponent* MuzzleMesh;
	UPROPERTY() AMenuPlayerController* OwnerPlayer;
};
