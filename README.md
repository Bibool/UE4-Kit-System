# UE4-Kit-System
A simple to use but expandable loadout system based on Star Wars First Assault built in C++.

It is fairly simple to include and use within projects.
It uses primarily DataTables to power it's logic.
It was made within the FPSK Template but uses none of it's feature. It just needs to be retargetted and that's it.
The Gun Preview is weird on the pictures as the skeletalmeshes were not sized differently (they SHOULD be uniform scaled).

A few blueprints are required to be set-up for this to be used:
Widget blueprints child of:
- UKitSelect
- UKitEditor
- UKitSlot
- USelectionSlot

Actor blueprint child of:
- AGunPreview

Player controller blueprint child of:
- AMenuPlayerController

There are UPROPERTIES to be set on them, such as widgets blueprints and values.
In your GameMode, you must set the MenuPlayerController as PlayerController (until it's been refactored into a component).

Lastly, you must make a DataTable of each type:
- KitInfo
- GunInfo
- AccessoryInfo
- AttachmentInfo

... and fill them appropriatly. Once done and all UPROPERTIES() pointing to the correct blueprints, simply press the Pause key and start viewing and editing kits!


KIT SELECT
![KIT SELECT](https://i.imgur.com/10N4OV8.png)

KIT EDITOR
![KIT EDITOR](https://i.imgur.com/TcyKsrV.png)

GUN SELECT
![GUN SELECT](https://i.imgur.com/kPpjHuF.png)

ACCESSORY SELECT
![ACCESSORY SELECT](https://i.imgur.com/5gGGMco.png)

GUN MODDING
![GUN MODDING](https://i.imgur.com/CsYWecs.png)

DATATABLES
![DATATABLES](https://i.imgur.com/HcPqoks.png)
