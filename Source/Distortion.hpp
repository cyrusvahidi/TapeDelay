//
//  Distortion.hpp
//  TapeDelay
//
//  Created by Cyrus Vahidi on 16/03/2019.
//  Copyright © 2019 vyrus. All rights reserved.
//

#ifndef Distortion_hpp
#define Distortion_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
class Distortion
{
public:
    Distortion();
    ~Distortion();
    
    float getNextSample();
private:
};

#endif /* Distortion_hpp */
