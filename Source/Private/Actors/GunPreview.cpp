


#include "Actors/GunPreview.h"

#include "FPSK/Debug.h"

AGunPreview::AGunPreview()
{
	PrimaryActorTick.bCanEverTick = false;

	// We create our components and attach them appropriately.
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Mesh (main model) to root.
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
	SightsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sights"));
	SideMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Side"));
	MagazineMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Magazine"));
	MuzzleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Muzzle"));

	// Attachments to Mesh (main model).
	SightsMesh->SetupAttachment(Mesh);
	SideMesh->SetupAttachment(Mesh);
	MagazineMesh->SetupAttachment(Mesh);
	MuzzleMesh->SetupAttachment(Mesh);
}

void AGunPreview::InitializeObject(AMenuPlayerController* ThisOwner)
{
	// Store the ptr to the player controller spawning this.
	OwnerPlayer = ThisOwner;

	// Attach the Attachments to their appropriate sockets.
	const FAttachmentTransformRules AttachRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	SightsMesh->AttachToComponent(Mesh, AttachRules, "SightsAttachmentPoint");
	SideMesh->AttachToComponent(Mesh, AttachRules, "SideAttachmentPoint");
	MagazineMesh->AttachToComponent(Mesh, AttachRules, "MagazineAttachmentPoint");
	MuzzleMesh->AttachToComponent(Mesh, AttachRules, "MuzzleAttachmentPoint");
}

void AGunPreview::UpdateMesh(USkeletalMesh* NewMesh) const
{
	Mesh->SetSkeletalMesh(NewMesh);
}

void AGunPreview::UpdateAttachment(USkeletalMesh* NewAttachment, const EAttachmentSlot& NewType) const
{
	if (NewAttachment)
	{
		switch (NewType)
		{
		case EAttachmentSlot::Invalid: break;
		case EAttachmentSlot::Sights:
			SightsMesh->SetSkeletalMesh(NewAttachment);
			break;
		case EAttachmentSlot::Side:
			SideMesh->SetSkeletalMesh(NewAttachment);
			break;
		case EAttachmentSlot::Magazine:
			MagazineMesh->SetSkeletalMesh(NewAttachment);
			break;
		case EAttachmentSlot::Muzzle:
			MagazineMesh->SetSkeletalMesh(NewAttachment);
			break;
		}
	}
	else
		WRITE_DEBUG_LOG(TEXT("GunPreviewActor -> Nullptr (no Mesh set)."))
}

void AGunPreview::ClearAttachment(const EAttachmentSlot& InSlot)
{
	switch (InSlot)
	{
	case EAttachmentSlot::Invalid:
		break;
	case EAttachmentSlot::Sights:
		SightsMesh->SetSkeletalMesh(nullptr);
		break;
	case EAttachmentSlot::Side:
		SideMesh->SetSkeletalMesh(nullptr);
		break;
	case EAttachmentSlot::Magazine:
		MagazineMesh->SetSkeletalMesh(nullptr);
		break;
	case EAttachmentSlot::Muzzle:
		MuzzleMesh->SetSkeletalMesh(nullptr);
		break;
	default: ;
	}

}

void AGunPreview::Rotate(const float Delta) const
{
	Mesh->AddLocalRotation(FRotator(0, Delta, 0));
}