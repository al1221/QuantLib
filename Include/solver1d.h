
/*
 * Copyright (C) 2000
 * Ferdinando Ametrano, Luigi Ballabio, Adolfo Benin, Marco Marchioro
 * 
 * This file is part of QuantLib.
 * QuantLib is a C++ open source library for financial quantitative
 * analysts and developers --- http://quantlib.sourceforge.net/
 *
 * QuantLib is free software and you are allowed to use, copy, modify, merge,
 * publish, distribute, and/or sell copies of it under the conditions stated 
 * in the QuantLib License.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You should have received a copy of the license along with this file;
 * if not, contact ferdinando@ametrano.net
 *
 * QuantLib license is also available at http://quantlib.sourceforge.net/LICENSE.TXT
*/

#ifndef quantlib_solver1d_h
#define quantlib_solver1d_h

#include "qldefines.h"
#include "null.h"
#include "qlerrors.h"
#include "dataformatters.h"
#include <cmath>
#include <limits>

namespace QuantLib {

	#define MAX_FUNCTION_EVALUATIONS 100
	
	class ObjectiveFunction {
	  public:
		virtual ~ObjectiveFunction() {}
		virtual double value(double x) const = 0;
		virtual double derivative(double x) const { return Null<double>(); }
	};

	class Solver1D {
	  public:
		// constructor
		Solver1D()
		: maxEvaluations(MAX_FUNCTION_EVALUATIONS), lowBoundEnforced(false), hiBoundEnforced(false) {}
		// destructor
		virtual ~Solver1D() {}
		// methods
		virtual double solve(const ObjectiveFunction& f, double xAccuracy, double guess, double step) const;
		virtual double solve(const ObjectiveFunction& f, double xAccuracy, double guess, double xMin, double xMax) const;
		void setMaxEvaluations(int evaluations);
		void setLowBound(double lowBound) {theLowBound = lowBound; lowBoundEnforced = true; }
		void setHiBound(double hiBound) {theHiBound = hiBound;  hiBoundEnforced = true; }
	  protected:
		// the following method assumes:
		//  a) a valid bracket [xMin, xMax];
		//  b) function already evaluated at xMin, xMax (fxMin, fxMax);
		//  c) variable root initialized;
		virtual double _solve(const ObjectiveFunction& f, double xAccuracy) const = 0;
		double enforceBounds(double x) const;
		int maxEvaluations;
		mutable int evaluationNumber;
		mutable double root, xMin, xMax, fxMin, fxMax;
		double theLowBound, theHiBound;
		bool lowBoundEnforced, hiBoundEnforced;
	};
	
	
	// inline definitions
	
	inline void Solver1D::setMaxEvaluations(int evaluations) {
		Require(evaluations > 0, "negative or null evaluations number");
		maxEvaluations = evaluations;
	}
	
	inline double Solver1D::enforceBounds(double x) const {
		if (lowBoundEnforced && x < theLowBound)
		return theLowBound;
	
		if (hiBoundEnforced && x > theHiBound)
		return theHiBound;
	
		return x;
	}

}


#endif
