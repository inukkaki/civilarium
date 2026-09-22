#ifndef CIVILARIUM_MATH_EPSILON_H_
#define CIVILARIUM_MATH_EPSILON_H_

namespace civilarium::math::epsilon {

inline constexpr double kZeroTolerance = 1e-15;

bool EqualsZero(double x);

}  // namespace civilarium::math::epsilon

#endif  // CIVILARIUM_MATH_EPSILON_H_
