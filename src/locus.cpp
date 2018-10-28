#include "locus.hpp"

#include <math.h>

// Guide
Guide::Guide(float curvature, float remainingDistance, float positionDeviation,
             float angularDeviation)
    : curvature(curvature), remainingDistance(remainingDistance),
      positionDeviation(positionDeviation), angularDeviation(angularDeviation) {
}
Guide::Guide(Availability availability)
    : curvature(0), remainingDistance(-1), positionDeviation(0),
      angularDeviation(0) {}

float Guide::getCurvature() const { return curvature; }

float Guide::getRemainingDistance() const { return remainingDistance; }

float Guide::getPositionDeviation() const { return positionDeviation; }

float Guide::getAngularDeviation() const { return angularDeviation; }

// LocusElement
LocusElement::LocusElement(const Vector terminalVector, float totalLength)
    : terminalVector(terminalVector), totalLength(totalLength) {}

// StrightLine
StrightLine::StrightLine(const Vector originVector, float length)
    : LocusElement(Vector(Point(originVector.getX() +
                                    length * cos(originVector.getTheta()),
                                originVector.getY() +
                                    length * sin(originVector.getTheta())),
                          originVector.getTheta()),
                   length),
      length(length) {}

Guide StrightLine::createGuide(const Vector& machineVector) {
	// calc remaining distance
	Vector terminalVerticalVector(terminalVector.getStartingPoint(),
	                              terminalVector.getTheta() + M_PI / 2);
	float remainingDistance = terminalVerticalVector.calcSignedDistanceTo(
	    machineVector.getStartingPoint());
	// calc distance deviation
	float positionDeviation = terminalVector.calcSignedDistanceTo(
	    machineVector.getStartingPoint());
	// calc angular deviation
	float angularDeviation =
	    machineVector.calcSignedAngularFrom(terminalVector);
	return Guide(0.0, remainingDistance, positionDeviation,
	             angularDeviation);
}

// CurveLine
CurveLine::CurveLine(const Vector originVector, float signedRadius,
        float signedAngle)
    : LocusElement(
            Vector(Point(originVector.getX() +
                    signedRadius *
                    (sin(originVector.getTheta() + signedAngle) -
                     sin(originVector.getTheta())),
                    originVector.getY() +
                    signedRadius *
                    (cos(originVector.getTheta()) -
                     cos(originVector.getTheta() + signedAngle))),
                originVector.getTheta() + signedAngle),
            signedRadius * signedAngle),
    signedRadius(signedRadius),
    centerPoint(Point(
                originVector.getX() - signedRadius * sin(originVector.getTheta()),
                originVector.getY() + signedRadius * cos(originVector.getTheta()))) {
    }

Guide CurveLine::createGuide(const Vector& machineVector) {
    // calc remaining distance
    Vector machineCentrifugalVector(centerPoint,
            machineVector.getStartingPoint());
	Vector terminalCentrifugalVector(centerPoint,
	                                 terminalVector.getStartingPoint());
	float remainingAngle = machineCentrifugalVector.calcSignedAngularTo(
	    terminalCentrifugalVector);
	float remainingDistance = remainingAngle * signedRadius;
	// calc distance deviation
	float machineRadius =
	    machineVector.getStartingPoint().calcDistanceFrom(centerPoint);
	float positionDeviation = signedRadius >= 0
	                              ? signedRadius - machineRadius
	                              : machineRadius + signedRadius;
	// calc angular deviation
	float targetAngle;
	if (signedRadius >= 0) {
		targetAngle = machineCentrifugalVector.getTheta() + M_PI / 2;
	} else {
		targetAngle = machineCentrifugalVector.getTheta() - M_PI / 2;
	}
	Vector target = Vector(Point(0, 0), targetAngle);
	float angularDeviation = machineVector.calcSignedAngularFrom(target);
	return Guide(1.0 / signedRadius, remainingDistance, positionDeviation,
	             angularDeviation);
}

// TransitionCurve
TransitionCurve::TransitionCurve(const Vector originVector, float signedRadius,
                                 float signedAngle)
    : LocusElement(
          Vector(Point(originVector.getX() +
                           signedRadius *
                               (sin(originVector.getTheta() + signedAngle) -
                                sin(originVector.getTheta())),
                       originVector.getY() +
                           signedRadius *
                               (cos(originVector.getTheta()) -
                                cos(originVector.getTheta() + signedAngle))),
                 originVector.getTheta() + signedAngle),
          0),
      signedRadius(signedRadius), originVector(originVector),
      firstTerminalVector(Vector(
          Point(
              originVector.getX() +
                  2.236067977 / (6 / tan(fabs(signedAngle) / 2) + 22.36067977) *
                      fabs(signedRadius) *
                      (6.708203932 * cos(originVector.getTheta()) -
                       (signedRadius > 0 ? 1 : -1) *
                           sin(originVector.getTheta())),
              originVector.getY() +
                  2.236067977 / (6 / tan(fabs(signedAngle) / 2) + 22.36067977) *
                      fabs(signedRadius) *
                      (6.708203932 * sin(originVector.getTheta()) +
                       (signedRadius > 0 ? 1 : -1) *
                           cos(originVector.getTheta()))),
          originVector.getTheta() +
              (signedAngle > 0 ? 0.42053433 : -0.42053433))),
      curve(firstTerminalVector,
            22.04540768 * tan(fabs(signedAngle) / 2) /
                (6 + 22.36067977 * tan(fabs(signedAngle) / 2)) * signedRadius,
            signedAngle - 2 * (signedAngle > 0 ? 0.42053433 : -0.42053433)) {
	// curveを利用したほうが総距離の計算が楽なので、実装部に書いている
	totalLength =
	    2 * fabs(signedRadius) * 0.538431021 + curve.getTotalLength();
}

Guide TransitionCurve::createGuide(const Vector& machineVector) {
	// 第一区間
	Vector verticalFirstTerminalVector(
	    firstTerminalVector.getStartingPoint(),
	    firstTerminalVector.getTheta() + M_PI / 2);
	    if (verticalFirstTerminalVector.calcSignedDistanceTo(
	            machineVector.getStartingPoint()) > 0) {
		    Vector verticalOriginVector(originVector.getStartingPoint(),
		                                originVector.getTheta() +
		                                    M_PI / 2);
		    float r = fabs(signedRadius);
		    float x = verticalOriginVector.calcSignedDistanceFrom(
		        machineVector.getStartingPoint());
		    float x_r = x / r;
		    float y = signedRadius * 0.5328985 * pow(x_r, 3);
		    float theta = (1.4564 * x_r + 0.0362) * x / signedRadius;

		    float remainingDistance =
		        2 * r * 0.538431021 - x +
		        curve.createGuide(firstTerminalVector)
		            .getRemainingDistance();
		    float curvature =
		        (-6.2629 * pow(x_r, 2) + 2.2799 * x_r + 2.9885) * x_r /
		        signedRadius;
		    float theta_o = originVector.getTheta();
		    Vector targetVector(
		        Point(originVector.getX() + x * cos(theta_o) -
		                  y * sin(theta_o),
		              originVector.getY() + x * sin(theta_o) +
		                  y * cos(theta_o)),
		        theta_o + theta);

		    float positionDeviation = targetVector.calcSignedDistanceTo(
		        machineVector.getStartingPoint());
		    float angularDeviation =
		        targetVector.calcSignedAngularTo(machineVector);

		    return Guide(curvature, remainingDistance,
		                 positionDeviation, angularDeviation);
	    }
	    // 第二区間
	    Guide curveGuide = curve.createGuide(machineVector);
	    if (curveGuide.isAvailable() == true) {
		    return Guide(curveGuide.getCurvature(),
		                 curveGuide.getRemainingDistance() +
		                     fabs(signedRadius) * 0.538431021,
		                 curveGuide.getPositionDeviation(),
		                 curveGuide.getAngularDeviation());
	    }
	    // 第三区間
	    Vector terminalVerticalVector(terminalVector.getStartingPoint(),
	                                  terminalVector.getTheta() + M_PI / 2);
	    if (terminalVerticalVector.calcSignedDistanceTo(
	            machineVector.getStartingPoint()) > 0) {
		    float r = fabs(signedRadius);
		    float x = terminalVerticalVector.calcSignedDistanceTo(
		        machineVector.getStartingPoint());
		    x = x < 0 ? 0 : x;
		    float x_r = x / r;
		    float y = signedRadius * 0.5328985 * pow(x_r, 3);
		    float theta = (1.4564 * x_r + 0.0362) * x / signedRadius;

		    float remainingDistance = x;
		    float curvature =
		        (-6.2629 * pow(x_r, 2) + 2.2799 * x_r + 2.9885) * x_r /
		        signedRadius;
		    float theta_t = terminalVector.getTheta();
		    float cos_theta_t = cos(theta_t);
		    float sin_theta_t = sin(theta_t);
		    Vector targetVector(
		        Point(terminalVector.getX() - x * cos_theta_t -
		                  y * sin_theta_t,
		              terminalVector.getY() - x * sin_theta_t +
		                  y * cos_theta_t),
		        theta_t - theta);

		    float positionDeviation = targetVector.calcSignedDistanceTo(
		        machineVector.getStartingPoint());
		    float angularDeviation =
		        targetVector.calcSignedAngularTo(machineVector);

		    return Guide(curvature, remainingDistance,
		                 positionDeviation, angularDeviation);
	    }
	    return Guide(Guide::NOT_AVAILABLE);
}

// Locus
Locus::Locus(int size, Vector originVector) : LocusElement(originVector, 0) {
	elements = new LocusElement*[size];
	writePointer = 0;
	readPointer = 0;
}

void Locus::addStrightLine(float length) {
	if (writePointer == 0) {
		elements[writePointer] =
		    new StrightLine(terminalVector, length);
	} else {
		elements[writePointer] = new StrightLine(
		    elements[writePointer - 1]->getTerminalVector(), length);
	}
	totalLength += elements[writePointer]->getTotalLength();
	writePointer++;
}

void Locus::addCurveLine(float signedRadius, float signedAngle) {
	if (writePointer == 0) {
		elements[writePointer] =
		    new CurveLine(terminalVector, signedRadius, signedAngle);
	} else {
		elements[writePointer] = new CurveLine(
		    elements[writePointer - 1]->getTerminalVector(),
		    signedRadius, signedAngle);
	}
	totalLength += elements[writePointer]->getTotalLength();
	writePointer++;
}

void Locus::addTransitionCurve(float signedRadius, float signedAngle) {
	if (writePointer == 0) {
		elements[writePointer] = new TransitionCurve(
		    terminalVector, signedRadius, signedAngle);
	} else {
		elements[writePointer] = new TransitionCurve(
		    elements[writePointer - 1]->getTerminalVector(),
		    signedRadius, signedAngle);
	}
	totalLength += elements[writePointer]->getTotalLength();
	writePointer++;
}

Vector Locus::getTerminalVector() {
	return elements[writePointer - 1]->getTerminalVector();
}

Guide Locus::createGuide(const Vector& machineVector) {
	if (readPointer >= writePointer) {
		// out of bounds
		return Guide(Guide::NOT_AVAILABLE);
	}
	Guide currentGuide = elements[readPointer]->createGuide(machineVector);
	while (currentGuide.isAvailable() == false) {
		readPointer++;
		if (readPointer >= writePointer) {
			// out of bounds
			return Guide(Guide::NOT_AVAILABLE);
		}
		// FIXME terminalVectorが使われていない
		terminalVector = elements[readPointer]->getTerminalVector();
		currentGuide =
		    elements[readPointer]->createGuide(machineVector);
	}
	float futureTotalLength = 0;
	for (int i = readPointer + 1; i < writePointer; i++) {
		futureTotalLength += elements[i]->getTotalLength();
	}
	return Guide(currentGuide.getCurvature(),
	             futureTotalLength + currentGuide.getRemainingDistance(),
	             currentGuide.getPositionDeviation(),
	             currentGuide.getAngularDeviation());
}
