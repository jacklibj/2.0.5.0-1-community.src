/****************************************************************************
** $Id: qg_actionfactory.cpp 2373 2005-04-29 11:44:58Z andrew $
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

#include "qg_actionfactory.h"

#include <qobject.h>

#include "rs_actionblockscreate.h"
#include "rs_actionblocksfreezeall.h"
#include "rs_actionblocksadd.h"
#include "rs_actionblocksremove.h"
#include "rs_actionblocksattributes.h"
#include "rs_actionblocksedit.h"
#include "rs_actionblocksinsert.h"
#include "rs_actionblockstoggleview.h"
#include "rs_actionblocksexplode.h"
#include "rs_actiondimaligned.h"
#include "rs_actiondimangular.h"
#include "rs_actiondimdiametric.h"
#include "rs_actiondimleader.h"
#include "rs_actiondimlinear.h"
#include "rs_actiondimradial.h"
#include "rs_actiondrawarc.h"
#include "rs_actiondrawarc3p.h"
#include "rs_actiondrawcircle.h"
#include "rs_actiondrawcircle2p.h"
#include "rs_actiondrawcircle3p.h"
#include "rs_actiondrawcirclecr.h"
#include "rs_actiondrawellipseaxis.h"
#include "rs_actiondrawhatch.h"
#include "rs_actiondrawimage.h"
#include "rs_actiondrawline.h"
#include "rs_actiondrawlineangle.h"
#include "rs_actiondrawlinebisector.h"
#include "rs_actiondrawlinefree.h"
#include "rs_actiondrawlinehorvert.h"
#include "rs_actiondrawlineparallel.h"
#include "rs_actiondrawlineparallelthrough.h"
#include "rs_actiondrawlinepolygon.h"
#include "rs_actiondrawlinepolygon2.h"
#include "rs_actiondrawlinerectangle.h"
#include "rs_actiondrawlinerelangle.h"
#include "rs_actiondrawlinetangent1.h"
#include "rs_actiondrawlinetangent2.h"
#include "rs_actiondrawpoint.h"
#include "rs_actiondrawspline.h"
#include "rs_actiondrawtext.h"
#include "rs_actioneditcopy.h"
#include "rs_actioneditpaste.h"
#include "rs_actioneditundo.h"
#include "rs_actionfilenew.h"
#include "rs_actionfileopen.h"
#include "rs_actionfilesave.h"
#include "rs_actionfilesaveas.h"
#include "rs_actioninfoangle.h"
#include "rs_actioninfodist.h"
#include "rs_actioninfodist2.h"
#include "rs_actioninfoinside.h"
#include "rs_actioninfototallength.h"
#include "rs_actionlayersadd.h"
#include "rs_actionlayersedit.h"
#include "rs_actionlayersfreezeall.h"
#include "rs_actionlayersremove.h"
#include "rs_actionlayerstogglelock.h"
#include "rs_actionlayerstoggleview.h"
#include "rs_actionlockrelativezero.h"
#include "rs_actionmodifyattributes.h"
#include "rs_actionmodifybevel.h"
#include "rs_actionmodifymirror.h"
#include "rs_actionmodifycut.h"
#include "rs_actionmodifydelete.h"
#include "rs_actionmodifydeletefree.h"
#include "rs_actionmodifydeletequick.h"
#include "rs_actionmodifyentity.h"
#include "rs_actionmodifymove.h"
#include "rs_actionmodifymoverotate.h"
#include "rs_actionmodifyrotate.h"
#include "rs_actionmodifyround.h"
#include "rs_actionmodifyscale.h"
#include "rs_actionmodifystretch.h"
#include "rs_actionmodifytrim.h"
#include "rs_actionmodifytrimamount.h"
#include "rs_actionmodifyexplodetext.h"
#include "rs_actionoptionsdrawing.h"
#include "rs_actionprintpreview.h"
#include "rs_actionselectall.h"
#include "rs_actionselectintersected.h"
#include "rs_actionselectinvert.h"
#include "rs_actionselectlayer.h"
#include "rs_actionselectsingle.h"
#include "rs_actionselectcontour.h"
#include "rs_actionselectwindow.h"
#include "rs_actionsetrelativezero.h"
#include "rs_actionsnapintersectionmanual.h"
#include "rs_actiontoolregeneratedimensions.h"
#include "rs_actionzoomauto.h"
#include "rs_actionzoomprevious.h"
#include "rs_actionzoomin.h"
#include "rs_actionzoompan.h"
#include "rs_actionzoomredraw.h"
#include "rs_actionzoomwindow.h"

#ifdef RS_PROF
#include "rs_actiondrawpolyline.h"
#include "rs_actionpolylineadd.h"
#include "rs_actionpolylinedel.h"
#include "rs_actionpolylinedelbetween.h"
#include "rs_actionpolylinetrim.h"
#endif

#ifdef RS_CAM
#include "rs_actioncamexportauto.h"
#include "rs_actioncamreorder.h"
#endif



/**
 * Constructor.
 *
 * @param ah Action handler which provides the slots.
 * @param w Widget through which the events come in.
 */
QG_ActionFactory::QG_ActionFactory(QG_ActionHandler* ah, QWidget* w) {
    actionHandler = ah;
    widget = w;
}



/**
 * Destructor
 */
QG_ActionFactory::~QG_ActionFactory() {}



/**
 * Creates a new action object and links it to the appropriate slot(s).
 *
 * @param id ID of the action to create (see rs.h).
 * @param obj Object which the action will connect its signal to.
 *
 * @return Pointer to the action object or NULL if the action is unknown.
 */
QAction* QG_ActionFactory::createAction(RS2::ActionType id, QObject* obj) {
    // assert that action handler is not invalid:
    if (actionHandler==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
        	"QG_ActionFactory::createAction: "
			"No valid action handler available to create action. id: %d", id);
        return NULL;
    }

    QWidget* mw = widget;
    QAction* action = NULL;
    QPixmap icon;

    if (mw==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
        	"QG_ActionFactory::createAction: "
			"No valid main window available to create action. id: %d ", id);
        return NULL;
    }

    // create requested action
    switch (id) {

        // File actions:
        //
    case RS2::ActionFileNew:
		action = RS_ActionFileNew::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotFileNew()));
        break;

    case RS2::ActionFileOpen:
		action = RS_ActionFileOpen::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotFileOpen()));
        break;

    case RS2::ActionFileSave:
		action = RS_ActionFileSave::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotFileSave()));
        break;

    case RS2::ActionFileSaveAs:
		action = RS_ActionFileSaveAs::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotFileSaveAs()));
        break;

    case RS2::ActionFileExport:
        action = new QAction(tr("Export Drawing"), tr("&Export..."),
                             0, mw);
        action->setStatusTip(tr("Exports the current drawing as bitmap"));
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotFileExport()));
        break;

    case RS2::ActionFileClose:
        action = new QAction(tr("Close Drawing"), QPixmap::fromMimeSource("fileclose.png"), tr("&Close"),
                             CTRL+Key_W, mw);
        action->setStatusTip(tr("Closes the current drawing"));
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotFileClose()));
        break;

    case RS2::ActionFilePrint:
        icon = QPixmap::fromMimeSource("fileprint.png");
        action = new QAction(tr("Print Drawing"), icon, tr("&Print..."),
                             CTRL+Key_P, mw);
        action->setStatusTip(tr("Prints out the current drawing"));
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotFilePrint()));
        break;

    case RS2::ActionFilePrintPreview:
		action = RS_ActionPrintPreview::createGUIAction(id, mw);
		action->setToggleAction(true);
        connect(action, SIGNAL(toggled(bool)),
                obj, SLOT(slotFilePrintPreview(bool)));
        break;

    case RS2::ActionFileQuit:
        action = new QAction(tr("Quit"), QPixmap::fromMimeSource("exit.png"), 
							tr("&Quit"),
                             CTRL+Key_Q, mw);
        action->setStatusTip(tr("Quits the application"));
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotFileQuit()));
        break;

        // Viewing actions:
        //
    case RS2::ActionViewGrid:
        icon = QPixmap::fromMimeSource("viewgrid.png");
        action = new QAction(tr("Grid"), icon, tr("&Grid"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Enables/disables the grid"));
        connect(action, SIGNAL(toggled(bool)),
                obj, SLOT(slotViewGrid(bool)));
        break;
    case RS2::ActionViewDraft:
        icon = QPixmap::fromMimeSource("viewdraft.png");
        action = new QAction(tr("Draft"), icon, tr("&Draft"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Enables/disables the draft mode"));
        connect(action, SIGNAL(toggled(bool)),
                obj, SLOT(slotViewDraft(bool)));
        break;
    case RS2::ActionViewStatusBar:
        action = new QAction(tr("Statusbar"), tr("&Statusbar"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Enables/disables the statusbar"));
        connect(action, SIGNAL(toggled(bool)),
                obj, SLOT(slotViewStatusBar(bool)));
        break;
        /*
                   case RS2::ActionViewLayerList:
                       action = new QAction(tr("Layer List"), tr("&Layer List"),
                                            0, mw, 0, true);
                       action->setStatusTip(tr("Enables/disables the layerlist"));
                       connect(action, SIGNAL(toggled(bool)),
                               obj, SLOT(slotViewLayerList(bool)));
                       break;
         
                   case RS2::ActionViewBlockList:
                       action = new QAction(tr("Block List"), tr("&Block List"),
                                            0, mw, 0, true);
                       action->setStatusTip(tr("Enables/disables the blocklist"));
                       connect(action, SIGNAL(toggled(bool)),
                               obj, SLOT(slotViewBlockList(bool)));
                       break;
         
                   case RS2::ActionViewCommandLine:
                       action = new QAction(tr("Command Widget"), tr("&Command Widget"),
                                            0, mw, 0, true);
                       action->setStatusTip(tr("Enables/disables the command widget"));
                       connect(action, SIGNAL(toggled(bool)),
                               obj, SLOT(slotViewCommandLine(bool)));
                       break;
         
                   case RS2::ActionViewOptionToolbar:
                       action = new QAction(tr("Option Toolbar"), tr("&Option Toolbar"),
                                            0, mw, 0, true);
                       action->setStatusTip(tr("Enables/disables the option toolbar"));
                       connect(action, SIGNAL(toggled(bool)),
                               obj, SLOT(slotViewOptionToolbar(bool)));
                       break;
                	*/

        // Tools:
        //
    case RS2::ActionToolRegenerateDimensions:
		action = RS_ActionToolRegenerateDimensions::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotToolRegenerateDimensions()));
        break;

        // Zooming actions:
        //
    case RS2::ActionZoomIn:
		action = RS_ActionZoomIn::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotZoomIn()));
        break;

    case RS2::ActionZoomOut:
		action = RS_ActionZoomIn::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotZoomOut()));
        break;

    case RS2::ActionZoomAuto:
		action = RS_ActionZoomAuto::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotZoomAuto()));
        break;

    case RS2::ActionZoomWindow:
		action = RS_ActionZoomWindow::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotZoomWindow()));
        break;

    case RS2::ActionZoomPan:
		action = RS_ActionZoomPan::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotZoomPan()));
        break;
    
	case RS2::ActionZoomPrevious:
		action = RS_ActionZoomPrevious::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotZoomPrevious()));
        break;

    case RS2::ActionZoomRedraw:
		action = RS_ActionZoomRedraw::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotZoomRedraw()));
        break;

        // Editing actions:
        //
    case RS2::ActionEditUndo:
		action = RS_ActionEditUndo::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotEditUndo()));
        break;

    case RS2::ActionEditRedo:
		action = RS_ActionEditUndo::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotEditRedo()));
        break;

    case RS2::ActionEditCut:
		action = RS_ActionEditCopy::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotEditCut()));
        break;

    case RS2::ActionEditCopy:
		action = RS_ActionEditCopy::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotEditCopy()));
        break;

    case RS2::ActionEditPaste:
		action = RS_ActionEditPaste::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotEditPaste()));
        break;

        // Selecting actions:
        //
    case RS2::ActionSelectSingle:
		action = RS_ActionSelectSingle::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSelectSingle()));
        break;

    case RS2::ActionSelectWindow:
		action = RS_ActionSelectWindow::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSelectWindow()));
        break;

    case RS2::ActionDeselectWindow:
		action = RS_ActionSelectWindow::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDeselectWindow()));
        break;

    case RS2::ActionSelectContour:
		action = RS_ActionSelectContour::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSelectContour()));
        break;

    case RS2::ActionSelectAll:
		action = RS_ActionSelectAll::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSelectAll()));
        break;

    case RS2::ActionDeselectAll:
		action = RS_ActionSelectAll::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDeselectAll()));
        break;

    case RS2::ActionSelectInvert:
		action = RS_ActionSelectInvert::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSelectInvert()));
        break;

    case RS2::ActionSelectIntersected:
		action = RS_ActionSelectIntersected::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSelectIntersected()));
        break;

    case RS2::ActionDeselectIntersected:
		action = RS_ActionSelectIntersected::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDeselectIntersected()));
        break;

    case RS2::ActionSelectLayer:
		action = RS_ActionSelectLayer::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSelectLayer()));
        break;

        // Drawing actions:
        //
    case RS2::ActionDrawPoint:
		action = RS_ActionDrawPoint::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawPoint()));
        break;

    case RS2::ActionDrawLine:
		action = RS_ActionDrawLine::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLine()));
        break;

    case RS2::ActionDrawLineAngle:
		action = RS_ActionDrawLineAngle::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineAngle()));
        break;

    case RS2::ActionDrawLineHorizontal:
		action = RS_ActionDrawLineAngle::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineHorizontal()));
        break;

    case RS2::ActionDrawLineHorVert:
		action = RS_ActionDrawLineHorVert::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineHorVert()));
        break;

    case RS2::ActionDrawLineVertical:
		action = RS_ActionDrawLineAngle::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineVertical()));
        break;

    case RS2::ActionDrawLineFree:
		action = RS_ActionDrawLineFree::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineFree()));
        break;

    case RS2::ActionDrawLineParallel:
		action = RS_ActionDrawLineParallel::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineParallel()));
        break;

    case RS2::ActionDrawLineParallelThrough:
		action = RS_ActionDrawLineParallelThrough::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineParallelThrough()));
        break;

    case RS2::ActionDrawLineRectangle:
		action = RS_ActionDrawLineRectangle::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineRectangle()));
        break;

    case RS2::ActionDrawLineBisector:
		action = RS_ActionDrawLineBisector::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineBisector()));
        break;

    case RS2::ActionDrawLineTangent1:
		action = RS_ActionDrawLineTangent1::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineTangent1()));
        break;

    case RS2::ActionDrawLineTangent2:
		action = RS_ActionDrawLineTangent2::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineTangent2()));
        break;

    case RS2::ActionDrawLineOrthogonal:
		action = RS_ActionDrawLineRelAngle::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineOrthogonal()));
        break;

    case RS2::ActionDrawLineRelAngle:
		action = RS_ActionDrawLineRelAngle::createGUIAction(id, mw);
		/*
        action = new QAction(tr("Relative angle"), tr("R&elative angle"),
                             0, mw);
        action->setStatusTip(tr("Draw line with relative angle"));
		*/
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLineRelAngle()));
        break;

#ifdef RS_PROF
    case RS2::ActionDrawPolyline:
		action = RS_ActionDrawPolyline::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawPolyline()));
        break;
#endif
		
    case RS2::ActionDrawLinePolygon:
		action = RS_ActionDrawLinePolygon::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLinePolygon()));
        break;

    case RS2::ActionDrawLinePolygon2:
		action = RS_ActionDrawLinePolygon::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawLinePolygon2()));
        break;

    case RS2::ActionDrawCircle:
		action = RS_ActionDrawCircle::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawCircle()));
        break;

    case RS2::ActionDrawCircleCR:
		action = RS_ActionDrawCircleCR::createGUIAction(id, mw);
		/*
        action = new QAction(tr("Circle: Center, Radius"),
                             tr("Center, &Radius"),
                             0, mw);
        action->setStatusTip(tr("Draw circles with center and radius"));
		*/
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawCircleCR()));
        break;

    case RS2::ActionDrawCircle2P:
		action = RS_ActionDrawCircle2P::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawCircle2P()));
        break;

    case RS2::ActionDrawCircle3P:
		action = RS_ActionDrawCircle3P::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawCircle3P()));
        break;

    case RS2::ActionDrawCircleParallel:
		action = RS_ActionDrawLineParallel::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawCircleParallel()));
        break;

    case RS2::ActionDrawArc:
		action = RS_ActionDrawArc::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawArc()));
        break;

    case RS2::ActionDrawArc3P:
		action = RS_ActionDrawArc3P::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawArc3P()));
        break;

    case RS2::ActionDrawArcParallel:
		action = RS_ActionDrawLineParallel::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawArcParallel()));
        break;

    case RS2::ActionDrawEllipseAxis:
		action = RS_ActionDrawEllipseAxis::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawEllipseAxis()));
        break;

    case RS2::ActionDrawEllipseArcAxis:
		action = RS_ActionDrawEllipseAxis::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawEllipseArcAxis()));
        break;
		
    case RS2::ActionDrawSpline:
		action = RS_ActionDrawSpline::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawSpline()));
        break;
    
#ifdef RS_PROF
	case RS2::ActionPolylineAdd:
		action = RS_ActionPolylineAdd::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotPolylineAdd()));
        break;
	
	case RS2::ActionPolylineDel:
		action = RS_ActionPolylineDel::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotPolylineDel()));
        break;
	
	case RS2::ActionPolylineDelBetween:
		action = RS_ActionPolylineDelBetween::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotPolylineDelBetween()));
        break;
	
	case RS2::ActionPolylineTrim:
		action = RS_ActionPolylineTrim::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotPolylineTrim()));
        break;
#endif

    case RS2::ActionDrawText:
		action = RS_ActionDrawText::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawText()));
        break;

    case RS2::ActionDrawHatch:
		action = RS_ActionDrawHatch::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawHatch()));
        break;

    case RS2::ActionDrawImage:
		action = RS_ActionDrawImage::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDrawImage()));
        break;

        // Dimensioning actions:
        //
    case RS2::ActionDimAligned:
		action = RS_ActionDimAligned::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDimAligned()));
        break;

    case RS2::ActionDimLinear:
		action = RS_ActionDimLinear::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDimLinear()));
        break;

    case RS2::ActionDimLinearHor:
		action = RS_ActionDimLinear::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDimLinearHor()));
        break;

    case RS2::ActionDimLinearVer:
		action = RS_ActionDimLinear::createGUIAction(id, mw);
		
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDimLinearVer()));
        break;

    case RS2::ActionDimRadial:
		action = RS_ActionDimRadial::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDimRadial()));
        break;

    case RS2::ActionDimDiametric:
		action = RS_ActionDimDiametric::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDimDiametric()));
        break;

    case RS2::ActionDimAngular:
		action = RS_ActionDimAngular::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDimAngular()));
        break;

    case RS2::ActionDimLeader:
		action = RS_ActionDimLeader::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotDimLeader()));
        break;

        // Modifying actions:
        //
    case RS2::ActionModifyAttributes:
		action = RS_ActionModifyAttributes::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyAttributes()));
        break;
    case RS2::ActionModifyDelete:
		action = RS_ActionModifyDelete::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyDelete()));
        break;

    case RS2::ActionModifyDeleteQuick:
		action = RS_ActionModifyDeleteQuick::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyDeleteQuick()));
        break;

    case RS2::ActionModifyDeleteFree:
		action = RS_ActionModifyDeleteFree::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyDeleteFree()));
        break;

    case RS2::ActionModifyMove:
		action = RS_ActionModifyMove::createGUIAction(id, mw);
		/*
        action = new QAction(tr("Move"), tr("&Move"),
                             0, mw);
        action->setStatusTip(tr("Move Entities"));
		*/
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyMove()));
        break;

    case RS2::ActionModifyRotate:
		action = RS_ActionModifyRotate::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyRotate()));
        break;

    case RS2::ActionModifyScale:
		action = RS_ActionModifyScale::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyScale()));
        break;

    case RS2::ActionModifyMirror:
		action = RS_ActionModifyMirror::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyMirror()));
        break;

    case RS2::ActionModifyMoveRotate:
		action = RS_ActionModifyMoveRotate::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyMoveRotate()));
        break;

    case RS2::ActionModifyRotate2:
		action = RS_ActionModifyRotate::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyRotate2()));
        break;

    case RS2::ActionModifyEntity:
		action = RS_ActionModifyEntity::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyEntity()));
        break;

    case RS2::ActionModifyTrim:
		action = RS_ActionModifyTrim::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyTrim()));
        break;

    case RS2::ActionModifyTrim2:
		action = RS_ActionModifyTrim::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyTrim2()));
        break;

    case RS2::ActionModifyTrimAmount:
		action = RS_ActionModifyTrimAmount::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyTrimAmount()));
        break;

    case RS2::ActionModifyCut:
		action = RS_ActionModifyCut::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyCut()));
        break;

    case RS2::ActionModifyStretch:
		action = RS_ActionModifyStretch::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyStretch()));
        break;

    case RS2::ActionModifyBevel:
		action = RS_ActionModifyBevel::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyBevel()));
        break;

    case RS2::ActionModifyRound:
		action = RS_ActionModifyRound::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyRound()));
        break;

    case RS2::ActionModifyExplodeText:
		action = RS_ActionModifyExplodeText::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotModifyExplodeText()));
        break;

        // Snapping actions:
        //
    case RS2::ActionSnapFree:
        action = new QAction(tr("Free"), tr("&Free"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Free positioning"));
        actionHandler->setActionSnapFree(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSnapFree()));
        break;

    case RS2::ActionSnapGrid:
        action = new QAction(tr("Grid"), tr("&Grid"),
                             0, mw, "snapGrid", true);
        action->setStatusTip(tr("Grid positioning"));
        actionHandler->setActionSnapGrid(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSnapGrid()));
        break;

    case RS2::ActionSnapEndpoint:
        action = new QAction(tr("Endpoints"), tr("&Endpoints"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Snap to endpoints"));
        actionHandler->setActionSnapEndpoint(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSnapEndpoint()));
        break;

    case RS2::ActionSnapOnEntity:
        action = new QAction(tr("On Entity"), tr("&On Entity"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Snap to nearest point on entity"));
        actionHandler->setActionSnapOnEntity(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSnapOnEntity()));
        break;

    case RS2::ActionSnapCenter:
        action = new QAction(tr("Center"), tr("&Center"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Snap to centers"));
        actionHandler->setActionSnapCenter(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSnapCenter()));
        break;

    case RS2::ActionSnapMiddle:
        action = new QAction(tr("Middle"), tr("&Middle"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Snap to middle points"));
        actionHandler->setActionSnapMiddle(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSnapMiddle()));
        break;

    case RS2::ActionSnapDist:
        action = new QAction(tr("Distance from Endpoint"),
                             tr("&Distance from Endpoint"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Snap to points with a given"
                                " distance to an endpoint"));
        actionHandler->setActionSnapDist(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSnapDist()));
        break;

    case RS2::ActionSnapIntersection:
        action = new QAction(tr("Intersection"), tr("&Intersection"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Snap to intersection points"));
        actionHandler->setActionSnapIntersection(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSnapIntersection()));
        break;

    case RS2::ActionSnapIntersectionManual:
		action = RS_ActionSnapIntersectionManual::createGUIAction(id, mw);
        actionHandler->setActionSnapIntersectionManual(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSnapIntersectionManual()));
        break;

        // Snap restriction actions:
        //
    case RS2::ActionRestrictNothing:
        action = new QAction(tr("Restrict Nothing"), tr("Restrict &Nothing"),
                             0, mw, 0, true);
        action->setStatusTip(tr("No snap restriction"));
        actionHandler->setActionRestrictNothing(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotRestrictNothing()));
        break;

    case RS2::ActionRestrictOrthogonal:
        action = new QAction(tr("Restrict Orthogonally"),
                             tr("Restrict &Orthogonally"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Restrict snapping orthogonally"));
        actionHandler->setActionRestrictOrthogonal(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotRestrictOrthogonal()));
        break;

    case RS2::ActionRestrictHorizontal:
        action = new QAction(tr("Restrict Horizontally"),
                             tr("Restrict &Horizontally"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Restrict snapping horizontally"));
        actionHandler->setActionRestrictHorizontal(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotRestrictHorizontal()));
        break;

    case RS2::ActionRestrictVertical:
        action = new QAction(tr("Restrict Vertically"),
                             tr("Restrict &Vertically"),
                             0, mw, 0, true);
        action->setStatusTip(tr("Restrict snapping vertically"));
        actionHandler->setActionRestrictVertical(action);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotRestrictVertical()));
        break;

        // Relative zero point
        //
    case RS2::ActionSetRelativeZero:
		action = RS_ActionSetRelativeZero::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotSetRelativeZero()));
        break;
    case RS2::ActionLockRelativeZero:
		action = RS_ActionLockRelativeZero::createGUIAction(id, mw);
        actionHandler->setActionLockRelativeZero(action);
        connect(action, SIGNAL(toggled(bool)),
                obj, SLOT(slotLockRelativeZero(bool)));
        break;

        // Info actions:
        //
    case RS2::ActionInfoInside:
		action = RS_ActionInfoInside::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotInfoInside()));
        break;

    case RS2::ActionInfoDist:
		action = RS_ActionInfoDist::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotInfoDist()));
        break;

    case RS2::ActionInfoDist2:
		action = RS_ActionInfoDist2::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotInfoDist2()));
        break;

    case RS2::ActionInfoAngle:
		action = RS_ActionInfoAngle::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotInfoAngle()));
        break;
		
    case RS2::ActionInfoTotalLength:
		action = RS_ActionInfoTotalLength::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotInfoTotalLength()));
        break;

        // Layer actions:
        //
    case RS2::ActionLayersDefreezeAll:
		action = RS_ActionLayersFreezeAll::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotLayersDefreezeAll()));
        break;
    case RS2::ActionLayersFreezeAll:
		action = RS_ActionLayersFreezeAll::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotLayersFreezeAll()));
        break;
    case RS2::ActionLayersAdd:
		action = RS_ActionLayersAdd::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotLayersAdd()));
        break;

    case RS2::ActionLayersRemove:
		action = RS_ActionLayersRemove::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotLayersRemove()));
        break;

    case RS2::ActionLayersEdit:
		action = RS_ActionLayersEdit::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotLayersEdit()));
        break;
		
    case RS2::ActionLayersToggleLock:
		action = RS_ActionLayersToggleLock::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotLayersToggleView()));
        break;

    case RS2::ActionLayersToggleView:
		action = RS_ActionLayersToggleView::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotLayersToggleView()));
        break;

        // Block actions:
        //
    case RS2::ActionBlocksDefreezeAll:
		action = RS_ActionBlocksFreezeAll::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotBlocksDefreezeAll()));
        break;
    case RS2::ActionBlocksFreezeAll:
		action = RS_ActionBlocksFreezeAll::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotBlocksFreezeAll()));
        break;
    case RS2::ActionBlocksAdd:
		action = RS_ActionBlocksAdd::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotBlocksAdd()));
        break;
    case RS2::ActionBlocksRemove:
		action = RS_ActionBlocksRemove::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotBlocksRemove()));
        break;
    case RS2::ActionBlocksAttributes:
		action = RS_ActionBlocksAttributes::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotBlocksAttributes()));
        break;
    case RS2::ActionBlocksEdit:
		action = RS_ActionBlocksEdit::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotBlocksEdit()));
        break;
    case RS2::ActionBlocksInsert:
		action = RS_ActionBlocksInsert::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotBlocksInsert()));
        break;
    case RS2::ActionBlocksToggleView:
		action = RS_ActionBlocksToggleView::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotBlocksToggleView()));
        break;
    case RS2::ActionBlocksCreate:
		action = RS_ActionBlocksCreate::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotBlocksCreate()));
        break;
    case RS2::ActionBlocksExplode:
		action = RS_ActionBlocksExplode::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotBlocksExplode()));
        break;
		

        // Options actions:
        //
    case RS2::ActionOptionsGeneral:
        action = new QAction(tr("Application"), QPixmap::fromMimeSource("configure.png"),
#ifdef __APPLE__
							 tr("&Preferences"),
#else
                             tr("&Application Preferences")+"...",
#endif
                             0, mw);
        action->setStatusTip(tr("General Application Preferences"));
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotOptionsGeneral()));
        break;

    case RS2::ActionOptionsDrawing:
		action = RS_ActionOptionsDrawing::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotOptionsDrawing()));
        break;
		
        // Scripting actions:
        //
	case RS2::ActionScriptOpenIDE:
        action = new QAction(tr("Open IDE"),
                             tr("&Open IDE"),
                             0, mw);
        action->setStatusTip(tr("Opens the integrated development "
		   "environment for scripting"));
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotScriptOpenIDE()));
		break;
		
	case RS2::ActionScriptRun:
        action = new QAction(tr("Run Script.."),
                             tr("&Run Script.."),
                             0, mw);
        action->setStatusTip(tr("Runs a script"));
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotScriptRun()));
		break;

		// CAM actions:
		//
#ifdef RS_CAM
    case RS2::ActionCamExportAuto:
		action = RS_ActionCamExportAuto::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotCamExportAuto()));
        break;
		
    case RS2::ActionCamReorder:
		action = RS_ActionCamReorder::createGUIAction(id, mw);
        connect(action, SIGNAL(activated()),
                obj, SLOT(slotCamReorder()));
        break;
#endif

    default:
        RS_DEBUG->print(RS_Debug::D_WARNING,
        	"No action %d defined", id);
		assert(true);
        break;
    }

    return action;
}


