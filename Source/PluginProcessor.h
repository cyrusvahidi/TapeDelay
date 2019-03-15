/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>

//==============================================================================
/**
*/
namespace Parameters
{
    static const Identifier delayTime  { "delayTime" };
    static const Identifier wetMix     { "wetMix" };
    static const Identifier feedback   { "feedback" };
    static const Identifier tapMix     { "tapMix" };
    
    struct ParameterInfo
    {
        String labelName;
        float defaultValue;
        float min;
        float max;
        float increment;
    };
    
    static std::map<Identifier, ParameterInfo> parameterInfoMap
    {
        { delayTime,   { "Delay Time", 0.5f, 0.f, 1.0f, 0.01f } },
        { wetMix,      { "Dry/Wet",    0.5f, 0.f, 1.0f, 0.01f } },
        { feedback,    { "Feedback",   0.5f, 0.f, 1.0f, 0.01f } },
        { tapMix,      { "Tap Mix",    0.5f, 0.f, 1.0f, 0.01f } }
    };
}

class TapeDelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    TapeDelayAudioProcessor();
    ~TapeDelayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

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
    //==============================================================================
    AudioParameterFloat* delayTime = nullptr;
    AudioParameterFloat* wetMix    = nullptr;
    AudioParameterFloat* tapMix    = nullptr;
    AudioParameterFloat* feedback    = nullptr;
    
    LinearSmoothedValue<float> rampedDelayTime, rampedWetMix, rampedTapMix, rampedFeedback;
    
    AudioProcessorValueTreeState state;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeDelayAudioProcessor)
};
