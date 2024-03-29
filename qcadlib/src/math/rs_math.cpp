/****************************************************************************
** $Id: rs_math.cpp 1938 2004-12-09 23:09:53Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "rs_math.h"

#include "rs_debug.h"


/**
 * Rounds the given double to the next int. 
 */
int RS_Math::round(double v) {
    return (v-floor(v)<0.5 ? (int)floor(v) : (int)ceil(v));
}




/**
 * Save pow function
 */
double RS_Math::pow(double x, double y) {
	errno = 0;
	double ret = ::pow(x, y);
	if (errno==EDOM) {
        RS_DEBUG->print(RS_Debug::D_ERROR,
			"RS_Math::pow: EDOM in pow");
		ret = 0.0;
	}
	else if (errno==ERANGE) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
			"RS_Math::pow: ERANGE in pow");
		ret = 0.0;
	}
	return ret;
}



/**
 * Converts radians to degrees.
 */
double RS_Math::rad2deg(double a) {
    return (a/(2.0*M_PI)*360.0);
}



/**
 * Converts degrees to radians.
 */
double RS_Math::deg2rad(double a) {
    return ((a/360.0)*(2.0*M_PI));
}



/**
 * Converts radians to gradians.
 */
double RS_Math::rad2gra(double a) {
    return (a/(2.0*M_PI)*400.0);
}



/**
 * Finds greatest common divider using Euclid's algorithm.
 */
int RS_Math::findGCD(int a, int b) {
    int rem;

    while (b!=0) {
        rem = a % b;
        a = b;
        b = rem;
    }

    return a;
}



/**
 * Tests if angle a is between a1 and a2. a, a1 and a2 must be in the 
 * range between 0 and 2*PI.
 * All angles in rad.
 *
 * @param reversed true for clockwise testing. false for ccw testing.
 * @return true if the angle a is between a1 and a2.
 */
bool RS_Math::isAngleBetween(double a,
                             double a1, double a2,
                             bool reversed) {

    bool ret = false;

    if (reversed) {
        double tmp = a1;
        a1 = a2;
        a2 = tmp;
    }

    if(a1>=a2-1.0e-12) {
        if(a>=a1-1.0e-12 || a<=a2+1.0e-12) {
            ret = true;
        }
    } else {
        if(a>=a1-1.0e-12 && a<=a2+1.0e-12) {
            ret = true;
        }
    }
    //RS_DEBUG->print("angle %f is %sbetween %f and %f",
    //                a, ret ? "" : "not ", a1, a2);
    return ret;
}



/**
 * Corrects the given angle to the range of 0-2*Pi.
 */
double RS_Math::correctAngle(double a) {
    while (a>2*M_PI)
        a-=2*M_PI;
    while (a<0)
        a+=2*M_PI;
    return a;
}



/**
 * @return The angle that needs to be added to a1 to reach a2.
 *         Always positive and less than 2*pi.
 */
double RS_Math::getAngleDifference(double a1, double a2) {
    double ret;

    if (a1>=a2) {
        a2+=2*M_PI;
    }
    ret = a2-a1;

    if (ret>=2*M_PI) {
        ret=0.0;
    }

    return ret;
}


/**
* Makes a text constructed with the given angle readable. Used
* for dimension texts and for mirroring texts.
*
* @param readable true: make angle readable, false: unreadable
* @param corrected Will point to true if the given angle was
*   corrected, false otherwise.
* 
 * @return The given angle or the given angle+PI, depending which on
 * is readable from the bottom or right.
 */
double RS_Math::makeAngleReadable(double angle, bool readable,
                                  bool* corrected) {

    double ret;

    bool cor = isAngleReadable(angle) ^ readable;

    // quadrant 1 & 4
    if (!cor) {
        ret = angle;
    }
    // quadrant 2 & 3
    else {
        ret = angle+M_PI;
    }

    if (corrected!=NULL) {
        *corrected = cor;
    }

    return ret;
}


/**
 * @return true: if the given angle is in a range that is readable 
 * for texts created with that angle.
 */
bool RS_Math::isAngleReadable(double angle) {
    if (angle>M_PI/2.0*3.0+0.001 ||
            angle<M_PI/2.0+0.001) {
        return true;
    } else {
        return false;
    }
}



/**
 * @param tol Tolerance in rad.
 * @retval true The two angles point in the same direction.
 */
bool RS_Math::isSameDirection(double dir1, double dir2, double tol) {
	double diff = fabs(dir1-dir2);
	if (diff<tol || diff>2*M_PI-tol) {
		//std::cout << "RS_Math::isSameDirection: " << dir1 << " and " << dir2 
		//	<< " point in the same direction" << "\n";
		return true;
	}
	else {
		//std::cout << "RS_Math::isSameDirection: " << dir1 << " and " << dir2 
		//	<< " don't point in the same direction" << "\n";
		return false;
	}
}


/**
 * Compares two double values with a tolerance.
 */
bool RS_Math::cmpDouble(double v1, double v2, double tol) {
    return (fabs(v2-v1)<tol);
}



/**
 * Evaluates a mathematical expression and returns the result.
 * If an error occured, the given default value 'def' will be returned.
 */
double RS_Math::eval(const RS_String& expr, double def) {

    bool ok;
    double res = RS_Math::eval(expr, &ok);

    if (!ok) {
        //std::cerr << "RS_Math::evaluate: Parse error at col "
        //<< ret << ": " << fp.ErrorMsg() << "\n";
        return def;
    }

    return res;
}


/**
 * Evaluates a mathematical expression and returns the result.
 * If an error occured, ok will be set to false (if ok isn't NULL).
 */
//double RS_Math::eval(const RS_String& expr, bool* ok);


/**
 * Converts a double into a string which is as short as possible
 *
 * @param value The double value
 * @param prec Precision e.g. a precision of 1 would mean that a 
 *     value of 2.12030 will be converted to "2.1". 2.000 is always just "2").
 */
RS_String RS_Math::doubleToString(double value, double prec) {
	if (prec<1.0e-12) {
		std::cerr << "RS_Math::doubleToString: invalid precision\n";
		return "";
	}

    RS_String ret;
    RS_String exaStr;
    int dotPos;
    int num = RS_Math::round(value / prec);

    exaStr = RS_Math::doubleToString(prec, 10);
    dotPos = exaStr.find('.');

    if (dotPos==-1) {
        ret.sprintf("%d", RS_Math::round(num*prec));
    } else {
        int digits = exaStr.length() - dotPos - 1;
        ret = RS_Math::doubleToString(num*prec, digits);
    }

    return ret;
}




/**
 * Converts a double into a string which is as short as possible.
 *
 * @param value The double value
 * @param prec Precision
 */
RS_String RS_Math::doubleToString(double value, int prec) {
    RS_String valStr;

    valStr.setNum(value, 'f', prec);

    if(valStr.contains('.')) {
        // Remove zeros at the end:
        while (valStr.at(valStr.length()-1)=='0') {
            valStr.truncate(valStr.length()-1);
        }

        if(valStr.at(valStr.length()-1)=='.') {
            valStr.truncate(valStr.length()-1);
        }
    }

    return valStr;
}



/**
 * Performs some testing for the math class.
 */
void RS_Math::test() {
    RS_String s;
    double v;

    std::cout << "RS_Math::test: doubleToString:\n";

    v = 0.1;
    s = RS_Math::doubleToString(v, 0.1);
    assert(s=="0.1");
    s = RS_Math::doubleToString(v, 0.01);
    assert(s=="0.1");
    s = RS_Math::doubleToString(v, 0.0);
    assert(s=="0");

    v = 0.01;
    s = RS_Math::doubleToString(v, 0.1);
    assert(s=="0");
    s = RS_Math::doubleToString(v, 0.01);
    assert(s=="0.01");
    s = RS_Math::doubleToString(v, 0.0);
    assert(s=="0");

    v = 0.001;
    s = RS_Math::doubleToString(v, 0.1);
    assert(s=="0");
    s = RS_Math::doubleToString(v, 0.01);
    assert(s=="0");
    s = RS_Math::doubleToString(v, 0.001);
    assert(s=="0.001");
    s = RS_Math::doubleToString(v, 0.0);
    assert(s=="0");

    std::cout << "RS_Math::test: complete\n";
}

