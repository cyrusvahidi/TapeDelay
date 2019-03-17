/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapeDelayAudioProcessorEditor::TapeDelayAudioProcessorEditor (TapeDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    for (auto paramID : { Parameters::delayTime, Parameters::wetMix, Parameters::readHeadMix, Parameters::feedback })
        addFloatParameter (paramID);
    
    setSize (600, 300);
}

TapeDelayAudioProcessorEditor::~TapeDelayAudioProcessorEditor()
{
}

//==============================================================================
void TapeDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void TapeDelayAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced (10);
    bounds.removeFromBottom (10);

    static constexpr int sliderWidth = 120;
    static constexpr int sliderGap = 5;
    
    auto paramSlice = bounds.removeFromBottom (250);
    
    auto getSliderForParameter = [this] (const Identifier& paramID) -> RotarySliderWithLabel*
    {
        for (auto* s : parameterSliders)
            if (s->identifier == paramID)
                return s;
        
        jassertfalse;
        return nullptr;
    };
    
    for (auto paramID : { Parameters::delayTime, Parameters::wetMix, Parameters::readHeadMix, Parameters::feedback })
    {
        getSliderForParameter (paramID)->setBounds (paramSlice.removeFromLeft (sliderWidth));
        paramSlice.removeFromLeft (sliderGap);
    }

}

void TapeDelayAudioProcessorEditor::addFloatParameter (const Identifier& paramID)
{
    auto* sliderComp = parameterSliders.add (new RotarySliderWithLabel (paramID));
    addAndMakeVisible (sliderComp);
    
    parameterSliderAttachments.add (new SliderAttachment (processor.getState(), paramID.toString(), sliderComp->slider));
}
