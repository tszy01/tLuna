#ifndef _TLSIMPLESPLINE_H_
#define _TLSIMPLESPLINE_H_
#include "TLVector3.h"
#include "TLMatrix4x4.h"
#include "TLVector4.h"
#include "TLList.h"
#include <assert.h>

namespace TLunaEngine
{
	// simple spline class from ogre
	template<typename T>
	class SimpleSpline
	{
	public:
        SimpleSpline()
		{
			// Set up matrix
			// Hermite polynomial
			mCoeffs[0][0] = 2;
			mCoeffs[0][1] = -2;
			mCoeffs[0][2] = 1;
			mCoeffs[0][3] = 1;
			mCoeffs[1][0] = -3;
			mCoeffs[1][1] = 3;
			mCoeffs[1][2] = -2;
			mCoeffs[1][3] = -1;
			mCoeffs[2][0] = 0;
			mCoeffs[2][1] = 0;
			mCoeffs[2][2] = 1;
			mCoeffs[2][3] = 0;
			mCoeffs[3][0] = 1;
			mCoeffs[3][1] = 0;
			mCoeffs[3][2] = 0;
			mCoeffs[3][3] = 0;

			mAutoCalc = TTRUE;
		}

		~SimpleSpline(){};

        /** Adds a control point to the end of the spline. */
        inline TVOID addPoint(const Vector3<T>& p)
		{
			mPoints.push_back(p);
			if (mAutoCalc)
			{
				recalcTangents();
			}
		}

        /** Gets the detail of one of the control points of the spline. */
        inline const Vector3<T>& getPoint(TU16 index) const
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
        inline TVOID updatePoint(TU16 index, const Vector3<T>& value)
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
        */
        inline Vector3<T> interpolate(TF32 t) const
		{
			// Currently assumes points are evenly spaced, will cause velocity
			// change where this is not the case
			// TODO: base on arclength?

        
			// Work out which segment this is in
			TF32 fSeg = t * (mPoints.size() - 1);
			TU32 segIdx = (TU32)fSeg;
			// Apportion t 
			t = fSeg - segIdx;

			return interpolate(segIdx, t);
		}

        /** Interpolates a single segment of the spline given a parametric value.
        @param fromIndex The point index to treat as t=0. fromIndex + 1 is deemed to be t=1
        @param t Parametric value
        */
        inline Vector3<T> interpolate(TU32 fromIndex, TF32 t) const
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

			// TF32 interpolation
			// Form a vector of powers of t
			T t2, t3;
			t2 = (T)t * (T)t;
			t3 = t2 * (T)t;
			Vector4<T> powers(t3, t2, t, 1);


			// Algorithm is ret = powers * mCoeffs * Matrix4x4<T>(point1, point2, tangent1, tangent2)
			const Vector3<T>& point1 = mPoints[fromIndex];
			const Vector3<T>& point2 = mPoints[fromIndex+1];
			const Vector3<T>& tan1 = mTangents[fromIndex];
			const Vector3<T>& tan2 = mTangents[fromIndex+1];
			Matrix4x4<T> pt;

			pt[0][0] = point1.x;
			pt[0][1] = point1.y;
			pt[0][2] = point1.z;
			pt[0][3] = 1.0f;
			pt[1][0] = point2.x;
			pt[1][1] = point2.y;
			pt[1][2] = point2.z;
			pt[1][3] = 1.0f;
			pt[2][0] = tan1.x;
			pt[2][1] = tan1.y;
			pt[2][2] = tan1.z;
			pt[2][3] = 1.0f;
			pt[3][0] = tan2.x;
			pt[3][1] = tan2.y;
			pt[3][2] = tan2.z;
			pt[3][3] = 1.0f;

			Vector4<T> ret = powers * mCoeffs * pt;


			return Vector3<T>(ret.x, ret.y, ret.z);
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
			// Catmull-Rom approach
			// 
			// tangent[i] = 0.5 * (point[i+1] - point[i-1])
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

			// Closed or open?
			if (mPoints[0] == mPoints[numPoints-1])
			{
				isClosed = TTRUE;
			}
			else
			{
				isClosed = TFALSE;
			}

			mTangents.resize(numPoints);



			for(i = 0; i < numPoints; ++i)
			{
				if (i ==0)
				{
					// Special case start
					if (isClosed)
					{
						// Use numPoints-2 since numPoints-1 is the last point and == [0]
						mTangents[i] = 0.5 * (mPoints[1] - mPoints[numPoints-2]);
					}
					else
					{
						mTangents[i] = 0.5 * (mPoints[1] - mPoints[0]);
					}
				}
				else if (i == numPoints-1)
				{
					// Special case end
					if (isClosed)
					{
						// Use same tangent as already calculated for [0]
						mTangents[i] = mTangents[0];
					}
					else
					{
						mTangents[i] = 0.5 * (mPoints[i] - mPoints[i-1]);
					}
				}
				else
				{
					mTangents[i] = 0.5 * (mPoints[i+1] - mPoints[i-1]);
				}
            
			}
		}

    protected:
        TBOOL mAutoCalc;
		List<Vector3<T>> mPoints;
		List<Vector3<T>> mTangents;
        /// Matrix of coefficients 
        Matrix4x4<T> mCoeffs;
	};

	// define float simple spline
	typedef SimpleSpline<TF32> SimpleSplineFloat;
	// define double simple spline
	typedef SimpleSpline<TF64> SimpleSplineDouble;
	// define int simple spline
	typedef SimpleSpline<TS32> SimpleSplineInt;
	// define uint simple spline
	typedef SimpleSpline<TU32> SimpleSplineUInt;
}

#endif