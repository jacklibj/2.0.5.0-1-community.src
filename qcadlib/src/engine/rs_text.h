/****************************************************************************
** $Id: rs_text.h 2367 2005-04-04 16:57:36Z andrew $
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


#ifndef RS_TEXT_H
#define RS_TEXT_H

#include "rs_entity.h"
#include "rs_entitycontainer.h"

/**
 * Holds the data that defines a text entity.
 */
class RS_TextData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_TextData() {}

    /**
     * Constructor with initialisation.
     *
     * @param insertionPoint Insertion point
     * @param height Nominal (initial) text height
     * @param width Reference rectangle width
     * @param valign Vertical alignment
     * @param halign Horizontal alignment
     * @param drawingDirection Drawing direction
     * @param lineSpacingStyle Line spacing style
     * @param lineSpacingFactor Line spacing factor
     * @param text Text string
     * @param style Text style name
     * @param angle Rotation angle
     * @param updateMode RS2::Update will update the text entity instantly
     *    RS2::NoUpdate will not update the entity. You can update
     *    it later manually using the update() method. This is
     *    often the case since you might want to adjust attributes
     *    after creating a text entity.
     */
    RS_TextData(const RS_Vector& insertionPoint,
                double height,
                double width,
                RS2::VAlign valign,
                RS2::HAlign halign,
                RS2::TextDrawingDirection drawingDirection,
                RS2::TextLineSpacingStyle lineSpacingStyle,
                double lineSpacingFactor,
                const RS_String& text,
                const RS_String& style,
                double angle,
                RS2::UpdateMode updateMode = RS2::Update) {
        this->insertionPoint = insertionPoint;
        this->height = height;
        this->width = width;
        this->valign = valign;
        this->halign = halign;
        this->drawingDirection = drawingDirection;
        this->lineSpacingStyle = lineSpacingStyle;
        this->lineSpacingFactor = lineSpacingFactor;
        this->style = style;
        this->angle = angle;
        this->text = text;
        this->updateMode = updateMode;
    }

    friend class RS_Text;

    friend std::ostream& operator << (std::ostream& os, const RS_TextData& td) {
        os << "(" << td.text.latin1() << ")";
        return os;
    }

public:
    /** Insertion point */
    RS_Vector insertionPoint;
    /** Nominal (initial) text height */
    double height;
    /** Reference rectangle width */
    double width;
    /** Vertical alignment */
    RS2::VAlign valign;
    /** Horizontal alignment */
    RS2::HAlign halign;
    /** Drawing direction */
    RS2::TextDrawingDirection drawingDirection;
    /** Line spacing style */
    RS2::TextLineSpacingStyle lineSpacingStyle;
    /** Line spacing factor */
    double lineSpacingFactor;
    /** Text string */
    RS_String text;
    /** Text style name */
    RS_String style;
    /** Rotation angle */
    double angle;
    /** Update mode */
    RS2::UpdateMode updateMode;
};



/**
 * Class for a text entity.
 * Please note that text strings can contain special 
 * characters such as %%c for a diameter sign as well as unicode
 * characters. Line feeds are stored as real line feeds in the string.
 *
 * @author Andrew Mustun
 */
class RS_Text : public RS_EntityContainer {
public:
    RS_Text(RS_EntityContainer* parent,
            const RS_TextData& d);
    virtual ~RS_Text() {}

    virtual RS_Entity* clone() {
        RS_Text* t = new RS_Text(*this);
		t->entities.setAutoDelete(entities.autoDelete());
        t->initId();
        t->detach();
        return t;
    }

    /**	@return RS2::EntityText */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityText;
    }

    /** @return Copy of data that defines the text. */
    RS_TextData getData() const {
        return data;
    }

    void update();
    void updateAddLine(RS_EntityContainer* textLine, int lineCounter);

    int getNumberOfLines();


    RS_Vector getInsertionPoint() {
        return data.insertionPoint;
    }
    double getHeight() {
        return data.height;
    }
    void setHeight(double h) {
        data.height = h;
    }
    double getWidth() {
        return data.width;
    }
    void setAlignment(int a);
    int getAlignment();
    RS2::VAlign getVAlign() {
        return data.valign;
    }
	void setVAlign(RS2::VAlign va) {
		data.valign = va;
	}
    RS2::HAlign getHAlign() {
        return data.halign;
    }
	void setHAlign(RS2::HAlign ha) {
		data.halign = ha;
	}
    RS2::TextDrawingDirection getDrawingDirection() {
        return data.drawingDirection;
    }
    RS2::TextLineSpacingStyle getLineSpacingStyle() {
        return data.lineSpacingStyle;
    }
    void setLineSpacingFactor(double f) {
        data.lineSpacingFactor = f;
    }
    double getLineSpacingFactor() {
        return data.lineSpacingFactor;
    }
    void setText(const RS_String& t);
    RS_String getText() {
        return data.text;
    }
    void setStyle(const RS_String& s) {
        data.style = s;
    }
    RS_String getStyle() {
        return data.style;
    }
	void setAngle(double a) {
		data.angle = a;
	}
    double getAngle() {
        return data.angle;
    }
    double getUsedTextWidth() {
        return usedTextWidth;
    }
    double getUsedTextHeight() {
        return usedTextHeight;
    }

	virtual double getLength() {
		return -1.0;
	}
	
    virtual RS_VectorSolutions getRefPoints();
    virtual RS_Vector getNearestRef(const RS_Vector& coord,
                                     double* dist = NULL);

    virtual void move(RS_Vector offset);
    virtual void rotate(RS_Vector center, double angle);
    virtual void scale(RS_Vector center, RS_Vector factor);
    virtual void mirror(RS_Vector axisPoint1, RS_Vector axisPoint2);
	virtual bool hasEndpointsWithinWindow(RS_Vector v1, RS_Vector v2);
    virtual void stretch(RS_Vector firstCorner,
                         RS_Vector secondCorner,
                         RS_Vector offset);

    friend std::ostream& operator << (std::ostream& os, const RS_Text& p);

protected:
    RS_TextData data;

    /**
     * Text width used by the current contents of this text entity. 
     * This property is updated by the update method.
     * @see update
     */
    double usedTextWidth;
    /**
     * Text height used by the current contents of this text entity.
     * This property is updated by the update method.
     * @see update
     */
    double usedTextHeight;
};

#endif
