#ifndef ScaleSpace_CONTROLS_HPP
#define ScaleSpace_CONTROLS_HPP

#include <array>

template <class T>
T limit (const T x, const T min, const T max)
{
    return (x < min ? min : (x > max ? max : x));
}

enum Parameters {
    kParameterX      = 0,
    kParameterY      = 1,
    kParameterCount  = 2
};

enum States {
    kStateFileSCL1 = 0,
    kStateFileSCL2 = 1,
    kStateFileSCL3 = 2,
    kStateFileSCL4 = 3,
    kStateFileKBM1 = 4,
    kStateFileKBM2 = 5,
    kStateFileKBM3 = 6,
    kStateFileKBM4 = 7,
    kStateCount    = 8
};

static const std::array<std::pair<float, float>, kParameterCount> controlLimits =
{{
    {-1.0f, 1.0f},   // kParameterX
	{-1.0f, 1.0f}   // kParameterY
}};

static const float ParameterDefaults[kParameterCount] = {
	0.0f, //kParameterX
	0.0f, //kParameterX,
};


#endif
