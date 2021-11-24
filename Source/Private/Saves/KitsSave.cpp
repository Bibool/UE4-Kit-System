


#include "Saves/KitsSave.h"


void UKitsSave::UpdateKits(TArray<FKitInfo> InLoadedKits, const FKitInfo& UpdatedKit)
{
	// Iterate through our Kits to find the one with matching ID.
	for (auto& Element : InLoadedKits)
	{
		// If we do find it, then overwrite it with the UpdatedKit.
		// Then we break out of the loop as we don't need to iterate anymore.
		if (Element.ID == UpdatedKit.ID)
		{
			Element = UpdatedKit;
			break;
		}
	}

	Kits = InLoadedKits;
}

void UKitsSave::AddKit(const FKitInfo& NewKit)
{
	// Add the new kit to the array.
	Kits.Add(NewKit);
}
