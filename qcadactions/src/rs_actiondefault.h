/****************************************************************************
** $Id: rs_actiondefault.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONSELECTWINDOW_H
#define RS_ACTIONSELECTWINDOW_H

#include "rs_previewactioninterface.h"
#include "rs_vector.h"


/**
 * This action class can handle user events to select all entities.
 *
 * @author Andrew Mustun
 */
class RS_ActionDefault : public RS_PreviewActionInterface {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        Neutral,        /**< we don't know what we do yet.  */
        Dragging,       /**< dragging (either an entity or the
        		                     first part of a selection window) */
        SetCorner2,     /**< Setting the 2nd corner of a selection window. */
        Moving,         /**< Moving entities (drag'n'drop) */
        MovingRef       /**< Moving a reference point of one or more selected
        		                     entities */
    };

public:
    RS_ActionDefault(RS_EntityContainer& container,
                     RS_GraphicView& graphicView);
    virtual ~RS_ActionDefault();
	
	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

    virtual RS2::ActionType rtti() {
        return RS2::ActionDefault;
    }

    virtual void init(int status=0);

    virtual void trigger();

    virtual void keyPressEvent(RS_KeyEvent* e);
    virtual void keyReleaseEvent(RS_KeyEvent* e);

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mousePressEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

    virtual void commandEvent(RS_CommandEvent* e);
    virtual RS_StringList getAvailableCommands();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

protected:
    RS_Vector v1;
    RS_Vector v2;
	RS2::SnapRestriction restrBak;

};

#endif
