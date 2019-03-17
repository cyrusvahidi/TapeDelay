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
    
    int delayReadPosition, delayWritePosition;
    
    AudioProcessorValueTreeState state;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeDelayAudioProcessor)
};
