# ScaleSpace

ScaleSpace is a prototype [MTS-ESP](https://github.com/ODDSound/MTS-ESP) master plugin, which allows the dynamic manipulation of the current scale by using an XY slider to explore the 'scale space' between four scales.

![ScaleSpace](https://github.com/eventual-recluse/ScaleSpace/blob/main/plugins/ScaleSpace/ScaleSpace_screenshot.png "ScaleSpace")<br/>

# Settings

Each of the four scales can be set by loading a either a Scala scale file (.scl), keymapping file (.kbm) file, or both. Click "Open SCL File" or "Open KBM File" to choose the file.

The four corners of the XY slider represent each of the four scales, for example, moving the slider to the top left corner will set the current scale to Scale 1, and moving it to the bottom right corner will set the current scale to Scale 4. Different positions within the square will set the current scale to a different weighted average of the four scales.

# Build instructions

## Build Instructions: Ubuntu
Install dependencies, clone this repository, enter the cloned repository, then make.
```
sudo apt-get install build-essential git pkg-config freeglut3-dev
git clone --recursive https://github.com/eventual-recluse/ScaleSpace.git
cd ScaleSpace
make
```
After building, the plugins can be found in the 'bin' folder.

## Build Instructions: Windows.
ScaleSpace can be built using [msys2](https://www.msys2.org/)
After installing msys2, launch the MinGW64 shell and enter the following commands to install dependencies, clone this repository, enter the cloned repository, then make.
```
pacman -S base-devel git mingw-w64-x86_64-gcc mingw-w64-x86_64-freeglut
git clone --recursive https://github.com/eventual-recluse/ScaleSpace.git
cd ScaleSpace
make
```
After building, the plugins can be found in the 'bin' folder.

## Building on macOS.
ScaleSpace can be built on macOS using the Xcode Command Line Tools.

Install Xcode Command Line Tools from the Terminal:
```
xcode-select --install
```
Then enter the following commands in the Terminal to clone this repository, enter the cloned repository, then make.
```
git clone --recursive https://github.com/eventual-recluse/ScaleSpace.git
cd ScaleSpace
make
```
After building, the plugins can be found in the 'bin' folder.

# Credits
[DISTRHO Plugin Framework.](https://github.com/DISTRHO/DPF) ISC license.

[MTS-ESP.](https://github.com/ODDSound/MTS-ESP) 0BSD license.

[Surge Synthesizer Tuning Library.](https://github.com/surge-synthesizer/tuning-library) MIT license.

[Dear ImGui.](https://github.com/ocornut/imgui) MIT license.

[DearWidgets](https://github.com/soufianekhiat/DearWidgets) for the XY slider widget. CC0-1.0 license.

[Bruno Ace Font](https://fonts.google.com/specimen/Bruno+Ace) designed by Astigmatic. [Open Font License.](https://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL)

[Bruno Ace SC Font](https://fonts.google.com/specimen/Bruno+Ace+SC) designed by Astigmatic. [Open Font License.](https://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL)

[Lekton Font](https://fonts.google.com/specimen/Lekton) designed by ISIA Urbino. [Open Font License.](https://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL)

[DPF-Widgets.](https://github.com/DISTRHO/DPF-Widgets) License: see plugins/ScaleSpace/lib/DPFDearImGuiWidgets/LICENSE
