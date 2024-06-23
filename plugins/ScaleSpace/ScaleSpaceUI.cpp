/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2021 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <string>
#include "DistrhoUI.hpp"
#include "ResizeHandle.hpp"
#include "extra/String.hpp"
#include "ScaleSpaceControls.hpp"
#include "BrunoAceFont.hpp"
#include "BrunoAceSCFont.hpp"
#include "LektonRegularFont.hpp"
#include "Tunings.h"
#include "dear_widgets.h"

START_NAMESPACE_DISTRHO

const char* kStateKeys[kStateCount] = {
    "scl_file_1",
    "scl_file_2",
    "scl_file_3",
    "scl_file_4",
    "kbm_file_1",
    "kbm_file_2",
    "kbm_file_3",
    "kbm_file_4",
};

// --------------------------------------------------------------------------------------------------------------------

class ScaleSpaceUI : public UI
{
public:
   /**
      UI class constructor.
      The UI should be initialized to a default state that matches the plugin side.
    */
    ScaleSpaceUI()
        : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true),
          fResizeHandle(this)
    {
        setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);
        
        // hide handle if UI is resizable
        if (isResizable())
            fResizeHandle.hide();
            
        std::memset(fParameters, 0, sizeof(fParameters));
        
        // populate fParameters with defaults
        for (int32_t i = 0; i < kParameterCount; i++)
        {
            fParameters[i] = ParameterDefaults[i];
        }
        
        for (int32_t i = 0; i < kStateCount; i++)
        {
			String d(" ");
			fFileBaseName[i] = d;
		}
		
		utuning1 = Tunings::Tuning(); 
		utuning2 = Tunings::Tuning(); 
		utuning3 = Tunings::Tuning(); 
		utuning4 = Tunings::Tuning(); 
        
        // account for scaling
        scale_factor = getScaleFactor();
        if (scale_factor == 0) {scale_factor = 1.0;}
        
        UI_COLUMN_WIDTH = 312 * scale_factor;
        SCALE_BOX_WIDTH = UI_COLUMN_WIDTH * 0.7f;
        
        // Setup fonts
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        
        show_error_popup = false;
        errorText.clear();
        
        lektonRegularFont = AddLektonRegularFont(scale_factor);
        brunoAceFont = AddBrunoAceFont(scale_factor);
        brunoAceSCFont = AddBrunoAceSCFont(scale_factor);
		
        // Set style and colours
        ImGuiStyle& uistyle = ImGui::GetStyle();
        
        uistyle.GrabRounding = 3.6f;
        
        ImVec4 light_orange (0.965f, 0.714f, 0.282f, 0.8f);
        ImVec4 medium_orange (1.0f, 0.63f, 0.0f, 0.9f);
        ImVec4 pale_orange (0.965f, 0.851f, 0.655f, 0.96f);
        ImVec4 pale (0.98f, 0.98f, 0.98f, 1.00f);
        ImVec4 black (0.0f, 0.0f, 0.0f, 1.00f);
        ImVec4 pale_trans (1.0f, 1.0f, 1.0f, 0.7f);
        ImVec4 transparent (0.0f, 0.0f, 0.0f, 0.00f);
        ImVec4 grey (0.50f, 0.50f, 0.50f, 1.0f);
        
        ImVec4 light_peach (0.95f, 0.33f, 0.14f, 0.36f);
        ImVec4 peach (0.95f, 0.33f, 0.14f, 0.47f);
        ImVec4 deep_peach (0.97f, 0.31f, 0.13f, 0.81f);
        ImVec4 light_orange_opaque (1.00f, 0.79f, 0.18f, 1.0f);

        uistyle.Colors[ImGuiCol_Text] = black;
        uistyle.Colors[ImGuiCol_TextDisabled] = black;
        uistyle.Colors[ImGuiCol_WindowBg] = pale;
        uistyle.Colors[ImGuiCol_ChildBg] = transparent;
        uistyle.Colors[ImGuiCol_PopupBg] = pale;
        uistyle.Colors[ImGuiCol_Border] = grey;
        uistyle.Colors[ImGuiCol_BorderShadow] = transparent;
        uistyle.Colors[ImGuiCol_FrameBg] = light_orange;
        uistyle.Colors[ImGuiCol_FrameBgHovered] = medium_orange;
        uistyle.Colors[ImGuiCol_FrameBgActive] = pale_orange;
        uistyle.Colors[ImGuiCol_TitleBg] = light_orange_opaque;
        uistyle.Colors[ImGuiCol_TitleBgActive] = light_orange_opaque;
        uistyle.Colors[ImGuiCol_TitleBgCollapsed] = light_orange_opaque;
        uistyle.Colors[ImGuiCol_MenuBarBg] = light_orange;
        uistyle.Colors[ImGuiCol_ScrollbarBg] = light_orange;
        uistyle.Colors[ImGuiCol_ScrollbarGrab] = pale_trans;
        uistyle.Colors[ImGuiCol_ScrollbarGrabHovered] = peach;
        uistyle.Colors[ImGuiCol_ScrollbarGrabActive] = deep_peach;
        uistyle.Colors[ImGuiCol_CheckMark] = pale;
        uistyle.Colors[ImGuiCol_SliderGrab] = pale_trans;
        uistyle.Colors[ImGuiCol_SliderGrabActive] = pale;
        uistyle.Colors[ImGuiCol_Button] = light_orange;
        uistyle.Colors[ImGuiCol_ButtonHovered] = medium_orange;
        uistyle.Colors[ImGuiCol_ButtonActive] = pale_orange;
        uistyle.Colors[ImGuiCol_Header] = light_orange;
        uistyle.Colors[ImGuiCol_HeaderHovered] = medium_orange;
        uistyle.Colors[ImGuiCol_HeaderActive] = pale_orange;
        uistyle.Colors[ImGuiCol_Separator] = pale;
        uistyle.Colors[ImGuiCol_SeparatorHovered] = pale;
        uistyle.Colors[ImGuiCol_SeparatorActive] = pale;
        uistyle.Colors[ImGuiCol_ResizeGrip] = light_orange;
        uistyle.Colors[ImGuiCol_ResizeGripHovered] = medium_orange;
        uistyle.Colors[ImGuiCol_ResizeGripActive] = pale_orange;
        uistyle.Colors[ImGuiCol_Tab] = black;
        uistyle.Colors[ImGuiCol_TabHovered] = black;
        uistyle.Colors[ImGuiCol_TabActive] = black;
        uistyle.Colors[ImGuiCol_TabUnfocused] = black;
        uistyle.Colors[ImGuiCol_TabUnfocusedActive] = black;
        uistyle.Colors[ImGuiCol_PlotLines] = black;
        uistyle.Colors[ImGuiCol_PlotLinesHovered] = black;
        uistyle.Colors[ImGuiCol_PlotHistogram] = black;
        uistyle.Colors[ImGuiCol_PlotHistogramHovered] = black;
        uistyle.Colors[ImGuiCol_TableHeaderBg] = black;
        uistyle.Colors[ImGuiCol_TableBorderStrong] = black;
        uistyle.Colors[ImGuiCol_TableBorderLight] = black;
        uistyle.Colors[ImGuiCol_TableRowBg] = black;
        uistyle.Colors[ImGuiCol_TableRowBgAlt] = black;
        uistyle.Colors[ImGuiCol_TextSelectedBg] = medium_orange;
        uistyle.Colors[ImGuiCol_DragDropTarget] = black;
        uistyle.Colors[ImGuiCol_NavHighlight] = black;
        uistyle.Colors[ImGuiCol_NavWindowingHighlight] = black;
        uistyle.Colors[ImGuiCol_NavWindowingDimBg] = black;
        uistyle.Colors[ImGuiCol_ModalWindowDimBg] =  black;
    }

protected:
    // ----------------------------------------------------------------------------------------------------------------
    // DSP/Plugin Callbacks

   /**
      A parameter has changed on the plugin side.@n
      This is called by the host to inform the UI about parameter changes.
    */
    void parameterChanged(uint32_t index, float value) override
    {
        fParameters[index] = value;
		
        repaint();
    }

   /**
      A state has changed on the plugin side.@n
      This is called by the host to inform the UI about state changes.
    */
    void stateChanged(const char* key, const char* value) override
    {
		States stateId = kStateCount;

        /**/ if (std::strcmp(key, "scl_file_1") == 0)
            stateId = kStateFileSCL1;
        else if (std::strcmp(key, "scl_file_2") == 0)
            stateId = kStateFileSCL2;
        else if (std::strcmp(key, "scl_file_3") == 0)
            stateId = kStateFileSCL3;
        else if (std::strcmp(key, "scl_file_4") == 0)
            stateId = kStateFileSCL4;
        else if (std::strcmp(key, "kbm_file_1") == 0)
            stateId = kStateFileKBM1;
        else if (std::strcmp(key, "kbm_file_2") == 0)
            stateId = kStateFileKBM2;
        else if (std::strcmp(key, "kbm_file_3") == 0)
            stateId = kStateFileKBM3;
        else if (std::strcmp(key, "kbm_file_4") == 0)
            stateId = kStateFileKBM4;

        if (stateId == kStateCount)
            return;
        
        fState[stateId] = value;
        
        // NOTE: We will mirror what's happening on the DSP side

        if (stateId == 0)
		{	
			checkScl(utuning1, value, stateId);
		}
		else if (stateId == 1)
		{
			checkScl(utuning2, value, stateId);
		}
		else if (stateId == 2)
		{
			checkScl(utuning3, value, stateId);
		}	
		else if (stateId == 3)
		{	
			checkScl(utuning4, value, stateId);
		}		
		else if (stateId == 4)
		{
			checkKbm(utuning1, value, stateId);
		}
		else if (stateId == 5)
		{
			checkKbm(utuning2, value, stateId);
		}
		else if (stateId == 6)
		{
			checkKbm(utuning3, value, stateId);
		}
		else if (stateId == 7)
		{
			checkKbm(utuning4, value, stateId);
		}
	
        repaint();
    }
	
	void checkScl(Tunings::Tuning & tn, const char* value, const States & stateId)
    {
		String filename(value);
		auto k = tn.keyboardMapping;
		
		if (filename.endsWith(".scl"))
		{
			try
			{   auto s = Tunings::readSCLFile(value);
				tn = Tunings::Tuning(s, k);
                const char *a = tn.scale.name.c_str();
                fFileBaseName[stateId] = getFileBaseName(a);
			}
			catch (const std::exception& e)
			{
				tn = Tunings::Tuning();
				String noScl("Standard SCL tuning");
                String noKbm("Standard KBM mapping");
                fFileBaseName[stateId] = noScl;
                fFileBaseName[stateId+4] = noKbm;
                String tuningError(e.what());
                errorText = "Tuning error:\n" + tuningError + "\nScale reset to standard tuning and mapping.";
                setState(kStateKeys[stateId], "");
				setState(kStateKeys[stateId+4], "");
                show_error_popup = true;
                //d_stdout("UI:");
                //d_stdout(e.what());
			}
		}
		else
		{
			auto s = Tunings::Tuning().scale;
			tn = Tunings::Tuning(s, k);
			String noScl("Standard SCL tuning");
			fFileBaseName[stateId] = noScl;
			
			if (filename.isNotEmpty())
			{
				errorText = "Not a .scl file.\nSCL tuning reset to standard.";
				show_error_popup = true;
				setState(kStateKeys[stateId], "");
			}
		}
	}
	
	void checkKbm(Tunings::Tuning & tn, const char* value, const States & stateId)
	{
		String filename(value);
		auto s = tn.scale;
		if (filename.endsWith(".kbm"))
		{
			try
			{
				auto k = Tunings::readKBMFile(value);
				tn = Tunings::Tuning(s, k);
				const char *a = tn.keyboardMapping.name.c_str();
                fFileBaseName[stateId] = getFileBaseName(a);
			}
			catch (const std::exception& e)
			{
				tn = Tunings::Tuning();
				String noScl("Standard SCL tuning");
                String noKbm("Standard KBM mapping");
                fFileBaseName[stateId - 4] = noScl;
                fFileBaseName[stateId] = noKbm;
                String tuningError(e.what());
                errorText = "Tuning error:\n" + tuningError + "\nScale reset to standard tuning and mapping.";
                setState(kStateKeys[stateId], "");
				setState(kStateKeys[stateId-4], "");
                show_error_popup = true;
                //d_stdout("UI:");
                //d_stdout(e.what());
			}
		}
		else
		{
			auto k = Tunings::Tuning().keyboardMapping;
			tn = Tunings::Tuning(s, k);
			String noKbm("Standard KBM mapping");
			fFileBaseName[stateId] = noKbm;
			
			if (filename.isNotEmpty())
			{
				errorText = "Not a .kbm file.\nKBM mapping reset to standard.";
				show_error_popup = true;
				setState(kStateKeys[stateId], "");
			}
		}
	}
	
    String getFileBaseName(const char* value)
    {
        std::string p(value);
        // TODO Find a better way to derive the file name!
        p = p.substr(p.find_last_of("/\\") +1);
        const char *a = p.c_str();
        String baseName(a);
        return baseName;
    }
    
    // ----------------------------------------------------------------------------------------------------------------
    // Widget Callbacks

   /**
      ImGui specific onDisplay function.
    */
    void onImGuiDisplay() override
    {
		const float width = getWidth();
        const float height = getHeight();
        const float margin = 10.0f * getScaleFactor();

        ImGui::SetNextWindowPos(ImVec2(margin, margin));
        ImGui::SetNextWindowSize(ImVec2(width - 2 * margin, height - 2 * margin));

        if (ImGui::Begin("ScaleSpace", nullptr, ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar))
        {
			ImGui::BeginChild("title pane", ImVec2(0, ImGui::GetFontSize() * 3));
            
            ImGui::PushFont(brunoAceSCFont);
            //ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(40,40,40,255));
            
            ImGui::LabelText("##title_label", "ScaleSpace");
            
            //ImGui::PopStyleColor();
            ImGui::PopFont();
            
            ImGui::EndChild(); // title pane
            
            ImGui::BeginChild("top pane", ImVec2(0, 600 * scale_factor)); // top pane holds four columns (one column is space)
            
            ImGui::BeginChild("left pane", ImVec2(SCALE_BOX_WIDTH, 0));
            
            //---
            
            ImGui::BeginChild("scale one pane", ImVec2(SCALE_BOX_WIDTH, ImGui::GetFontSize() * 9.0f), true);
            
            ImGui::PushFont(brunoAceFont);
            
            ImGui::LabelText("##scale_1_label", "SCALE 1");
            
            ImGui::PopFont();
            
			if (ImGui::Button("Open SCL File##1"))
			{
				requestStateFile(kStateKeys[kStateFileSCL1]);
			}
			
			
			if (ImGui::Button("Open KBM File##1"))
			{
				requestStateFile(kStateKeys[kStateFileKBM1]);
			}
			
			ImGui::PushItemWidth(-1);
			ImGui::LabelText("##scale_1_scl", fFileBaseName[kStateFileSCL1]);
			ImGui::LabelText("##scale_1_kbm", fFileBaseName[kStateFileKBM1]);
            ImGui::PopItemWidth();
            
            ImGui::EndChild(); // scale one pane
            
            // SPACER
            ImGui::BeginChild("spacer_pane_1", ImVec2(SCALE_BOX_WIDTH, ImGui::GetFontSize() * 8.0f), false);
            
            ImGui::EndChild(); // spacer_pane_1
            
 
            //---
            
            ImGui::BeginChild("scale three pane", ImVec2(SCALE_BOX_WIDTH, ImGui::GetFontSize() * 9.0f), true);
            
            ImGui::PushFont(brunoAceFont);
            
            ImGui::LabelText("##scale_3_label", "SCALE 3");
            
            ImGui::PopFont();
            
			if (ImGui::Button("Open SCL File##3"))
			{
				requestStateFile(kStateKeys[kStateFileSCL3]);
			}
			
			
			if (ImGui::Button("Open KBM File##3"))
			{
				requestStateFile(kStateKeys[kStateFileKBM3]);
			}
			
			ImGui::PushItemWidth(-1);
			ImGui::LabelText("##scale_3_scl", fFileBaseName[kStateFileSCL3]);
			ImGui::LabelText("##scale_3_kbm", fFileBaseName[kStateFileKBM3]);
            ImGui::PopItemWidth();
            
            ImGui::EndChild(); // scale three pane
            

            
            ImGui::EndChild(); // left pane
            
            ImGui::SameLine(); 
            
            ImGui::BeginChild("spacer_vertical", ImVec2(UI_COLUMN_WIDTH * 0.2f, 0));
            
            ImGui::EndChild(); // spacer_vertical
            
            ImGui::SameLine(); 
            
            ImGui::BeginChild("middle pane", ImVec2(UI_COLUMN_WIDTH * 1.5f, 0));
            
			if (ImWidgets::Slider2DFloat(" ", &fParameters[kParameterX], &fParameters[kParameterY], controlLimits[kParameterX].first, controlLimits[kParameterX].second, controlLimits[kParameterY].first, controlLimits[kParameterY].second, 1.0f))
		    {
				if (ImGui::IsItemActivated())
                    editParameter(kParameterX, true);
                    
                setParameterValue(kParameterX, fParameters[kParameterX]);
                
                if (ImGui::IsItemActivated())
                    editParameter(kParameterY, true);
                    
                setParameterValue(kParameterY, fParameters[kParameterY]);
                    
	        }
	        
	        if (ImGui::IsItemDeactivated())
            {
                editParameter(kParameterX, false);
                editParameter(kParameterY, false);
            }
	        
            ImGui::EndChild(); // middle pane
            
            ImGui::SameLine(); 
            
            ImGui::BeginChild("right pane", ImVec2(UI_COLUMN_WIDTH, 0));
            
            
            //---
            
            ImGui::BeginChild("scale two pane", ImVec2(SCALE_BOX_WIDTH, ImGui::GetFontSize() * 9.0f), true);
            
            ImGui::PushFont(brunoAceFont);
            
            ImGui::LabelText("##scale_2_label", "SCALE 2");
            
            ImGui::PopFont();
            
			if (ImGui::Button("Open SCL File##2"))
			{
				requestStateFile(kStateKeys[kStateFileSCL2]);
			}
			
			
			if (ImGui::Button("Open KBM File##2"))
			{
				requestStateFile(kStateKeys[kStateFileKBM2]);
			}
			
			ImGui::PushItemWidth(-1);
			ImGui::LabelText("##scale_2_scl", fFileBaseName[kStateFileSCL2]);
			ImGui::LabelText("##scale_2_kbm", fFileBaseName[kStateFileKBM2]);
            ImGui::PopItemWidth();
            
            ImGui::EndChild(); // scale two pane
            
            
             // SPACER
            ImGui::BeginChild("spacer_pane_2", ImVec2(SCALE_BOX_WIDTH, ImGui::GetFontSize() * 8.0f), false);
            
            ImGui::EndChild(); // spacer_pane_2
            
            //---
            
            ImGui::BeginChild("scale four pane", ImVec2(SCALE_BOX_WIDTH, ImGui::GetFontSize() * 9.0f), true);
            
            ImGui::PushFont(brunoAceFont);
            
            ImGui::LabelText("##scale_4_label", "SCALE 4");
            
            ImGui::PopFont();
            
			if (ImGui::Button("Open SCL File##4"))
			{
				requestStateFile(kStateKeys[kStateFileSCL4]);
			}
			
			
			if (ImGui::Button("Open KBM File##4"))
			{
				requestStateFile(kStateKeys[kStateFileKBM4]);
			}
			
			ImGui::PushItemWidth(-1);
			ImGui::LabelText("##scale_4_scl", fFileBaseName[kStateFileSCL4]);
			ImGui::LabelText("##scale_4_kbm", fFileBaseName[kStateFileKBM4]);
            ImGui::PopItemWidth();
            
            ImGui::EndChild(); // scale four pane
            
            ImGui::EndChild(); // right pane
            
            
            ImGui::EndChild(); // top pane
            
            // Error popup
			if (show_error_popup)
				ImGui::OpenPopup("error_popup");
			if (ImGui::BeginPopup("error_popup"))
			{
				ImGui::PushFont(lektonRegularFont);
				ImGui::Text(errorText);
				ImGui::PopFont();
				show_error_popup = false;
				ImGui::EndPopup();
			}
            
		}
		ImGui::End();
    }

    // -------------------------------------------------------------------------------------------------------

private:
    // Parameters
    float fParameters[kParameterCount];

    // State (files)
    String fState[kStateCount];
    String fFileBaseName[kStateCount];
    
    Tunings::Tuning utuning1, utuning2, utuning3, utuning4;

    // UI stuff
    double scale_factor;
    int UI_COLUMN_WIDTH;
    int SCALE_BOX_WIDTH;
    ResizeHandle fResizeHandle;
    ImFont* brunoAceFont;
    ImFont* brunoAceSCFont;
	ImFont* lektonRegularFont;
	
	bool show_error_popup;
    String errorText;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScaleSpaceUI)
};

// --------------------------------------------------------------------------------------------------------------------

UI* createUI()
{
    return new ScaleSpaceUI();
}

// --------------------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
