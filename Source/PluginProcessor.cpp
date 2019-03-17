/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessorValueTreeState::ParameterLayout TapeDelayAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    {
        using FloatParamPair = std::pair<Identifier, AudioParameterFloat*&>;
        
        for (auto p : { FloatParamPair (Parameters::delayTime, _delayTime),
            FloatParamPair (Parameters::wetMix,     _wetMix),
            FloatParamPair (Parameters::readHeadMix, _readHeadMix),
            FloatParamPair (Parameters::feedback, _feedback)
        })
        {
            auto& info = Parameters::parameterInfoMap[p.first];
            auto param = std::make_unique<AudioParameterFloat> (p.first.toString(), info.labelName, NormalisableRange<float>(info.min, info.max, info.increment), info.defaultValue);
            
            p.second = param.get();
            params.push_back (std::move (param));
        }
    }
    
    return { params.begin(), params.end() };
}

//==============================================================================
TapeDelayAudioProcessor::TapeDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
        delayBuffer(2, 1),
        state(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    state.addParameterListener (Parameters::delayTime, this);
    state.addParameterListener (Parameters::feedback, this);
    state.addParameterListener (Parameters::wetMix, this);
    
    delayBufferLength = 1;
    
    delayReadPosition = 0;
    delayWritePosition = 0;
}

TapeDelayAudioProcessor::~TapeDelayAudioProcessor()
{
}

//==============================================================================
const String TapeDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TapeDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TapeDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TapeDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TapeDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TapeDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TapeDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TapeDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String TapeDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void TapeDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TapeDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    float rampLengthInSeconds = 1;
    rampedDelayTime.reset(sampleRate, rampLengthInSeconds);
    rampedWetMix.reset(sampleRate, rampLengthInSeconds);
    rampedFeedback.reset(sampleRate, rampLengthInSeconds);
    rampedReadHeadMix.reset(sampleRate, rampLengthInSeconds);
    
    rampedDelayTime.setValue(*_delayTime);
    rampedWetMix.setValue(*_wetMix);
    rampedFeedback.setValue(*_feedback);
    rampedReadHeadMix.setValue(*_readHeadMix);
    
    delayBufferLength = (int) (2.0 * sampleRate);
    if(delayBufferLength < 1)
        delayBufferLength = 1;
    delayBuffer.setSize(2, delayBufferLength);
    delayBuffer.clear();
    
    
    delayReadPosition = (int)(delayWritePosition - (*_delayTime * getSampleRate()) + delayBufferLength) % delayBufferLength;
    
}

void TapeDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void TapeDelayAudioProcessor::parameterChanged (const String &parameterID, float newValue)
{
    if (parameterID == Parameters::delayTime.toString()) {
         rampedDelayTime.setValue(newValue);
        *_delayTime = newValue;
    }
    else if (parameterID == Parameters::feedback.toString()) {
        rampedFeedback.setValue(newValue);
        *_feedback = newValue;
    }
    else if (parameterID == Parameters::wetMix.toString()) {
        rampedWetMix.setValue(newValue);
        *_wetMix = newValue;
    }
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TapeDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TapeDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    const auto numSamples = buffer.getNumSamples();
    
    int dpr, dpw;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        float* delayData = delayBuffer.getWritePointer (jmin (channel, delayBuffer.getNumChannels() - 1));
        
        dpw = delayWritePosition;

        for (int i = 0; i < numSamples; i++)
        {
            dpr = fmodf((float) dpw
                        - (float) (rampedDelayTime.getNextValue() * getSampleRate())
                        + (float) delayBufferLength,
                          (float) delayBufferLength);
            
            const float in = channelData[i];
            float out = 0.0;
            
            // Linear interpolation of delay line samples
            float fraction = dpr - floorf(dpr);
            int previousSample = (int) floorf(dpr);
            int nextSample = (previousSample + 1) % delayBufferLength;
            
            float interpolatedSample = fraction * delayData[nextSample] + (1.0f - fraction) * delayData[previousSample];
            
            float wet = rampedWetMix.getNextValue();
            out = (1 - wet) * in + wet * interpolatedSample;
            
            delayData[dpw] = in + (interpolatedSample * rampedFeedback.getNextValue());
            
            if (++dpr >= delayBufferLength)
                dpr = 0;
            if (++dpw >= delayBufferLength)
                dpw = 0;
            
            channelData[i] = out;
        }
    }
    delayReadPosition = dpr;
    delayWritePosition = dpw;
}

//==============================================================================
bool TapeDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TapeDelayAudioProcessor::createEditor()
{
    return new TapeDelayAudioProcessorEditor (*this);
}

//==============================================================================
void TapeDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TapeDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TapeDelayAudioProcessor();
}

AudioProcessorValueTreeState& TapeDelayAudioProcessor::getState()
{
    return state; // Reference to the APTVS to be accessed by processor children
}
