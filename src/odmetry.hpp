#ifndef ODMETRY_H
#define ODMETRY_H

#include "encoder.hpp"

class Vector;

class Point {
    public:
        Point(float, float);
        float getX() const ;
        float getY() const ;
        float calcDistanceFrom(const Point&) const;
        float calcSignedDistanceFrom(const Vector& vector) const;
        float calcSignedDistanceTo(const Vector& vector) const;
    
    private:
        float x;
        float y;
};

class Vector {
    public:
        Vector(const Point startingPoint, float theta);
        Vector(const Point startingPoint, const Point endingPoint);
        Point getStartingPoint() const;
        float getX() const;
        float getY() const;
        float getTheta() const;
        float calcSignedDistanceFrom(const Point& point) const;
        float calcSignedDistanceTo(const Point& point) const;
        float calcSignedAngularFrom(const Vector& vector) const;
        float calcSignedAngularTo(const Vector& vector) const;

    private:
        float x;
        float y;
        float theta;
};

class Odmetry {
    public:
        Odmetry(const Encoder& leftEncoder, const Encoder& rightEncoder);
        void update();
        void modifyDifference(const Vector&);
        Vector getMachineVector() const;
    private:
        const Encoder& leftEncoder;
        const Encoder& rightEncoder;
        float x;
        float y;
        float theta;
};

#endif
