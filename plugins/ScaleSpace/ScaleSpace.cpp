/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2020 Filipe Coelho <falktx@falktx.com>
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

#include "DistrhoPlugin.hpp"
#include "ScaleSpaceControls.hpp"
#include "Tunings.h"
#include "libMTSMaster.cpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

/**
  Plugin to demonstrate File handling within DPF.
 */
class ScaleSpace : public Plugin
{
public:
    ScaleSpace()
        : Plugin(kParameterCount, 0, kStateCount),
          sampleRate(getSampleRate())
    {
        std::memset(fParameters, 0, sizeof(fParameters));
        
        // populate fParameters with defaults
        for (int32_t i = 0; i < kParameterCount; i++)
        {
            fParameters[i] = ParameterDefaults[i];
        }
        
        sampleRateChanged(sampleRate);
        
        tuning1 = Tunings::Tuning();
        tuning2 = Tunings::Tuning();
        tuning3 = Tunings::Tuning();
        tuning4 = Tunings::Tuning();
        
        //Fill frequency arrays with default frequencies from tuning1
        
        for (int32_t i = 0; i < 128; i++)
        {
            frequencies_in_hz[i] = tuning1.frequencyForMidiNote(i);;
            target_frequencies_in_hz[i] = tuning1.frequencyForMidiNote(i);;
        }
        
        x_range_min = controlLimits[kParameterX].first;
		x_range_max = controlLimits[kParameterX].second;
		y_range_min = controlLimits[kParameterY].first;
		y_range_max = controlLimits[kParameterY].second;
		x_size = x_range_max - x_range_min;
		y_size = y_range_max - y_range_min;
    }

protected:
   /* --------------------------------------------------------------------------------------------------------
    * Information */

   /**
      Get the plugin label.
      This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
    */
    const char* getLabel() const override
    {
        return "ScaleSpace";
    }

   /**
      Get an extensive comment/description about the plugin.
    */
    const char* getDescription() const override
    {
        return "MTS-ESP Scale Space Explorer.";
    }

   /**
      Get the plugin author/maker.
    */
    const char* getMaker() const override
    {
        return "eventual-recluse";
    }

   /**
      Get the plugin homepage.
    */
    const char* getHomePage() const override
    {
        return "https://github.com/eventual-recluse/ScaleSpace";
    }

   /**
      Get the plugin license name (a single line of text).
      For commercial plugins this should return some short copyright information.
    */
    const char* getLicense() const override
    {
        return "GPL3";
    }

   /**
      Get the plugin version, in hexadecimal.
    */
    uint32_t getVersion() const override
    {
        return d_version(0, 0, 0);
    }

   /**
      Get the plugin unique Id.
      This value is used by LADSPA, DSSI and VST plugin formats.
    */
    int64_t getUniqueId() const override
    {
        return d_cconst('S', 'c', 'S', 'p');
    }

   /* --------------------------------------------------------------------------------------------------------
    * Init */

   /**
      Initialize the audio port @a index.@n
      This function will be called once, shortly after the plugin is created.
    */
    void initAudioPort(bool input, uint32_t index, AudioPort& port) override
    {
        // treat meter audio ports as stereo
        port.groupId = kPortGroupMono;

        // everything else is as default
        Plugin::initAudioPort(input, index, port);
    }

   /**
      Initialize the parameter @a index.
      This function will be called once, shortly after the plugin is created.
    */
    void initParameter(uint32_t index, Parameter& parameter) override
    {
        
        switch (index)
        {
        case kParameterX:
            parameter.name = "X";
            parameter.symbol = "x";
            parameter.hints = kParameterIsAutomatable;
            parameter.ranges.min = controlLimits[index].first;
            parameter.ranges.max = controlLimits[index].second;
            parameter.ranges.def = ParameterDefaults[index];
            break;
        case kParameterY:
            parameter.name = "Y";
            parameter.symbol = "y";
            parameter.hints = kParameterIsAutomatable;
            parameter.ranges.min = controlLimits[index].first;
            parameter.ranges.max = controlLimits[index].second;
            parameter.ranges.def = ParameterDefaults[index];
            break;
        }
    }

   /**
      Initialize the state @a index.@n
      This function will be called once, shortly after the plugin is created.
    */
    void initState(uint32_t index, State& state) override
    {
        switch (index)
        {
        case kStateFileSCL1:
            state.key = "scl_file_1";
            state.label = "SCL File 1";
            break;
        case kStateFileSCL2:
            state.key = "scl_file_2";
            state.label = "SCL File 2";
            break;
        case kStateFileSCL3:
            state.key = "scl_file_3";
            state.label = "SCL File 3";
            break;
        case kStateFileSCL4:
            state.key = "scl_file_4";
            state.label = "SCL File 4";
            break;
        case kStateFileKBM1:
            state.key = "kbm_file_1";
            state.label = "KBM File 1";
            break;
        case kStateFileKBM2:
            state.key = "kbm_file_2";
            state.label = "KBM File 2";
            break;
        case kStateFileKBM3:
            state.key = "kbm_file_3";
            state.label = "KBM File 3";
            break;
        case kStateFileKBM4:
            state.key = "kbm_file_4";
            state.label = "KBM File 4";
            break;
        }

        state.hints = kStateIsFilenamePath;
    }

   /* --------------------------------------------------------------------------------------------------------
    * Internal data */

   /**
      Get the current value of a parameter.
      The host may call this function from any context, including realtime processing.
    */
    float getParameterValue(uint32_t index) const override
    {
        return fParameters[index];
    }

   /**
      Change a parameter value.@n
      The host may call this function from any context, including realtime processing.

      This function will only be called for parameter inputs.
    */
    void setParameterValue(uint32_t index, float value) override
    {
		fParameters[index] = value;
	}

   /**
      Change an internal state @a key to @a value.
    */
    void setState(const char* key, const char* value) override
    {
		// note: internal states seem to get set as soon as file chosen by file dialog, and could end up being anything

        /**/ if (std::strcmp(key, "scl_file_1") == 0)
        {
		    loadScl(tuning1, value);
		}
        else if (std::strcmp(key, "scl_file_2") == 0)
        {   
			loadScl(tuning2, value);
		}
        else if (std::strcmp(key, "scl_file_3") == 0)
	    {
            loadScl(tuning3, value);
        }
        else if (std::strcmp(key, "scl_file_4") == 0)
	    {
            loadScl(tuning4, value);
        }
        else if (std::strcmp(key, "kbm_file_1") == 0)
	    {
            loadKbm(tuning1, value);
        }
        else if (std::strcmp(key, "kbm_file_2") == 0)
	    {
            loadKbm(tuning2, value);
        }
        else if (std::strcmp(key, "kbm_file_3") == 0)
	    {
            loadKbm(tuning3, value);
        }
        else if (std::strcmp(key, "kbm_file_4") == 0)
	    {
            loadKbm(tuning4, value);
        }
    }
    
    void loadScl(Tunings::Tuning & tn, const char* value)
    {
		String filename(value);
		auto k = tn.keyboardMapping;
		
		if (filename.endsWith(".scl"))
		{
			try
			{   auto s = Tunings::readSCLFile(value);
				tn = Tunings::Tuning(s, k);
				//d_stdout("ScaleSpace: tuning set to %s", value);
			}
			catch (const std::exception& e)
			{
				tn = Tunings::Tuning();
				d_stdout("ScaleSpace:Exception when setting tuning");
				d_stdout(e.what());
			}
		}
		else
		{
			auto s = Tunings::Tuning().scale;
			tn = Tunings::Tuning(s, k);
			//d_stdout("ScaleSpace: tuning scl reset");
		}
	}
	
	void loadKbm(Tunings::Tuning & tn, const char* value)
	{
		String filename(value);
		auto s = tn.scale;
		if (filename.endsWith(".kbm"))
		{
			try
			{
				auto k = Tunings::readKBMFile(value);
				tn = Tunings::Tuning(s, k);
				//d_stdout("ScaleSpace: tuning set to %s", value);
			}
			catch (const std::exception& e)
			{
				tn = Tunings::Tuning();
				d_stdout("ScaleSpace:Exception when setting tuning");
				d_stdout(e.what());
			}
		}
		else
		{
			auto k = Tunings::Tuning().keyboardMapping;
			tn = Tunings::Tuning(s, k);
			//d_stdout("ScaleSpace: tuning kbm reset");
		}
	}

    /* --------------------------------------------------------------------------------------------------------
    * Activate / Deactivate */
    
    void activate() override
    {
	    if (MTS_CanRegisterMaster())
			MTS_RegisterMaster();
	}
	
    void deactivate() override
    {
        MTS_DeregisterMaster();
    }
    
   /* --------------------------------------------------------------------------------------------------------
    * Audio/MIDI Processing */

   /**
      Run/process function for plugins without MIDI input.
      @note Some parameters might be null if there are no audio inputs or outputs.
    */
    void run(const float** inputs, float** outputs, uint32_t frames) override
    {
			
		// Calculated weighted average of the four scales, and set target frequencies 
		
		double freq_increment[128];
		double frame_count =  static_cast<double>(frames);

		for (uint32_t i = 0; i < 128; i++)
		{
			double freq1_c = tuning1.frequencyForMidiNote(i) * (0.5f - (fParameters[kParameterX] / x_size)) * (0.5f + (fParameters[kParameterY] / y_size));
			double freq2_c = tuning2.frequencyForMidiNote(i) * (0.5f + (fParameters[kParameterX] / x_size)) * (0.5f + (fParameters[kParameterY] / y_size));
			double freq3_c = tuning3.frequencyForMidiNote(i) * (0.5f - (fParameters[kParameterX] / x_size)) * (0.5f - (fParameters[kParameterY] / y_size));
			double freq4_c = tuning4.frequencyForMidiNote(i) * (0.5f + (fParameters[kParameterX] / x_size)) * (0.5f - (fParameters[kParameterY] / y_size));
			
			target_frequencies_in_hz[i] = freq1_c + freq2_c + freq3_c + freq4_c;
			
			freq_increment[i] = (target_frequencies_in_hz[i] - frequencies_in_hz[i]) * (1.0 / frame_count);
		}
		
		// smoothing
		for (uint32_t fr = 0; fr < frames; ++fr)
		{
			for (uint32_t i = 0; i < 128; i++)
			{
				if (fr == frames - 1)
				{
					//ensure target met
					frequencies_in_hz[i] = target_frequencies_in_hz[i];
				}
				else
				{
					frequencies_in_hz[i] += freq_increment[i];
				}
			}
			// Set MTS-ESP Scale
		    MTS_SetNoteTunings(frequencies_in_hz);
		}
   
    }
    

    // -------------------------------------------------------------------------------------------------------

private:
    float sampleRate;

    float fParameters[kParameterCount];
    Tunings::Tuning tuning1, tuning2, tuning3, tuning4;
    
    double frequencies_in_hz[128];
    double target_frequencies_in_hz[128];
    
    float x_range_min;
	float x_range_max;
	float y_range_min;
	float y_range_max;
	float x_size;
	float y_size;

   /**
      Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScaleSpace)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new ScaleSpace();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
