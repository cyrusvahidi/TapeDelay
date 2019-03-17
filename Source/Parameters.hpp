//
//  Parameters.hpp
//  TapeDelay
//
//  Created by Cyrus Vahidi on 17/03/2019.
//  Copyright © 2019 vyrus. All rights reserved.
//

#ifndef Parameters_hpp
#define Parameters_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include <stdio.h>
#include <map>

namespace Parameters
{
    static const Identifier delayTime   { "delayTime"   };
    static const Identifier wetMix      { "wetMix"      };
    static const Identifier feedback    { "feedback"    };
    static const Identifier readHeadMix { "readHeadMix" };
    static const Identifier density     { "density"     };
    static const Identifier lfoRate     { "lfoRate"     };
    static const Identifier lfoAmount   { "lfoAmount"   };
    static const Identifier tapeSpeed   { "tapeSpeed"   };
    // TODO: add distortion control
    // TODO: init other params in processor
    
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
        { delayTime,   { "Delay Time", 0.2f, 0.f, 2.0f, 0.01f   } },
        { wetMix,      { "Dry/Wet",    0.5f, 0.f, 1.0f, 0.01f   } },
        { feedback,    { "Feedback",   0.5f, 0.f, 0.995f, 0.01f } },
        { readHeadMix, { "Head Mix",   0.5f, 0.f, 1.0f, 0.01f   } },
        { density,     { "Density",    0.5f, 0.f, 1.0f, 0.01f   } },
        { lfoRate,     { "lfoRate",    10.f, 1.f, 20.f, 0.01f   } },
        { lfoAmount,   { "lfoAmount",  0.f, 0.f, 1.f, 0.01f     } },
        { tapeSpeed,   { "tapeSpeed",  0.5f, 0.f, 1.f, 0.01f    } }
    };
}

#endif /* Parameters_hpp */
