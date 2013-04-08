/****************************************************************************
** $Id: rs_actiondrawarc.h 1062 2004-01-16 21:51:20Z andrew $
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

#ifndef RS_ACTIONDRAWARC_H
#define RS_ACTIONDRAWARC_H

#include "rs_previewactioninterface.h"
#include "rs_arc.h"

/**
 * This action class can handle user events to draw 
 * simple arcs with the center, radius, start- and endangle given.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawArc : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetCenter,       /**< Setting the center point. */
        SetRadius,       /**< Setting the radius. */
        SetAngle1,       /**< Setting the startpoint.  */
        SetAngle2,       /**< Setting the endpoint. */
		SetIncAngle,     /**< Setting the included angle. */
		SetChordLength   /**< Setting carc chord length. */
    };

public:
    RS_ActionDrawArc(RS_EntityContainer& container,
                     RS_GraphicView& graphicView);
    ~RS_ActionDrawArc();
	
	static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);
	
	virtual RS2::ActionType rtti() {
		return RS2::ActionDrawArc;
	}

    void reset();

    virtual void init(int status=0);
    virtual void trigger();
	
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

	virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
	virtual RS_StringList getAvailableCommands();
	
    virtual void hideOptions();
    virtual void showOptions();
	
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

	bool isReversed() {
		return data.reversed;
	}

	void setReversed(bool r) {
		data.reversed = r;
	}

protected:
    /**
     * Arc data defined so far.
     */
    RS_ArcData data;
	/**
	 * Commands
	 */
	/*RS_String cmdReversed;
	RS_String cmdReversed2;
	RS_String cmdReversed3;
	*/
};

#endif
