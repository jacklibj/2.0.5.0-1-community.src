/****************************************************************************
** $Id: rs_leader.cpp 1938 2004-12-09 23:09:53Z andrew $
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


#include "rs_leader.h"

#include "rs_debug.h"
#include "rs_line.h"
#include "rs_solid.h"


/**
 * Constructor.
 */
RS_Leader::RS_Leader(RS_EntityContainer* parent)
        :RS_EntityContainer(parent) {

    empty = true;
}


/**
 * Constructor.
 * @param d Leader data
 */
RS_Leader::RS_Leader(RS_EntityContainer* parent,
                     const RS_LeaderData& d)
        :RS_EntityContainer(parent), data(d) {
    empty = true;
}



/**
 * Destructor 
 */
RS_Leader::~RS_Leader() {}



/**
 * Implementation of update. Updates the arrow.
 */
void RS_Leader::update() {

    // find and delete arrow:
    for (RS_Entity* e=firstEntity(); e!=NULL; e=nextEntity()) {
        if (e->rtti()==RS2::EntitySolid) {
            removeEntity(e);
            break;
        }
    }
	
	if (isUndone()) {
		setVisible(false);
		return;
	}

    RS_Entity* fe = firstEntity();
    if (fe!=NULL && fe->isAtomic()) {
        RS_Vector p1 = ((RS_AtomicEntity*)fe)->getStartpoint();
        RS_Vector p2 = ((RS_AtomicEntity*)fe)->getEndpoint();

        // first entity must be the line which gets the arrow:
        if (hasArrowHead()) {
            RS_Solid* s = new RS_Solid(this, RS_SolidData());
            s->shapeArrow(p1,
                          p2.angleTo(p1),
                          getGraphicVariableDouble("$DIMASZ", 2.5));
            s->setPen(RS_Pen(RS2::FlagInvalid));
            s->setLayer(NULL);
            RS_EntityContainer::addEntity(s);
        }
    }
}



/**
 * Adds a vertex from the endpoint of the last element or 
 * sets the startpoint to the point 'v'.
 *
 * The very first vertex added is the starting point.
 * 
 * @param v vertex coordinate
 *
 * @return Pointer to the entity that was addded or NULL if this
 *         was the first vertex added.
 */
RS_Entity* RS_Leader::addVertex(const RS_Vector& v) {

    RS_Entity* entity=NULL;
    static RS_Vector last = RS_Vector(false);

    if (empty) {
        last = v;
        empty = false;
    } else {
        // add line to the leader:
        entity = new RS_Line(this, RS_LineData(last, v));
        entity->setPen(RS_Pen(RS2::FlagInvalid));
        entity->setLayer(NULL);
        RS_EntityContainer::addEntity(entity);

		if (count()==1 && hasArrowHead()) {
			update();
		}

        last = v;
    }

    return entity;
}



/**
 * Reimplementation of the addEntity method for a normal container.
 * This reimplementation deletes the given entity!
 *
 * To add entities use addVertex() instead.
 */
void RS_Leader::addEntity(RS_Entity* entity) {
    RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Leader::addEntity:"
                    " should never be called");

    if (entity==NULL) {
        return;
    }
    delete entity;
}



void RS_Leader::move(RS_Vector offset) {
    RS_EntityContainer::move(offset);
    update();
}



void RS_Leader::rotate(RS_Vector center, double angle) {
    RS_EntityContainer::rotate(center, angle);
    update();
}



void RS_Leader::scale(RS_Vector center, RS_Vector factor) {
    RS_EntityContainer::scale(center, factor);
    update();
}



void RS_Leader::mirror(RS_Vector axisPoint1, RS_Vector axisPoint2) {
    RS_EntityContainer::mirror(axisPoint1, axisPoint2);
    update();
}


void RS_Leader::stretch(RS_Vector firstCorner,
                       RS_Vector secondCorner,
                       RS_Vector offset) {

    RS_EntityContainer::stretch(firstCorner, secondCorner, offset);
    update();
}

/**
 * Dumps the leader's data to stdout.
 */
std::ostream& operator << (std::ostream& os, const RS_Leader& l) {
    os << " Leader: " << l.getData() << " {\n";

    os << (RS_EntityContainer&)l;

    os << "\n}\n";

    return os;
}

