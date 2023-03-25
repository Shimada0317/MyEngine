#include "GenericProcess.h"
#include<cassert>
#include<Windows.h>

GenericProcess* GenericProcess::GetInstance()
{
    static GenericProcess instance;

    return &instance;
}

int GenericProcess::Clamp(int value, int low, int high)
{
    assert(low <= high);
    return min(max(value, low), high);
}

float GenericProcess::Clampf(float value, float low, float high)
{
    assert(low <= high);
    return min(max(value, low), high);
}

int GenericProcess::Wrap(int value, int low, int high)
{
    assert(low < high);
    const int n = (value - low) % (high - low);
    return (n >= 0) ? (n + low) : (n + high);
}
