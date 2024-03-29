/****************************************************************************
** $Id: rs_dimangular.h 2367 2005-04-04 16:57:36Z andrew $
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


#ifndef RS_DIMANGULAR_H
#define RS_DIMANGULAR_H

#include "rs_dimension.h"

/**
 * Holds the data that defines a angular dimension entity.
 */
class RS_DimAngularData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_DimAngularData() {}

    /**
     * Constructor with initialisation.
     *
     * @param definitionPoint Definition point of the angular dimension. 
     * @param leader Leader length.
     */
    RS_DimAngularData(const RS_Vector& definitionPoint1,
                      const RS_Vector& definitionPoint2,
					  const RS_Vector& definitionPoint3,
					  const RS_Vector& definitionPoint4) {
        this->definitionPoint1 = definitionPoint1;
        this->definitionPoint2 = definitionPoint2;
        this->definitionPoint3 = definitionPoint3;
        this->definitionPoint4 = definitionPoint4;
    }

    friend std::ostream& operator << (std::ostream& os,
                                      const RS_DimAngularData& dd) {
        os << "(" << dd.definitionPoint1 << "/" << dd.definitionPoint2 << "/"
		          << dd.definitionPoint3 << "/" << dd.definitionPoint3 << ")";
        return os;
    }

public:
    /** Definition point 1. */
    RS_Vector definitionPoint1;
    /** Definition point 2. */
    RS_Vector definitionPoint2;
    /** Definition point 3. */
    RS_Vector definitionPoint3;
    /** Definition point 4. */
    RS_Vector definitionPoint4;
};



/**
 * Class for angular dimension entities.
 *
 * @author Andrew Mustun
 */
class RS_DimAngular : public RS_Dimension {
public:
    RS_DimAngular(RS_EntityContainer* parent,
                 const RS_DimensionData& d,
                 const RS_DimAngularData& ed);
    virtual ~RS_DimAngular() {}

    virtual RS_Entity* clone() {
        RS_DimAngular* d = new RS_DimAngular(*this);
		d->entities.setAutoDelete(entities.autoDelete());
        d->initId();
        d->detach();
        return d;
    }

    /**	@return RS2::EntityDimAngular */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityDimAngular;
    }

    /**
     * @return Copy of data that defines the angular dimension. 
     * @see getData()
     */
    RS_DimAngularData getEData() const {
        return edata;
    }

    virtual RS_String getMeasuredLabel();
    double getAngle();
    RS_Vector getCenter();
	bool getAngles(double& ang1, double& ang2, bool& reversed,
		RS_Vector& p1, RS_Vector& p2);

    virtual void update(bool autoText=false);

    RS_Vector getDefinitionPoint1() {
        return edata.definitionPoint1;
    }
    RS_Vector getDefinitionPoint2() {
        return edata.definitionPoint2;
    }
    RS_Vector getDefinitionPoint3() {
        return edata.definitionPoint3;
    }
    RS_Vector getDefinitionPoint4() {
        return edata.definitionPoint4;
    }

    virtual void move(RS_Vector offset);
    virtual void rotate(RS_Vector center, double angle);
    virtual void scale(RS_Vector center, RS_Vector factor);
    virtual void mirror(RS_Vector axisPoint1, RS_Vector axisPoint2);

    friend std::ostream& operator << (std::ostream& os,
                                      const RS_DimAngular& d);

protected:
    /** Extended data. */
    RS_DimAngularData edata;
};

#endif
