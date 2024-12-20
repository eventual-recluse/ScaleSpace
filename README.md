# ScaleSpace

ScaleSpace is a prototype [MTS-ESP](https://github.com/ODDSound/MTS-ESP) master plugin, which allows the dynamic manipulation of the current scale by using an XY slider to explore the 'scale space' between four scales.

![ScaleSpace](https://raw.githubusercontent.com/eventual-recluse/ScaleSpace/main/plugins/ScaleSpace/ScaleSpace_screenshot.png "ScaleSpace")<br/>

# Settings

Each of the four scales can be set by loading a either a Scala scale file (.scl), keymapping file (.kbm) file, or both. Click "Open SCL File" or "Open KBM File" to choose the file.

The four corners of the XY slider represent each of the four scales, for example, moving the slider to the top left corner will set the current scale to Scale 1, and moving it to the bottom right corner will set the current scale to Scale 4. Different positions within the square will set the current scale to a different weighted average of the four scales.

The current scale can be saved as a .scl and .kbm pair by pressing the EXPORT button. To allow for any scale, the .scl file defines every MIDI note from 0 to 127 as a difference in cents from the reference note frequency. The .scl assumes that the reference note will be MIDI note 60, so the first listed pitch difference is assumed to be for MIDI note 61. The .kbm file sets the reference frequency at MIDI note 60, using the frequency of MIDI note 60 in the current scale. The .kbm file also sets key-for-key mapping. **NOTE: Attempting to overwrite either the .scl or .kbm will overwrite both files, as they are saved as a pair.**

# Notes

To use these plugins, you will need Scala scale files (.scl) and / or keymapping files (.kbm). You will also need to install [libMTS.](https://github.com/ODDSound/MTS-ESP)

There is a large collection of .scl files at the [Scala Scale Archive.](https://huygens-fokker.org/microtonality/scales.html)

A collection of .scl and .kbm files can be found in the [Sevish Tuning Pack.](https://sevish.com/music-resources/#tuning-files)

# Builds
Builds can be found at [Scale-Plugin-Builds.](https://github.com/eventual-recluse/Scale-Plugin-Builds)

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
