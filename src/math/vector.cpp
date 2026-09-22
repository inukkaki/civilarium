#include "math/vector.h"

#include <cmath>

#include "math/epsilon.h"

namespace civilarium::math::vector {

namespace impl {

namespace epsilon = civilarium::math::epsilon;

}  // namespace impl

void Vector2D::Zero()
{
    x = 0.0;
    y = 0.0;
}

void Vector2D::Set(double x, double y)
{
    this->x = x;
    this->y = y;
}

double Vector2D::Length() const
{
    return std::sqrt(x*x + y*y);
}

bool Vector2D::ParallelTo(const Vector2D& other) const
{
    return impl::epsilon::EqualsZero(CrossZ(*this, other));
}

void Vector2D::FlushToZero()
{
    if (impl::epsilon::EqualsZero(x)) { x = 0.0; }
    if (impl::epsilon::EqualsZero(y)) { y = 0.0; }
}

Vector2D& Vector2D::operator=(const Vector2D& rhs)
{
    x = rhs.x;
    y = rhs.y;
    return *this;
}

Vector2D& Vector2D::operator+=(const Vector2D& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vector2D& Vector2D::operator*=(double rhs)
{
    x *= rhs;
    y *= rhs;
    return *this;
}

Vector2D& Vector2D::operator/=(double rhs)
{
    x /= rhs;
    y /= rhs;
    return *this;
}

Vector2D operator+(const Vector2D& v)
{
    Vector2D tmp(+v.x, +v.y);
    return tmp;
}

Vector2D operator-(const Vector2D& v)
{
    Vector2D tmp(-v.x, -v.y);
    return tmp;
}

Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs)
{
    Vector2D tmp(lhs.x + rhs.x, lhs.y + rhs.y);
    return tmp;
}

Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs)
{
    Vector2D tmp(lhs.x - rhs.x, lhs.y - rhs.y);
    return tmp;
}

Vector2D operator*(double lhs, const Vector2D& rhs)
{
    Vector2D tmp(lhs*rhs.x, lhs*rhs.y);
    return tmp;
}

Vector2D operator*(const Vector2D& lhs, double rhs)
{
    Vector2D tmp(lhs.x*rhs, lhs.y*rhs);
    return tmp;
}

Vector2D operator/(const Vector2D& lhs, double rhs)
{
    Vector2D tmp(lhs.x/rhs, lhs.y/rhs);
    return tmp;
}

double Dot(const Vector2D& v)
{
    return v.x*v.x + v.y*v.y;
}

double Dot(const Vector2D& lhs, const Vector2D& rhs)
{
    return lhs.x*rhs.x + lhs.y*rhs.y;
}

double CrossZ(const Vector2D& lhs, const Vector2D& rhs)
{
    return lhs.x*rhs.y - lhs.y*rhs.x;
}

}  // namespace civilarium::math::vector
