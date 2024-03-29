/****************************************************************************
** $Id: rs_spline.cpp 2367 2005-04-04 16:57:36Z andrew $
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


#include "rs_spline.h"

#include "rs_debug.h"
#include "rs_graphicview.h"
#include "rs_painter.h"
#include "rs_graphic.h"


/**
 * Constructor.
 */
RS_Spline::RS_Spline(RS_EntityContainer* parent,
                     const RS_SplineData& d)
        :RS_EntityContainer(parent), data(d) {
    calculateBorders();
}



/**
 * Destructor.
 */
RS_Spline::~RS_Spline() {}




RS_Entity* RS_Spline::clone() {
    RS_Spline* l = new RS_Spline(*this);
	l->entities.setAutoDelete(entities.autoDelete());
    l->initId();
    l->detach();
    return l;
}



void RS_Spline::calculateBorders() {
    /*minV = RS_Vector::minimum(data.startpoint, data.endpoint);
    maxV = RS_Vector::maximum(data.startpoint, data.endpoint);

    RS_ValueList<RS_Vector>::iterator it;
    for (it = data.controlPoints.begin(); 
    it!=data.controlPoints.end(); ++it) {

    minV = RS_Vector::minimum(*it, minV);
    maxV = RS_Vector::maximum(*it, maxV);
}
    */
}



RS_VectorSolutions RS_Spline::getRefPoints() {

    RS_VectorSolutions ret(data.controlPoints.count());

    int i=0;
    RS_ValueList<RS_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it, ++i) {

        ret.set(i, (*it));
    }

    return ret;
}

RS_Vector RS_Spline::getNearestRef(const RS_Vector& coord,
                                   double* dist) {

    //return getRefPoints().getClosest(coord, dist);
    return RS_Entity::getNearestRef(coord, dist);
}

RS_Vector RS_Spline::getNearestSelectedRef(const RS_Vector& coord,
        double* dist) {

    //return getRefPoints().getClosest(coord, dist);
    return RS_Entity::getNearestSelectedRef(coord, dist);
}


/**
 * Updates the internal polygon of this spline. Called when the 
 * spline or it's data, position, .. changes.
 */
void RS_Spline::update() {

    RS_DEBUG->print("RS_Spline::update");

    clear();

    if (isUndone()) {
        return;
    }

    if (data.degree<1 || data.degree>3) {
        RS_DEBUG->print("RS_Spline::update: invalid degree: %d", data.degree);
        return;
    }

    if (data.controlPoints.count()<(uint)data.degree+1) {
        RS_DEBUG->print("RS_Spline::update: not enough control points");
        return;
    }

    resetBorders();

    RS_ValueList<RS_Vector> tControlPoints = data.controlPoints;

    if (data.closed) {
        for (int i=0; i<data.degree; ++i) {
            tControlPoints.append(data.controlPoints[i]);
        }
    }

    int i;
    int npts = tControlPoints.count();
    // order:
    int k = data.degree+1;
    // resolution:
    int p1 = getGraphicVariableInt("$SPLINESEGS", 8) * npts;

    double* b = new double[npts*3+1];
    double* h = new double[npts+1];
    double* p = new double[p1*3+1];

    RS_ValueList<RS_Vector>::iterator it;
    i = 1;
    for (it = tControlPoints.begin(); it!=tControlPoints.end(); ++it) {
        b[i] = (*it).x;
        b[i+1] = (*it).y;
        b[i+2] = 0.0;

        RS_DEBUG->print("RS_Spline::update: b[%d]: %f/%f", i, b[i], b[i+1]);
        i+=3;
    }

    // set all homogeneous weighting factors to 1.0
    for (i=1; i <= npts; i++) {
        h[i] = 1.0;
    }

    for (i = 1; i <= 3*p1; i++) {
        p[i] = 0.0;
    }

    if (data.closed) {
        rbsplinu(npts,k,p1,b,h,p);
    } else {
        rbspline(npts,k,p1,b,h,p);
    }

    RS_Vector prev(false);
    for (i = 1; i <= 3*p1; i=i+3) {
        if (prev.valid) {
            RS_Line* line = new RS_Line(this,
                                        RS_LineData(prev, RS_Vector(p[i], p[i+1])));
            line->setLayer(NULL);
            line->setPen(RS_Pen(RS2::FlagInvalid));
            addEntity(line);
        }
        prev = RS_Vector(p[i], p[i+1]);

        minV = RS_Vector::minimum(prev, minV);
        maxV = RS_Vector::maximum(prev, maxV);
    }

    delete[] b;
    delete[] h;
    delete[] p;
}



RS_Vector RS_Spline::getNearestEndpoint(const RS_Vector& coord,
                                        double* dist) {
    double minDist = RS_MAXDOUBLE;
    double d;
    RS_Vector ret(false);

    for (uint i=0; i<data.controlPoints.count(); i++) {
        d = data.controlPoints[i].distanceTo(coord);

        if (d<minDist) {
            minDist = d;
            ret = data.controlPoints[i];
        }
    }
    if (dist!=NULL) {
        *dist = minDist;
    }
	return ret;
}



/*
// The default implementation of RS_EntityContainer is inaccurate but
//   has to do for now..
RS_Vector RS_Spline::getNearestPointOnEntity(const RS_Vector& coord,
        bool onEntity, double* dist, RS_Entity** entity) {
}
*/



RS_Vector RS_Spline::getNearestCenter(const RS_Vector& /*coord*/,
                                      double* dist) {

    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }

    return RS_Vector(false);
}



RS_Vector RS_Spline::getNearestMiddle(const RS_Vector& /*coord*/,
                                      double* dist) {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }

    return RS_Vector(false);
}



RS_Vector RS_Spline::getNearestDist(double /*distance*/,
                                    const RS_Vector& /*coord*/,
                                    double* dist) {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }

    return RS_Vector(false);
}



void RS_Spline::move(RS_Vector offset) {
    RS_ValueList<RS_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it) {

        (*it).move(offset);
    }

    update();
}



void RS_Spline::rotate(RS_Vector center, double angle) {
    RS_ValueList<RS_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it) {

        (*it).rotate(center, angle);
    }

    update();
}



void RS_Spline::scale(RS_Vector center, RS_Vector factor) {
    RS_ValueList<RS_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it) {

        (*it).scale(center, factor);
    }

    update();
}



void RS_Spline::mirror(RS_Vector axisPoint1, RS_Vector axisPoint2) {
    RS_ValueList<RS_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it) {

        (*it).mirror(axisPoint1, axisPoint2);
    }

    update();
}



void RS_Spline::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
    RS_ValueList<RS_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it) {

        if (ref.distanceTo(*it)<1.0e-4) {
            (*it).move(offset);
        }
    }

    update();
}




void RS_Spline::draw(RS_Painter* painter, RS_GraphicView* view,
                     double /*patternOffset*/) {

    if (painter==NULL || view==NULL) {
        return;
    }

    RS_Entity* e = firstEntity(RS2::ResolveNone);
    double offset = 0.0;

    if (e!=NULL) {
        view->drawEntity(e);
        offset+=e->getLength();
        //RS_DEBUG->print("offset: %f\nlength was: %f", offset, e->getLength());
    }

    for (RS_Entity* e=nextEntity(RS2::ResolveNone);
            e!=NULL;
            e = nextEntity(RS2::ResolveNone)) {

        view->drawEntityPlain(e, -offset);
        offset+=e->getLength();
        //RS_DEBUG->print("offset: %f\nlength was: %f", offset, e->getLength());
    }
}



/**
 * Todo: draw the spline, user patterns.
 */
/*
void RS_Spline::draw(RS_Painter* painter, RS_GraphicView* view) {
   if (painter==NULL || view==NULL) {
       return;
   }
 
   / *
      if (data.controlPoints.count()>0) {
          RS_Vector prev(false);
          RS_ValueList<RS_Vector>::iterator it;
          for (it = data.controlPoints.begin(); it!=data.controlPoints.end(); ++it) {
              if (prev.valid) {
                  painter->drawLine(view->toGui(prev),
                                    view->toGui(*it));
              }
              prev = (*it);
          }
      }
   * /
 
   int i;
   int npts = data.controlPoints.count();
   // order:
   int k = 4;
   // resolution:
   int p1 = 100;
 
   double* b = new double[npts*3+1];
   double* h = new double[npts+1];
   double* p = new double[p1*3+1];
 
   RS_ValueList<RS_Vector>::iterator it;
   i = 1;
   for (it = data.controlPoints.begin(); it!=data.controlPoints.end(); ++it) {
       b[i] = (*it).x;
       b[i+1] = (*it).y;
       b[i+2] = 0.0;
 
	RS_DEBUG->print("RS_Spline::draw: b[%d]: %f/%f", i, b[i], b[i+1]);
	i+=3;
   }
 
   // set all homogeneous weighting factors to 1.0
   for (i=1; i <= npts; i++) {
       h[i] = 1.0;
   }
 
   //
   for (i = 1; i <= 3*p1; i++) {
       p[i] = 0.0;
   }
 
   rbspline(npts,k,p1,b,h,p);
 
   RS_Vector prev(false);
   for (i = 1; i <= 3*p1; i=i+3) {
       if (prev.valid) {
           painter->drawLine(view->toGui(prev),
                             view->toGui(RS_Vector(p[i], p[i+1])));
       }
       prev = RS_Vector(p[i], p[i+1]);
   }
}
*/



/**
 * @return The reference points of the spline.
 */
RS_ValueList<RS_Vector> RS_Spline::getControlPoints() {
    return data.controlPoints;
}



/**
 * Appends the given point to the control points.
 */
void RS_Spline::addControlPoint(const RS_Vector& v) {
    data.controlPoints.append(v);
}



/**
 * Removes the control point that was last added.
 */
void RS_Spline::removeLastControlPoint() {
    data.controlPoints.pop_back();
}


/**
 * Generates B-Spline open knot vector with multiplicity 
 * equal to the order at the ends.
 */
void RS_Spline::knot(int num, int order, int knotVector[]) {
    knotVector[1] = 0;
    for (int i = 2; i <= num + order; i++) {
        if ( (i > order) && (i < num + 2) ) {
            knotVector[i] = knotVector[i-1] + 1;
        } else {
            knotVector[i] = knotVector[i-1];
        }
    }
}



/**
 * Generates rational B-spline basis functions for an open knot vector.
 */
void RS_Spline::rbasis(int c, double t, int npts,
                       int x[], double h[], double r[]) {

    int nplusc;
    int i,k;
    double d,e;
    double sum;
    //double temp[36];

    nplusc = npts + c;

    double* temp = new double[nplusc+1];

    // calculate the first order nonrational basis functions n[i]
    for (i = 1; i<= nplusc-1; i++) {
        if (( t >= x[i]) && (t < x[i+1]))
            temp[i] = 1;
        else
            temp[i] = 0;
    }

    /* calculate the higher order nonrational basis functions */

    for (k = 2; k <= c; k++) {
        for (i = 1; i <= nplusc-k; i++) {
            // if the lower order basis function is zero skip the calculation
            if (temp[i] != 0)
                d = ((t-x[i])*temp[i])/(x[i+k-1]-x[i]);
            else
                d = 0;
            // if the lower order basis function is zero skip the calculation
            if (temp[i+1] != 0)
                e = ((x[i+k]-t)*temp[i+1])/(x[i+k]-x[i+1]);
            else
                e = 0;

            temp[i] = d + e;
        }
    }

    // pick up last point
    if (t == (double)x[nplusc]) {
        temp[npts] = 1;
    }

    // calculate sum for denominator of rational basis functions
    sum = 0.;
    for (i = 1; i <= npts; i++) {
        sum = sum + temp[i]*h[i];
    }

    // form rational basis functions and put in r vector
    for (i = 1; i <= npts; i++) {
        if (sum != 0) {
            r[i] = (temp[i]*h[i])/(sum);
        } else
            r[i] = 0;
    }

    delete[] temp;
}


/**
 * Generates a rational B-spline curve using a uniform open knot vector.
 */
void RS_Spline::rbspline(int npts, int k, int p1,
                         double b[], double h[], double p[]) {

    int i,j,icount,jcount;
    int i1;
    //int x[30]; /* allows for 20 data points with basis function of order 5 */
    int nplusc;

    double step;
    double t;
    //double nbasis[20];
    double temp;

    nplusc = npts + k;

    int* x = new int[nplusc+1];
    double* nbasis = new double[npts+1];

    // zero and redimension the knot vector and the basis array

    for(i = 0; i <= npts; i++) {
        nbasis[i] = 0.0;
    }

    for(i = 0; i <= nplusc; i++) {
        x[i] = 0;
    }

    // generate the uniform open knot vector
    knot(npts,k,x);

    icount = 0;

    // calculate the points on the rational B-spline curve
    t = 0;
    step = ((double)x[nplusc])/((double)(p1-1));

    for (i1 = 1; i1<= p1; i1++) {

        if ((double)x[nplusc] - t < 5e-6) {
            t = (double)x[nplusc];
        }

        // generate the basis function for this value of t
        rbasis(k,t,npts,x,h,nbasis);

        // generate a point on the curve
        for (j = 1; j <= 3; j++) {
            jcount = j;
            p[icount+j] = 0.;

            // Do local matrix multiplication
            for (i = 1; i <= npts; i++) {
                temp = nbasis[i]*b[jcount];
                p[icount + j] = p[icount + j] + temp;
                jcount = jcount + 3;
            }
        }
        icount = icount + 3;
        t = t + step;
    }

    delete[] x;
    delete[] nbasis;
}


void RS_Spline::knotu(int num, int order, int knotVector[]) {
    int nplusc,nplus2,i;

    nplusc = num + order;
    nplus2 = num + 2;

    knotVector[1] = 0;
    for (i = 2; i <= nplusc; i++) {
        knotVector[i] = i-1;
    }
}



void RS_Spline::rbsplinu(int npts, int k, int p1,
                         double b[], double h[], double p[]) {

    int i,j,icount,jcount;
    int i1;
    //int x[30];		/* allows for 20 data points with basis function of order 5 */
    int nplusc;

    double step;
    double t;
    //double nbasis[20];
    double temp;


    nplusc = npts + k;

    int* x = new int[nplusc+1];
    double* nbasis = new double[npts+1];

    /*  zero and redimension the knot vector and the basis array */

    for(i = 0; i <= npts; i++) {
        nbasis[i] = 0.0;
    }

    for(i = 0; i <= nplusc; i++) {
        x[i] = 0;
    }

    /* generate the uniform periodic knot vector */

    knotu(npts,k,x);

    /*
    	printf("The knot vector is ");
    	for (i = 1; i <= nplusc; i++){
    		printf(" %d ", x[i]);
    	}
    	printf("\n");
     
    	printf("The usable parameter range is ");
    	for (i = k; i <= npts+1; i++){
    		printf(" %d ", x[i]);
    	}
    	printf("\n");
    */

    icount = 0;

    /*    calculate the points on the rational B-spline curve */

    t = k-1;
    step = ((double)((npts)-(k-1)))/((double)(p1-1));

    for (i1 = 1; i1<= p1; i1++) {

        if ((double)x[nplusc] - t < 5e-6) {
            t = (double)x[nplusc];
        }

        rbasis(k,t,npts,x,h,nbasis);      /* generate the basis function for this value of t */
        /*
        		printf("t = %f \n",t);
        		printf("nbasis = ");
        		for (i = 1; i <= npts; i++){
        			printf("%f  ",nbasis[i]);
        		}
        		printf("\n");
        */
        for (j = 1; j <= 3; j++) {      /* generate a point on the curve */
            jcount = j;
            p[icount+j] = 0.;

            for (i = 1; i <= npts; i++) { /* Do local matrix multiplication */
                temp = nbasis[i]*b[jcount];
                p[icount + j] = p[icount + j] + temp;
                /*
                				printf("jcount,nbasis,b,nbasis*b,p = %d %f %f %f %f\n",jcount,nbasis[i],b[jcount],temp,p[icount+j]);
                */
                jcount = jcount + 3;
            }
        }
        /*
        		printf("icount, p %d %f %f %f \n",icount,p[icount+1],p[icount+2],p[icount+3]);
        */
        icount = icount + 3;
        t = t + step;
    }

    delete[] x;
    delete[] nbasis;
}


/**
 * Dumps the spline's data to stdout.
 */
std::ostream& operator << (std::ostream& os, const RS_Spline& l) {
    os << " Spline: " << l.getData() << "\n";
    return os;
}


