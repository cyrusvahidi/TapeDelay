//
//  DelayLine.cpp
//  TapeDelay
//
//  Created by Cyrus Vahidi on 16/03/2019.
//  Copyright © 2019 vyrus. All rights reserved.
//

#include "DelayLine.hpp"
#include "Parameters.hpp"

DelayLine::DelayLine(AudioProcessorValueTreeState& state) :
    APVTS(state)
{
    state.addParameterListener (Parameters::delayTime, this);

}

void DelayLine::parameterChanged (const String &parameterID, float newValue)
{
    if (parameterID == Parameters::delayTime.toString()) {
        rampedDelayTime.setValue(newValue);
    }
}

/* Given the initial delay time, the number of read pointers (heads) and a density factor
 the tape will be initialised with a main head offset from dpw according to the delay time.
 The secondary read pointer positions will be set according to the density.
*/
void DelayLine::prepareToPlay(double sampleRate, float delayTime, int numReadPointers, float density)
{
    delayBuffer.setSize(2, 2.0 * sampleRate);
    delayBufferLength = (int) (2.0 * sampleRate);
    delayBuffer.clear();
    
    dpw = 0;
    dprs[0] = (int)(dpw - (delayTime * sampleRate) + delayBufferLength) % delayBufferLength;
    
    this->sampleRate = sampleRate;
    // set read ptrs
    // set write pts
    // set spacing according to density
}

void DelayLine::write(float inputSample, int channel)
{
    delayBuffer.setSample(channel, dpw, inputSample);
    
    if (++dpw > delayBufferLength) dpw = 0;
}

float DelayLine::read(int readPointerIndex, int channel)
{
    float dpr = fmodf((float) dpw
                - (float) (rampedDelayTime.getNextValue() * sampleRate)
                + (float) delayBufferLength,
                (float) delayBufferLength);
    
    // Linear interpolation of delay line samples
    float fraction = dpr - floorf(dpr);
    int previousSample = (int) floorf(dpr);
    int nextSample = (previousSample + 1) % delayBufferLength;
    
    float interpolatedSample = fraction * delayBuffer.getSample(channel, nextSample) + (1.0f - fraction) * delayBuffer.getSample(channel, previousSample);
    
    return interpolatedSample;
}
