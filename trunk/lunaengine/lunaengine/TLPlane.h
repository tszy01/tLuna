#ifndef _TLPLANE_H_
#define _TLPLANE_H_
#include "TLVector3.h"
namespace TLunaEngine{

	template<typename T>
	class Plane
	{
	public:
		// Constructors

		Plane(): Normal(0,1,0) { recalculateD(Vector3<T>(0,0,0)); }
		Plane(const Vector3<T>& MPoint, const Vector3<T>& Normal) : Normal(Normal) { recalculateD(MPoint); }
		Plane(T px, T py, T pz, T nx, T ny, T nz) : Normal(nx, ny, nz) { recalculateD(Vector3<T>(px, py, pz)); }
		Plane(const Vector3<T>& point1, const Vector3<T>& point2, const Vector3<T>& point3)
		{ setPlane(point1, point2, point3); }
		Plane(const Plane<T>& other) 
		{
			Normal = other.Normal;
			D = other.D;
		}
		~Plane()
		{
		}

		// operators
		inline TBOOL operator==(const Plane<T>& other) const { return (D==other.D && Normal==other.Normal);}
		inline TBOOL operator!=(const Plane<T>& other) const { return !(D==other.D && Normal==other.Normal);}
		inline Plane<T>& operator=(const Plane<T>& other)
		{
			Normal = other.Normal;
			D = other.D;
			return *this;
		}

		// functions
		inline TVOID setPlane(const Vector3<T>& point, const Vector3<T>& nvector)
		{
			Normal = nvector;
			recalculateD(point);
		}

		inline TVOID setPlane(const Vector3<T>& nvect, T d)
		{
			Normal = nvect;
			D = d;
		}

		inline TVOID setPlane(const Vector3<T>& point1, const Vector3<T>& point2, const Vector3<T>& point3)
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
		\return True if there was an intersection, TFALSE if there was not.
		*/
		inline TBOOL getIntersectionWithLine(const Vector3<T>& linePoint,
				const Vector3<T>& lineVect,
				Vector3<T>& outIntersection, TF32& outTValue) const
		{
			T t2 = Normal.dotProduct(lineVect);

			if (t2 == 0)
				return TFALSE;

			TF32 t =(TF32)(- (Normal.dotProduct(linePoint) + D) / t2);
			outIntersection = linePoint + (lineVect * t);
			outTValue = t;
			return TTRUE;
		}

		//! Get percentage of line between two points where an intersection with this plane happens.
		/** Only useful if known that there is an intersection.
		\param linePoint1 Point1 of the line to intersect with.
		\param linePoint2 Point2 of the line to intersect with.
		\return Where on a line between two points an intersection with this plane happened.
		For example, 0.5 is returned if the intersection happened exactly in the middle of the two points.
		*/
		inline TF32 getKnownIntersectionWithLine(const Vector3<T>& linePoint1,
			const Vector3<T>& linePoint2) const
		{
			Vector3<T> vect = linePoint2 - linePoint1;
			T t2 = Normal.dotProduct(vect);
			/*return (TF32)(-((TF32)(Normal.dotProduct(linePoint1) + D) / (TF32)t2));*/
			return (TF32)(Normal.dotProduct(linePoint1) + D) / (TF32)t2 * -1.0f;
		}

		//! Get an intersection with a 3d line, limited between two 3d points.
		/** \param linePoint1 Point 1 of the line.
		\param linePoint2 Point 2 of the line.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, TFALSE if there was not.
		*/
		inline TBOOL getIntersectionWithLimitedLine(
				const Vector3<T>& linePoint1,
				const Vector3<T>& linePoint2,
				Vector3<T>& outIntersection, TF32& outTValue) const
		{
			return (getIntersectionWithLine(linePoint1, linePoint2 - linePoint1, outIntersection, outTValue) &&
					outIntersection.isBetweenPoints(linePoint1, linePoint2));
		}

		//! Classifies the relation of a point to this plane.
		/** \param point Point to classify its relation.
		\return ISREL3D_FRONT if the point is in front of the plane,
		ISREL3D_BACK if the point is behind of the plane, and
		ISREL3D_PLANAR if the point is within the plane. */
		inline EIntersectionRelation3D classifyPointRelation(const Vector3<T>& point) const
		{
			const T d = Normal.dotProduct(point) + D;

			if (d < -ROUNDING_ERROR_32)
				return ISREL3D_BACK;

			if (d > ROUNDING_ERROR_32)
				return ISREL3D_FRONT;

			return ISREL3D_PLANAR;
		}

		inline EIntersectionRelation3D classifyPointRelation(const Vector3<T>& center, const Vector3<T>& halfSize) const
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
		inline TVOID recalculateD(const Vector3<T>& MPoint)
		{
			D = - MPoint.dotProduct(Normal);
		}

		//! Gets a member point of the plane.
		// return N*(-D)
		inline Vector3<T> getMemberPoint() const
		{
			return Normal * -D;
		}

		//! Tests if there is an intersection with the other plane
		/** \return True if there is a intersection. */
		inline TBOOL existsIntersection(const Plane<T>& other) const
		{
			Vector3<T> cross = other.Normal.crossProduct(Normal);
			return cross.getLength() > ROUNDING_ERROR_32;
		}

		//! Intersects this plane with another.
		/** \param other Other plane to intersect with.
		\param outLinePoint Base point of intersection line.
		\param outLineVect Vector of intersection.
		\return True if there is a intersection, TFALSE if not. */
		inline TBOOL getIntersectionWithPlane(const Plane<T>& other,
				Vector3<T>& outLinePoint,
				Vector3<T>& outLineVect) const
		{
			const T fn00 = Normal.getLength();
			const T fn01 = Normal.dotProduct(other.Normal);
			const T fn11 = other.Normal.getLength();
			const T det = fn00*fn11 - fn01*fn01;

			if ((TF64)(abs_(det)) < ROUNDING_ERROR_64 )
				return TFALSE;

			const TF32 invdet = 1.0f / (TF32)det;
			const TF32 fc0 = (TF32)(fn11*-D + fn01*other.D) * invdet;
			const TF32 fc1 = (TF32)(fn00*-other.D + fn01*D) * invdet;

			outLineVect = Normal.crossProduct(other.Normal);
			outLinePoint = Normal*fc0 + other.Normal*fc1;
			return TTRUE;
		}

		//! Get the intersection point with two other planes if there is one.
		inline TBOOL getIntersectionWithPlanes(const Plane<T>& o1,
				const Plane<T>& o2, Vector3<T>& outPoint) const
		{
			Vector3<T> linePoint, lineVect;
			TF32 tmpTValue;
			if (getIntersectionWithPlane(o1, linePoint, lineVect))
				return o2.getIntersectionWithLine(linePoint, lineVect, outPoint, tmpTValue);

			return TFALSE;
		}

		//! Test if the triangle would be front or backfacing from any point.
		/** Thus, this method assumes a camera position from
		which the triangle is definitely visible when looking into
		the given direction.
		Note that this only works if the normal is Normalized.
		Do not use this method with points as it will give wrong results!
		\param lookDirection: Look direction.
		\return True if the plane is front facing and
		TFALSE if it is backfacing. */
		inline TBOOL isFrontFacing(const Vector3<T>& lookDirection) const
		{
			const T d = Normal.dotProduct(lookDirection);
			return F32_LOWER_EQUAL_0 ( d );
		}

		//! Get the distance to a point.
		/** Note that this only works if the normal is normalized. */
		inline T getDistanceTo(const Vector3<T>& point) const
		{
			return point.dotProduct(Normal) + D;
		}

		//! Normal vector of the plane.
		Vector3<T> Normal;
		//! Distance from origin.
		T D;
	};

	// define float plane
	typedef Plane<TF32> PlaneFloat;
	// define double plane
	typedef Plane<TF64> PlaneDouble;
	// define int plane
	typedef Plane<TS32> PlaneInt;
	// define uint plane
	typedef Plane<TU32> PlaneUInt;
}

#endif