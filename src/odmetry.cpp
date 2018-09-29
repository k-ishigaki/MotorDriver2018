#include "odmetry.hpp"

#include "config.hpp"
#include <math.h>

// Point
Point::Point(float x, float y) : x(x), y(y) {}

float Point::getX() const { return x; }

float Point::getY() const { return y; }

float Point::calcDistanceFrom(const Point& point) const {
    return hypot(x - point.getX(), y - point.getY());
}

// Vector
Vector::Vector(const Point startingPoint, float theta)
    : x(startingPoint.getX()), y(startingPoint.getY()), theta(theta) {}

Vector::Vector(const Point startingPoint, const Point endingPoint)
    : x(startingPoint.getX()), y(startingPoint.getY()),
    theta(atan2(endingPoint.getY() - startingPoint.getY(),
                endingPoint.getX() - startingPoint.getX())) {}

    float Point::calcSignedDistanceFrom(const Vector& vector) const {
        return vector.calcSignedDistanceTo(*this);
    }

float Point::calcSignedDistanceTo(const Vector& vector) const {
    return vector.calcSignedDistanceFrom(*this);
}

Point Vector::getStartingPoint() const { return Point(x, y); }

float Vector::getX() const { return x; }

float Vector::getY() const { return y; }

float Vector::getTheta() const { return theta; }

float Vector::calcSignedDistanceFrom(const Point& point) const {
    return (point.getX() - x) * sin(theta) -
        (point.getY() - y) * cos(theta);
}

float Vector::calcSignedDistanceTo(const Point& point) const {
    return (point.getY() - y) * cos(theta) -
        (point.getX() - x) * sin(theta);
}

float Vector::calcSignedAngularFrom(const Vector& vector) const {
    return atan2(sin(theta - vector.getTheta()),
            cos(theta - vector.getTheta()));
}

float Vector::calcSignedAngularTo(const Vector& vector) const {
    return -calcSignedAngularFrom(vector);
}

// Odmetry
Odmetry::Odmetry(const Encoder& leftEncoder, const Encoder& rightEncoder) : leftEncoder(leftEncoder), rightEncoder(rightEncoder) {
    this->x = 0;
    this->y = 0;
    this->theta = 0;
};

void Odmetry::update() {
    auto leftDelta = this->leftEncoder.getIntegratedLength();
    auto rightDelta = this->rightEncoder.getIntegratedLength();
    // 角速度
    float deltaTheta = (rightDelta - leftDelta) / config::WheelTread;

    // 水平、垂直軸速度
    // 中間速度は先に計算する
    // 角度は変化の中間値を入力する
    float centerSpeed = (leftDelta + rightDelta) / 2;
    float rad = theta + deltaTheta / 2;
    // 近似 正確には
    // deltaX =
    // centerSpeed * cos(rad) * sin(deltaTheta / 2) / (deltaTheta / 2);
    float deltaX = centerSpeed * cos(rad) * (1 + square(deltaTheta) / 12);
    float deltaY = centerSpeed * sin(rad) * (1 + square(deltaTheta) / 12);

    // 値の更新
    this->theta += deltaTheta;
    this->x += deltaX;
    this->y += deltaY;
}

void Odmetry::modifyDifference(const Vector& vector) {
    theta += vector.getTheta();
    float x_diff = vector.getX();
    float y_diff = vector.getY();
    this->x += x_diff * cos(this->theta) - y_diff * sin(this->theta);
    this->y += x_diff * sin(this->theta) + y_diff * cos(this->theta);
}

Vector Odmetry::getMachineVector() const {
    // returns advance reading vector
    // float advanceReadLength = DELTA_T *
    // indicator.getCenterSpeed();
    return Vector(Point(this->x, this->y), this->theta);
}

