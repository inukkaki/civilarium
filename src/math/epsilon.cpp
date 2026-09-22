#include "math/epsilon.h"

namespace civilarium::math::epsilon {

bool EqualsZero(double x)
{
    return (-kZeroTolerance < x) && (x < kZeroTolerance);
}

}  // namespace civilarium::math::epsilon
