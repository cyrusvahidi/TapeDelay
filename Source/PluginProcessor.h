/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>
#include "Parameters.hpp"
//#include "DelayLine.hpp"
//namespace Parameters
//{
//    static const Identifier delayTime   { "delayTime"   };
//    static const Identifier wetMix      { "wetMix"      };
//    static const Identifier feedback    { "feedback"    };
//    static const Identifier readHeadMix { "readHeadMix" };
//    static const Identifier density     { "density"     };
//    static const Identifier lfoRate     { "lfoRate"     };
//    static const Identifier lfoAmount   { "lfoAmount"   };
//    static const Identifier tapeSpeed   { "tapeSpeed"   };
//    // TODO: add distortion control
//    // TODO: init other params in processor
//
//    struct ParameterInfo
//    {
//        String labelName;
//        float defaultValue;
//        float min;
//        float max;
//        float increment;
//    };
//
//    static std::map<Identifier, ParameterInfo> parameterInfoMap
//    {
//        { delayTime,   { "Delay Time", 0.2f, 0.f, 2.0f, 0.01f   } },
//        { wetMix,      { "Dry/Wet",    0.5f, 0.f, 1.0f, 0.01f   } },
//        { feedback,    { "Feedback",   0.5f, 0.f, 0.995f, 0.01f } },
//        { readHeadMix, { "Head Mix",   0.5f, 0.f, 1.0f, 0.01f   } },
//        { density,     { "Density",    0.5f, 0.f, 1.0f, 0.01f   } },
//        { lfoRate,     { "lfoRate",    10.f, 1.f, 20.f, 0.01f   } },
//        { lfoAmount,   { "lfoAmount",  0.f, 0.f, 1.f, 0.01f     } },
//        { tapeSpeed,   { "tapeSpeed",  0.5f, 0.f, 1.f, 0.01f    } }
//    };
//}

//==============================================================================
/**
*/

class TapeDelayAudioProcessor  : public AudioProcessor,
                                 public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    TapeDelayAudioProcessor();
    ~TapeDelayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
    void parameterChanged (const String &parameterID, float newValue) override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    AudioProcessorValueTreeState& getState();
    
private:
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    AudioProcessorValueTreeState state;
    //==============================================================================
    AudioParameterFloat* _delayTime    = nullptr;
    AudioParameterFloat* _wetMix       = nullptr;
    AudioParameterFloat* _readHeadMix  = nullptr;
    AudioParameterFloat* _feedback     = nullptr;
    AudioParameterFloat* _density      = nullptr;
    AudioParameterFloat* _lfoRate      = nullptr;
    AudioParameterFloat* _lfoAmount    = nullptr;
    AudioParameterFloat* _tapeSpeed    = nullptr;
    
    LinearSmoothedValue<float> rampedDelayTime, rampedWetMix, rampedReadHeadMix, rampedFeedback;
    
    AudioBuffer<float> delayBuffer;
    int delayBufferLength;
    
//    DelayLine delayLine;
    
    int delayReadPosition, delayWritePosition;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeDelayAudioProcessor)
};
