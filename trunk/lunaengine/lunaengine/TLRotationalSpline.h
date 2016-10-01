#ifndef _TLROTATIONALSPLINE_H_
#define _TLROTATIONALSPLINE_H_
#include "TLQuaternion.h"
#include "TLList.h"
#include <assert.h>

namespace TLunaEngine
{
	// rotational spline from ogre
	template<typename T>
	class RotationalSpline
	{
	public:
        RotationalSpline()
		{
			mAutoCalc = TTRUE;
		}
        ~RotationalSpline()
		{
		}

        /** Adds a control point to the end of the spline. */
        inline TVOID addPoint(const Quaternion<T>& p)
		{
			mPoints.push_back(p);
			if (mAutoCalc)
			{
				recalcTangents();
			}
		}

        /** Gets the detail of one of the control points of the spline. */
        inline const Quaternion<T>& getPoint(TU16 index) const
		{
			assert (index < mPoints.size() && "Point index is out of bounds!!");
			return mPoints[index];
		}

        /** Gets the number of control points in the spline. */
        inline TU16 getNumPoints(TVOID) const
		{
			return (TU16)mPoints.size();
		}

        /** Clears all the points in the spline. */
        inline TVOID clear(TVOID)
		{
			mPoints.clear();
			mTangents.clear();
		}

        /** Updates a single point in the spline. 
        @remarks
            This point must already exist in the spline.
        */
        inline TVOID updatePoint(TU16 index, const Quaternion<T>& value)
		{
			assert (index < mPoints.size() && "Point index is out of bounds!!");
			mPoints[index] = value;
			if (mAutoCalc)
			{
				recalcTangents();
			}
		}

        /** Returns an interpolated point based on a parametric value over the whole series.
        @remarks
            Given a t value between 0 and 1 representing the parametric distance along the
            whole length of the spline, this method returns an interpolated point.
        @param t Parametric value.
		@param useShortestPath Defines if rotation should take the shortest possible path
        */
        inline Quaternion<T> interpolate(TF32 t, TBOOL useShortestPath=TTRUE)
		{
			// Work out which segment this is in
			TF32 fSeg = t * (mPoints.size() - 1);
			TU32 segIdx = (TU32)fSeg;
			// Apportion t 
			t = fSeg - segIdx;
			return interpolate(segIdx, t, useShortestPath);
		}

        /** Interpolates a single segment of the spline given a parametric value.
        @param fromIndex The point index to treat as t=0. fromIndex + 1 is deemed to be t=1
        @param t Parametric value
		@param useShortestPath Defines if rotation should take the shortest possible path
        */
        inline Quaternion<T> interpolate(TU32 fromIndex, TF32 t, TBOOL useShortestPath=TTRUE)
		{
			// Bounds check
			assert (fromIndex < mPoints.size() &&
				"fromIndex out of bounds");

			if ((fromIndex + 1) == mPoints.size())
			{
				// Duff request, cannot blend to nothing
				// Just return source
				return mPoints[fromIndex];

			}
			// Fast special cases
			if (t == 0.0f)
			{
				return mPoints[fromIndex];
			}
			else if(t == 1.0f)
			{
				return mPoints[fromIndex + 1];
			}

			// Real interpolation
			// Use squad using tangents we've already set up
			Quaternion<T> &p = mPoints[fromIndex];
			Quaternion<T> &q = mPoints[fromIndex+1];
			Quaternion<T> &a = mTangents[fromIndex];
			Quaternion<T> &b = mTangents[fromIndex+1];

			// NB interpolate to nearest rotation
			Quaternion<T> result;
			result.squad(t, p, a, b, q/*, useShortestPath*/);
			return result;
		}

        /** Tells the spline whether it should automatically calculate tangents on demand
            as points are added.
        @remarks
            The spline calculates tangents at each point automatically based on the input points.
            Normally it does this every time a point changes. However, if you have a lot of points
            to add in one go, you probably don't want to incur this overhead and would prefer to 
            defer the calculation until you are finished setting all the points. You can do this
            by calling this method with a parameter of 'TFALSE'. Just remember to manually call 
            the recalcTangents method when you are done.
        @param autoCalc If TTRUE, tangents are calculated for you whenever a point changes. If TFALSE, 
            you must call reclacTangents to recalculate them when it best suits.
        */
        inline TVOID setAutoCalculate(TBOOL autoCalc)
		{
			mAutoCalc = autoCalc;
		}

        /** Recalculates the tangents associated with this spline. 
        @remarks
            If you tell the spline not to update on demand by calling setAutoCalculate(TFALSE)
            then you must call this after completing your updates to the spline points.
        */
        inline TVOID recalcTangents(TVOID)
		{
			// ShoeMake (1987) approach
			// Just like Catmull-Rom really, just more gnarly
			// And no, I don't understand how to derive this!
			//
			// let p = point[i], pInv = p.Inverse
			// tangent[i] = p * exp( -0.25 * ( log(pInv * point[i+1]) + log(pInv * point[i-1]) ) )
			//
			// Assume endpoint tangents are parallel with line with neighbour

			TU32 i, numPoints;
			TBOOL isClosed;

			numPoints = (TU32)mPoints.size();

			if (numPoints < 2)
			{
				// Can't do anything yet
				return;
			}

			mTangents.resize(numPoints);

			if (mPoints[0] == mPoints[numPoints-1])
			{
				isClosed = TTRUE;
			}
			else
			{
				isClosed = TFALSE;
			}

			Quaternion<T> invp, part1, part2, preExp;
			for(i = 0; i < numPoints; ++i)
			{
				Quaternion<T> &p = mPoints[i];
				invp = p.Inverse();

				if (i ==0)
				{
					// special case start
					part1 = (invp * mPoints[i+1]).log();
					if (isClosed)
					{
						// Use numPoints-2 since numPoints-1 == end == start == this one
						part2 = (invp * mPoints[numPoints-2]).log();
					}
					else
					{
						part2 = (invp * p).log();
					}
				}
				else if (i == numPoints-1)
				{
					// special case end
					if (isClosed)
					{
						// Wrap to [1] (not [0], this is the same as end == this one)
						part1 = (invp * mPoints[1]).log();
					}
					else
					{
						part1 = (invp * p).log();
					}
					part2 = (invp * mPoints[i-1]).log();
				}
				else
				{
					part1 = (invp * mPoints[i+1]).log();
					part2 = (invp * mPoints[i-1]).log();
				}

				preExp = -0.25 * (part1 + part2);
				mTangents[i] = p * preExp.exp();
            
			}
		}

    protected:
        TBOOL mAutoCalc;
		List<Quaternion<T>> mPoints;
		List<Quaternion<T>> mTangents;
	};

	// define float rotational spline
	typedef RotationalSpline<TF32> RotationalSplineFloat;
	// define double rotational spline
	typedef RotationalSpline<TF64> RotationalSplineDouble;
	// define int rotational spline
	typedef RotationalSpline<TS32> RotationalSplineInt;
	// define uint rotational spline
	typedef RotationalSpline<TU32> RotationalSplineUInt;
}

#endif