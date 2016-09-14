#pragma once
#include "TVector3.h"
namespace TLunaEngine{

	template<typename T>
	class TPlane
	{
	public:
		// Constructors

		TPlane(): Normal(0,1,0) { recalculateD(TVector3<T>(0,0,0)); }
		TPlane(const TVector3<T>& MPoint, const TVector3<T>& Normal) : Normal(Normal) { recalculateD(MPoint); }
		TPlane(T px, T py, T pz, T nx, T ny, T nz) : Normal(nx, ny, nz) { recalculateD(TVector3<T>(px, py, pz)); }
		TPlane(const TVector3<T>& point1, const TVector3<T>& point2, const TVector3<T>& point3)
		{ setPlane(point1, point2, point3); }
		TPlane(const TPlane<T>& other) 
		{
			Normal = other.Normal;
			D = other.D;
		}
		~TPlane()
		{
		}

		// operators
		inline bool operator==(const TPlane<T>& other) const { return (D==other.D && Normal==other.Normal);}
		inline bool operator!=(const TPlane<T>& other) const { return !(D==other.D && Normal==other.Normal);}
		inline TPlane<T>& operator=(const TPlane<T>& other)
		{
			Normal = other.Normal;
			D = other.D;
			return *this;
		}

		// functions
		inline void setPlane(const TVector3<T>& point, const TVector3<T>& nvector)
		{
			Normal = nvector;
			recalculateD(point);
		}

		inline void setPlane(const TVector3<T>& nvect, T d)
		{
			Normal = nvect;
			D = d;
		}

		inline void setPlane(const TVector3<T>& point1, const TVector3<T>& point2, const TVector3<T>& point3)
		{
			// creates the plane from 3 memberpoints
			Normal = (point2 - point1).crossProduct(point3 - point1);
			Normal.normalize();

			recalculateD(point1);
		}

		//! Get an intersection with a 3d line.
		/** \param lineVect Vector of the line to intersect with.
		\param linePoint Point of the line to intersect with.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, false if there was not.
		*/
		inline bool getIntersectionWithLine(const TVector3<T>& linePoint,
				const TVector3<T>& lineVect,
				TVector3<T>& outIntersection) const
		{
			T t2 = Normal.dotProduct(lineVect);

			if (t2 == 0)
				return false;

			T t =- (Normal.dotProduct(linePoint) + D) / t2;
			outIntersection = linePoint + (lineVect * t);
			return true;
		}

		//! Get percentage of line between two points where an intersection with this plane happens.
		/** Only useful if known that there is an intersection.
		\param linePoint1 Point1 of the line to intersect with.
		\param linePoint2 Point2 of the line to intersect with.
		\return Where on a line between two points an intersection with this plane happened.
		For example, 0.5 is returned if the intersection happened exactly in the middle of the two points.
		*/
		inline TF32 getKnownIntersectionWithLine(const TVector3<T>& linePoint1,
			const TVector3<T>& linePoint2) const
		{
			TVector3<T> vect = linePoint2 - linePoint1;
			T t2 = Normal.dotProduct(vect);
			/*return (TF32)(-((TF32)(Normal.dotProduct(linePoint1) + D) / (TF32)t2));*/
			return (TF32)(Normal.dotProduct(linePoint1) + D) / (TF32)t2 * -1.0f;
		}

		//! Get an intersection with a 3d line, limited between two 3d points.
		/** \param linePoint1 Point 1 of the line.
		\param linePoint2 Point 2 of the line.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, false if there was not.
		*/
		inline bool getIntersectionWithLimitedLine(
				const TVector3<T>& linePoint1,
				const TVector3<T>& linePoint2,
				TVector3<T>& outIntersection) const
		{
			return (getIntersectionWithLine(linePoint1, linePoint2 - linePoint1, outIntersection) &&
					outIntersection.isBetweenPoints(linePoint1, linePoint2));
		}

		//! Classifies the relation of a point to this plane.
		/** \param point Point to classify its relation.
		\return ISREL3D_FRONT if the point is in front of the plane,
		ISREL3D_BACK if the point is behind of the plane, and
		ISREL3D_PLANAR if the point is within the plane. */
		inline EIntersectionRelation3D classifyPointRelation(const TVector3<T>& point) const
		{
			const T d = Normal.dotProduct(point) + D;

			if (d < -ROUNDING_ERROR_32)
				return ISREL3D_BACK;

			if (d > ROUNDING_ERROR_32)
				return ISREL3D_FRONT;

			return ISREL3D_PLANAR;
		}

		inline EIntersectionRelation3D classifyPointRelation(const TVector3<T>& center, const TVector3<T>& halfSize) const
		{
			const T d = Normal.dotProduct(center) + D;

			const T maxAbsDist = Normal.absDotProduct(halfSize);

			if (d < -maxAbsDist)
				return ISREL3D_BACK;

			if (d > maxAbsDist)
				return ISREL3D_FRONT;

			return ISREL3D_SPANNING;
		}

		//! Recalculates the distance from origin by applying a new member point to the plane.
		inline void recalculateD(const TVector3<T>& MPoint)
		{
			D = - MPoint.dotProduct(Normal);
		}

		//! Gets a member point of the plane.
		// return N*(-D)
		inline TVector3<T> getMemberPoint() const
		{
			return Normal * -D;
		}

		//! Tests if there is an intersection with the other plane
		/** \return True if there is a intersection. */
		inline bool existsIntersection(const TPlane<T>& other) const
		{
			TVector3<T> cross = other.Normal.crossProduct(Normal);
			return cross.getLength() > ROUNDING_ERROR_32;
		}

		//! Intersects this plane with another.
		/** \param other Other plane to intersect with.
		\param outLinePoint Base point of intersection line.
		\param outLineVect Vector of intersection.
		\return True if there is a intersection, false if not. */
		inline bool getIntersectionWithPlane(const TPlane<T>& other,
				TVector3<T>& outLinePoint,
				TVector3<T>& outLineVect) const
		{
			const T fn00 = Normal.getLength();
			const T fn01 = Normal.dotProduct(other.Normal);
			const T fn11 = other.Normal.getLength();
			const T det = fn00*fn11 - fn01*fn01;

			if ((TF64)(abs_(det)) < ROUNDING_ERROR_64 )
				return false;

			const TF32 invdet = 1.0f / (TF32)det;
			const TF32 fc0 = (TF32)(fn11*-D + fn01*other.D) * invdet;
			const TF32 fc1 = (TF32)(fn00*-other.D + fn01*D) * invdet;

			outLineVect = Normal.crossProduct(other.Normal);
			outLinePoint = Normal*fc0 + other.Normal*fc1;
			return true;
		}

		//! Get the intersection point with two other planes if there is one.
		inline bool getIntersectionWithPlanes(const TPlane<T>& o1,
				const TPlane<T>& o2, TVector3<T>& outPoint) const
		{
			TVector3<T> linePoint, lineVect;
			if (getIntersectionWithPlane(o1, linePoint, lineVect))
				return o2.getIntersectionWithLine(linePoint, lineVect, outPoint);

			return false;
		}

		//! Test if the triangle would be front or backfacing from any point.
		/** Thus, this method assumes a camera position from
		which the triangle is definitely visible when looking into
		the given direction.
		Note that this only works if the normal is Normalized.
		Do not use this method with points as it will give wrong results!
		\param lookDirection: Look direction.
		\return True if the plane is front facing and
		false if it is backfacing. */
		inline bool isFrontFacing(const TVector3<T>& lookDirection) const
		{
			const T d = Normal.dotProduct(lookDirection);
			return F32_LOWER_EQUAL_0 ( d );
		}

		//! Get the distance to a point.
		/** Note that this only works if the normal is normalized. */
		inline T getDistanceTo(const TVector3<T>& point) const
		{
			return point.dotProduct(Normal) + D;
		}

		//! Normal vector of the plane.
		TVector3<T> Normal;
		//! Distance from origin.
		T D;
	};

	// define float plane
	typedef TPlane<TF32> TPlaneFloat;
	// define double plane
	typedef TPlane<TF64> TPlaneDouble;
	// define int plane
	typedef TPlane<TS32> TPlaneInt;
	// define uint plane
	typedef TPlane<TU32> TPlaneUInt;
}