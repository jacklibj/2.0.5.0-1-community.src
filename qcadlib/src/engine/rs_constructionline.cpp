/****************************************************************************
** $Id: rs_constructionline.cpp 1938 2004-12-09 23:09:53Z andrew $
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


#include "rs_constructionline.h"

#include "rs_debug.h"



/**
 * Constructor.
 */
RS_ConstructionLine::RS_ConstructionLine(RS_EntityContainer* parent,
        const RS_ConstructionLineData& d)
        :RS_AtomicEntity(parent), data(d) {

    calculateBorders();
}



/**
 * Destructor.
 */
RS_ConstructionLine::~RS_ConstructionLine() {}



RS_Entity* RS_ConstructionLine::clone() {
    RS_ConstructionLine* c = new RS_ConstructionLine(*this);
    c->initId();
    return c;
}



void RS_ConstructionLine::calculateBorders() {
    minV = RS_Vector::minimum(data.point1, data.point2);
    maxV = RS_Vector::maximum(data.point1, data.point2);
}



RS_Vector RS_ConstructionLine::getNearestEndpoint(const RS_Vector& coord,
        double* dist) {
    double dist1, dist2;
    RS_Vector* nearerPoint;

    dist1 = data.point1.distanceTo(coord);
    dist2 = data.point2.distanceTo(coord);

    if (dist2<dist1) {
        if (dist!=NULL) {
            *dist = dist2;
        }
        nearerPoint = &data.point2;
    } else {
        if (dist!=NULL) {
            *dist = dist1;
        }
        nearerPoint = &data.point1;
    }

    return *nearerPoint;
}



RS_Vector RS_ConstructionLine::getNearestPointOnEntity(const RS_Vector& coord,
        bool /*onEntity*/, double* /*dist*/, RS_Entity** entity) {

    if (entity!=NULL) {
        *entity = this;
    }

    RS_Vector ae = data.point2-data.point1;
    RS_Vector ea = data.point1-data.point2;
    RS_Vector ap = coord-data.point1;
    RS_Vector ep = coord-data.point2;
	
	if (ae.magnitude()<1.0e-6 || ea.magnitude()<1.0e-6) {
		return RS_Vector(false);
	}

    // Orthogonal projection from both sides:
    RS_Vector ba = ae * RS_Vector::dotP(ae, ap)
                   / (ae.magnitude()*ae.magnitude());
    RS_Vector be = ea * RS_Vector::dotP(ea, ep)
                   / (ea.magnitude()*ea.magnitude());

    return data.point1+ba;
}



RS_Vector RS_ConstructionLine::getNearestCenter(const RS_Vector& /*coord*/,
        double* dist) {

    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }

    return RS_Vector(false);
}



RS_Vector RS_ConstructionLine::getNearestMiddle(const RS_Vector& /*coord*/,
        double* dist) {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}



RS_Vector RS_ConstructionLine::getNearestDist(double /*distance*/,
        const RS_Vector& /*coord*/,
        double* dist) {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}


double RS_ConstructionLine::getDistanceToPoint(const RS_Vector& coord,
        RS_Entity** entity,
        RS2::ResolveLevel /*level*/, double /*solidDist*/) {

    RS_DEBUG->print("RS_ConstructionLine::getDistanceToPoint");

    if (entity!=NULL) {
        *entity = this;
    }
    double dist = RS_MAXDOUBLE;
    RS_Vector ae = data.point2-data.point1;
    RS_Vector ea = data.point1-data.point2;
    RS_Vector ap = coord-data.point1;
    RS_Vector ep = coord-data.point2;
	
	if (ae.magnitude()<1.0e-6 || ea.magnitude()<1.0e-6) {
		return dist;
	}

    // Orthogonal projection from both sides:
    RS_Vector ba = ae * RS_Vector::dotP(ae, ap) /
                   RS_Math::pow(ae.magnitude(), 2);
    RS_Vector be = ea * RS_Vector::dotP(ea, ep) /
                   RS_Math::pow(ea.magnitude(), 2);

    RS_DEBUG->print("ba: %f", ba.magnitude());
    RS_DEBUG->print("ae: %f", ae.magnitude());

    RS_Vector cp = RS_Vector::crossP(ap, ae);
    dist = cp.magnitude() / ae.magnitude();

    return dist;
}



void RS_ConstructionLine::move(RS_Vector offset) {
    data.point1.move(offset);
    data.point2.move(offset);
    //calculateBorders();
}



void RS_ConstructionLine::rotate(RS_Vector center, double angle) {
    data.point1.rotate(center, angle);
    data.point2.rotate(center, angle);
    //calculateBorders();
}



void RS_ConstructionLine::scale(RS_Vector center, RS_Vector factor) {
    data.point1.scale(center, factor);
    data.point2.scale(center, factor);
    //calculateBorders();
}



void RS_ConstructionLine::mirror(RS_Vector axisPoint1, RS_Vector axisPoint2) {
	data.point1.mirror(axisPoint1, axisPoint2);
	data.point2.mirror(axisPoint1, axisPoint2);
}



/**
 * Dumps the point's data to stdout.
 */
std::ostream& operator << (std::ostream& os, const RS_ConstructionLine& l) {
    os << " ConstructionLine: " << l.getData() << "\n";
    return os;
}


