#ifndef LOCUS_H
#define LOCUS_H

#include "odmetry.hpp"

class Guide {
public:
	Guide(float curvature, float remainingDistance, float positionDeviation,
	      float angularDeviation);
	enum Availability {
		NOT_AVAILABLE,
	};
	Guide(Availability availability);
	bool isAvailable() const {
		return remainingDistance < 0 ? false : true;
	}
	float getCurvature() const;
	float getRemainingDistance() const;
	float getPositionDeviation() const;
	float getAngularDeviation() const;

private:
	float curvature;
	float remainingDistance;
	float positionDeviation;
	float angularDeviation;
};

class LocusElement {
public:
	LocusElement(const Vector terminalVector, float totalLength);
	virtual ~LocusElement(){};

	/**
	 * Get terminal vector of this locus element.
	 *
	 * @return terminal vector of this locus element
	 */
	Vector getTerminalVector() const { return terminalVector; }

	/**
	 * Get total length of this locus element.
	 *
	 * note: if you have once called {@link createGuide}, this method does
	 * not guarantee the length to be right.
	 *
	 * @return total lenght of this locus element until createGuide is
	 * called
	 */
	float getTotalLength() const { return totalLength; }

	/**
	 * Create Guide object from machine vector.
	 *
	 * note: this method implements sequential call from client, so you must
	 * not substitute excessive error of machine vector.
	 *
	 * @return {@link Guide} object calced from machine vector
	 */
	virtual Guide createGuide(const Vector& machineVector) = 0;

protected:
	Vector terminalVector;
	float totalLength;
};

class StrightLine : public LocusElement {
public:
	StrightLine(const Vector originVector, float length);
	Guide createGuide(const Vector& machineVector) override;

private:
	const float length;
};

class CurveLine : public LocusElement {
public:
	CurveLine(const Vector originVector, float signedRadius,
	          float signedAngle);
	Guide createGuide(const Vector& machineVector) override;

private:
	const float signedRadius;
	const Point centerPoint;
};

class TransitionCurve : public LocusElement {
public:
	TransitionCurve(const Vector originVector, float signedRadius,
	                float signedAngle);
	Guide createGuide(const Vector& machineVector) override;

private:
	const float signedRadius;
	Vector originVector;
	Vector firstTerminalVector;
	CurveLine curve;
};

class Locus : public LocusElement {
public:
	Locus(int size, Vector originVector);
	// for construction
	// will be called by main
	void addStrightLine(float length);
	void addCurveLine(float signedRadius, float signedAngle);
	void addTransitionCurve(float signedRadius, float signedAngle);

	Vector getTerminalVector();

	// for readPointer update
	// this must be called before {@link getElement}
	// this will be called by {@link ApplicationManager}
	// void update(const Vector& machineVector);

	// below method also may be called by {@link Application}
	// to obtain Guide objects
	// bool hasGuide() const;
	// Guide getGuide() const;

	Guide createGuide(const Vector& machineVector) override;

private:
	int writePointer;
	int readPointer;
	LocusElement** elements;
};
#endif
