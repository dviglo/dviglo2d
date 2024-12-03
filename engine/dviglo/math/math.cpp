// Copyright (c) the Dviglo project
// Copyright (c) 2008-2023 the Urho3D project
// License: MIT

#include "math.hpp"

#include <cmath>


namespace dviglo
{

void sin_cos(f32 angle, f32& sin, f32& cos)
{
#ifdef _MSC_VER
    sin = sinf(angle);
    cos = cosf(angle);
#else // Linux или MinGW
    sincosf(angle, &sin, &cos);
#endif
}

} // namespace dviglo
