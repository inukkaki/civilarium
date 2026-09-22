#ifndef CIVILARIUM_MATH_VECTOR_H_
#define CIVILARIUM_MATH_VECTOR_H_

namespace civilarium::math::vector {

struct Vector2D {
    double x;
    double y;

    Vector2D() : x(0.0), y(0.0) {}
    Vector2D(double x, double y) : x(x), y(y) {}
    Vector2D(const Vector2D&) = default;

    void Zero();
    void Set(double x, double y);

    double Length() const;

    bool ParallelTo(const Vector2D& other) const;

    void FlushToZero();

    Vector2D& operator=(const Vector2D& rhs);

    Vector2D& operator+=(const Vector2D& rhs);
    Vector2D& operator-=(const Vector2D& rhs);
    Vector2D& operator*=(double rhs);
    Vector2D& operator/=(double rhs);
};

Vector2D operator+(const Vector2D& v);
Vector2D operator-(const Vector2D& v);

Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs);
Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs);
Vector2D operator*(double lhs, const Vector2D& rhs);
Vector2D operator*(const Vector2D& lhs, double rhs);
Vector2D operator/(const Vector2D& lhs, double rhs);

double Dot(const Vector2D& v);
double Dot(const Vector2D& lhs, const Vector2D& rhs);

double CrossZ(const Vector2D& lhs, const Vector2D& rhs);

}  // namespace civilarium::math::vector

#endif  // CIVILARIUM_MATH_VECTOR_H_
