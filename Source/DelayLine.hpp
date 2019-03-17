//
//  DelayLine.hpp
//  TapeDelay
//
//  Created by Cyrus Vahidi on 16/03/2019.
//  Copyright © 2019 vyrus. All rights reserved.
//

#ifndef DelayLine_hpp
#define DelayLine_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

#endif /* DelayLine_hpp */
class DelayLine : public AudioProcessorValueTreeState::Listener
{
public:
    DelayLine(AudioProcessorValueTreeState&);
    ~DelayLine();
    
    void parameterChanged (const String &parameterID, float newValue) override;

    void prepareToPlay(double sampleRate, float delayTime, int numberOfReadPointers, float density);
    
    void write(float inputSample, int channel);
    float read(int readPointerIndex, int channel);
private:
    AudioProcessorValueTreeState& APVTS;
    
    double sampleRate;
    
    AudioBuffer<float> delayBuffer;
    int delayBufferLength;
    
    int dpw;
    std::vector<int> dprs;
    
    LinearSmoothedValue<float> rampedDelayTime;
    
    float prevDelayTime;
    float prevDensity;
    
    float speed;
};
