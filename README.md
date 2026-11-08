# Aliens Versus Predator 2 and Primal Hunt: VR
PCVR mod for Aliens Versus Predator 2 (2001) game. This is for Windows 10 upwards and Steam Link. Tested on Quest 2 and 3. Untested on Linux. **Don't use Meta Link as it doesn't support 32 bit games**. Virtual Desktop should work but untested.

Should work on medium graphics cards from Nvidia 3060 and upwards.

v0.6 is the latest version. Updating will overwrite your avp2xr.ini file, so back it up if you have some customisations you want to copy.

Short video of v0.2 in action on a Quest 2 here on [YouTube](https://youtu.be/4kNUM0h5kPE). Will update to demonstrate v0.3 soon, but can read the description in the Releases to see changes listed.

![v0.2 Released](https://github.com/Bassquake/Aliens-Versus-Predator-2-VR/blob/main/captures/thumb_AvP2VR_v0-2.jpg)

> [!CAUTION]
> You need the original installation game files and updated to v1.0.9.6. If you're installing the Primal Hunt expansion pack the original Aliens Versus Predator 2 game needs to be installed too. The 2 disc CD version can be found on eBay or maybe archive.org. This build is in playable state. Some weapon models have missing faces as they aren't designed to be looked at from different angles! I'm not primarily a programmer, this was done with a lot of help from various AI like Claude, Copilot and Gemini.

## Installation of standard Aliens Versus Predator 2
- Install the Aliens Versus Predator 2 game into its default folder location: **C:\Program Files (x86)\Fox\Aliens vs. Predator 2**
- Update it to v1.0.9.6 if it isn't already. Update can be [found here](https://www.gamefront.com/games/aliens-vs-predator-2/file/avp2-update-1-0-9-6).
- Download my **avp2vr-0.6.zip** in [Releases](https://github.com/Bassquake/Aliens-Versus-Predator-2-VR/releases) and extract to a folder somewhere on your drive.
- Go to the extracted folder and right click and **Run As Administrator** on **Install_avp2vr.bat**. This will copy the necessary files into the game folder, press any key to close the window.

## Installation of Primal Hunt expansion pack
The Primal Hunt expansion is also supported, with its own installer and launcher. It uses files from the main game, so install both.

- Install Aliens Versus Predator 2 as described above, then install Primal Hunt into its default folder location: **C:\Program Files (x86)\Fox\Aliens versus Predator 2 - Primal Hunt**
- Start Primal Hunt from its own launcher, press Play once, then quit. This creates the list of game files the VR launcher needs.
- Go to the extracted folder and right click and **Run As Administrator** on **Install_avp2vr_PrimalHunt.bat**. This will copy the necessary files into the Primal Hunt folder, press any key to close the window.
- Start the game with **AVP2VR_PrimalHunt_(SteamVR_runtime).bat** in the Primal Hunt folder, not with PrimalHunt.exe. To add it to your Steam Library, follow the steps above and set the Target Path to this file.

Each game has its own **avp2xr.ini**, so customise the one in the Primal Hunt folder for Primal Hunt. With the dual pistols, each trigger fires its own pistol.

Note: Some of the first-person weapon models new to Primal Hunt aren't set up for VR yet, so may not look right. To remove the mod, right click and **Run As Administrator** on **Uninstall_avp2vr_PrimalHunt.bat** in the Primal Hunt folder.

![Install Files](https://github.com/Bassquake/Aliens-Versus-Predator-2-VR/blob/main/captures/avp2-install-files-results.jpg)

- Enhance the Steam Library by adding cover art etc. Do the same for Primal Hunt (Steam images zip for both is in Releases):

Add the original **AVP2.exe** (in the game folder) to the Steam Library as a non-Steam game:

![Screenshot of adding non-Steam game](https://github.com/Bassquake/Aliens-Versus-Predator-2-VR/blob/main/captures/steam-add-app.png)

Edit the Properties:

![Screenshot of Steam options](https://github.com/Bassquake/Aliens-Versus-Predator-2-VR/blob/main/captures/steam-add-options.png)

Rename the shortcut seen here, set the Target Path to where **AVP2VR_(SteamVR_runtime).bat** is and set the **Include in VR Library** to on:

![Screenshot of Shortcut naming](https://github.com/Bassquake/Aliens-Versus-Predator-2-VR/blob/main/captures/steam-custom-name.png)

To customise the images in Steam Library so it looks nicer, download the extra zip file **steamvr-custom-images-avp2.zip** in Releases page, unzip the images from steamvr-custom-images-avp2.zip somewhere. Then click the gear icon and select Properties:

![Screenshot of Steam options](https://github.com/Bassquake/Aliens-Versus-Predator-2-VR/blob/main/captures/steam-add-options.png)

Choose Customisation and change images. The image files are named the same as the artwork title:

![Screenshot of Steam options](https://github.com/Bassquake/Aliens-Versus-Predator-2-VR/blob/main/captures/steam-add-images.png)

- Finally, on your headset, run Steam Link and navigate to the game in your library. Simply click Play! (Virtual Desktop is untested). Your desktop will mirror the view.

Note: You can also double click **AVP2VR_(SteamVR_runtime).bat** on the PC and it'll auto load in headset if Steam Link is already running.

## Controls
I've tried to map as many of the functions to the controllers buttons as there are a lot more than Aliens Versus Predator had! They can be customised by editing the **avp2xr.ini** located in **C:\Program Files (x86)\Fox\Aliens vs. Predator 2**.

![Control layout](https://github.com/Bassquake/Aliens-Versus-Predator-2-VR/blob/main/captures/controllers-avp2-v0.3.jpg)

## Tip
There's many customisation options in **avp2xr.ini** for controls/button mapping and a number of other settings can be found in there to customise positioning of HUD and weapons etc. Too many to list here so read the comments what each one does.

## Adjusting the HUD
New in v0.6: each part of the HUD can be moved and resized separately in the **[VR]** section of **avp2xr.ini**. Settings are read when the game starts, so restart it after a change.

- **HudWidth** / **HudDistance** / **HudHeight** size and place the whole HUD panel, as before.
- **Move a part:** `HudTopLeft`, `HudTopRight`, `HudBottomLeft`, `HudBottomRight`, `HudTop`, `HudBottom`, `HudLeft`, `HudRight` and `HudMotionTracker` each take two numbers, `x y`, in percent of the screen. Positive moves the part inward toward the centre, negative moves it out. For a part centred on an edge (`HudTop`, `HudBottom`), the other number slides it along the edge (x right, y down).
- **Move everything at once:** `HudInset` pulls every part inward, e.g. `HudInset=5 5`. Each part's own setting is added to it.
- **Resize a part:** add `Scale` to the name, e.g. `HudMotionTrackerScale=0.75` for a smaller motion tracker. 1 is the original size, from 0.25 to 3. A part shrinks toward its own corner or edge. Very small sizes lose some detail in thin lines and text.

Where each part sits:

| Species | Top left | Top right | Bottom left | Bottom right | Top centre | Bottom centre |
|---|---|---|---|---|---|---|
| Marine | Health, armour, battery, air | Flares | Motion tracker (`HudMotionTracker`) | Ammo | Boss health | |
| Predator | Health | Energy | Skull count | Air | Boss health | |
| Alien | | | | | Boss health | Health |

Example: pull the whole HUD in a little and shrink the Marine's motion tracker:
```
HudInset=4 4
HudMotionTrackerScale=0.75
```
