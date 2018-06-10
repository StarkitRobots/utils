#pragma once

namespace rhoban_utils
{

class Control
{
    public:
        Control();
        void reset();
        float update(float error, float dt = 1.0);

        // Minimum and maximum for the output
        float min, max;
        // Gain of the proportional 
        float k_p;
        // Gain for the integration
        float k_i;
        // Accumulator
        float acc;
        // Output of the controller
        float output;
};

}
