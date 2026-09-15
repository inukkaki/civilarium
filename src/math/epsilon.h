#ifndef CIVILARIUM_MATH_EPSILON_H_
#define CIVILARIUM_MATH_EPSILON_H_

namespace civilarium::math::epsilon {

inline constexpr float kZeroTolerance = 1e-6f;

bool EqualsZero(float x);

}  // namespace civilarium::math::epsilon

#endif  // CIVILARIUM_MATH_EPSILON_H_
