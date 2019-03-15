/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TapeDelayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    TapeDelayAudioProcessorEditor (TapeDelayAudioProcessor&);
    ~TapeDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void addFloatParameter (const Identifier&);
//    void addReverbEnabledParameter();
    
    struct RotarySliderWithLabel    : public Component
    {
        RotarySliderWithLabel (const Identifier& paramID)
        : identifier (paramID),
        slider (identifier.toString()),
        label  (identifier.toString(), Parameters::parameterInfoMap[paramID].labelName)
        {
            slider.setSliderStyle (Slider::Rotary);
            slider.setTextBoxStyle (Slider::TextBoxBelow, false, slider.getTextBoxWidth(), slider.getTextBoxHeight());
            addAndMakeVisible (slider);
            
            label.setJustificationType (Justification::centred);
            addAndMakeVisible (label);
            
        }
        
        void resized() override
        {
            auto bounds = getLocalBounds();
            
            label.setBounds (bounds.removeFromTop (25));
            slider.setBounds (bounds);
        }
        
        Identifier identifier;
        Slider slider;
        Label label;
    };
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TapeDelayAudioProcessor& processor;
    
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    OwnedArray<RotarySliderWithLabel> parameterSliders;
    OwnedArray<SliderAttachment> parameterSliderAttachments;

//    using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;
//    
//    ToggleButton reverbEnabledToggle;
//    std::unique_ptr<ButtonAttachment> reverbEnabledToggleAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeDelayAudioProcessorEditor)
};
