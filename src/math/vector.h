#ifndef CIVILARIUM_MATH_VECTOR_H_
#define CIVILARIUM_MATH_VECTOR_H_

namespace civilarium::math::vector {

struct Vector2D {
    float x;
    float y;

    Vector2D() : x(0.0f), y(0.0f) {}
    Vector2D(float x, float y) : x(x), y(y) {}
    Vector2D(const Vector2D&) = default;

    void Zero();
    void Set(float x, float y);

    float Length() const;

    bool ParallelTo(const Vector2D& other) const;

    void FlushToZero();

    Vector2D& operator=(const Vector2D& rhs);

    Vector2D& operator+=(const Vector2D& rhs);
    Vector2D& operator-=(const Vector2D& rhs);
    Vector2D& operator*=(float rhs);
    Vector2D& operator/=(float rhs);
};

Vector2D operator+(const Vector2D& v);
Vector2D operator-(const Vector2D& v);

Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs);
Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs);
Vector2D operator*(float lhs, const Vector2D& rhs);
Vector2D operator*(const Vector2D& lhs, float rhs);
Vector2D operator/(const Vector2D& lhs, float rhs);

float Dot(const Vector2D& v);
float Dot(const Vector2D& lhs, const Vector2D& rhs);

float CrossZ(const Vector2D& lhs, const Vector2D& rhs);

}  // namespace civilarium::math::vector

#endif  // CIVILARIUM_MATH_VECTOR_H_
