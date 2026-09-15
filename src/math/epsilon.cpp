#include "math/epsilon.h"

namespace civilarium::math::epsilon {

bool EqualsZero(float x)
{
    return (-kZeroTolerance < x) && (x < kZeroTolerance);
}

}  // namespace civilarium::math::epsilon
