#ifndef _TLTRIANGLE_H_
#define _TLTRIANGLE_H_
#include "TLVector3.h"
namespace TLunaEngine{

	template<typename T>
	class Triangle
	{
	public:
		//! Constructor for an all 0 triangle
		Triangle() {}
		//! Constructor for triangle with given three vertices
		Triangle(const Vector3<T>& v1,const Vector3<T>& v2,const Vector3<T>& v3) : pointA(v1), pointB(v2), pointC(v3) {}
		// 拷贝
		Triangle(const Triangle<T>& other) 
		{
			pointA = other.pointA;
			pointB = other.pointB;
			pointC = other.pointC;
		}
		// 析构
		~Triangle()
		{
		}

		//! Equality operator
		inline TBOOL operator==(const Triangle<T>& other) const
		{
			return other.pointA==pointA && other.pointB==pointB && other.pointC==pointC;
		}

		//! Inequality operator
		inline TBOOL operator!=(const Triangle<T>& other) const
		{
			return !(*this==other);
		}

		// 赋值
		inline Triangle<T>& operator=(const Triangle<T>& other)
		{
			pointA = other.pointA;
			pointB = other.pointB;
			pointC = other.pointC;
			return *this;
		}

		//! Get the closest point on a triangle to a point on the same plane.
		/** \param p Point which must be on the same plane as the triangle.
		\return The closest point of the triangle */
		inline Vector3<T> closestPointOnTriangle(const Vector3<T>& p) const
		{
			const Vector3<T> rab = Line<T>(pointA, pointB).getClosestPoint(p);
			const Vector3<T> rbc = Line<T>(pointB, pointC).getClosestPoint(p);
			const Vector3<T> rca = Line<T>(pointC, pointA).getClosestPoint(p);

			const T d1 = rab.getDistanceFrom(p);
			const T d2 = rbc.getDistanceFrom(p);
			const T d3 = rca.getDistanceFrom(p);

			if (d1 < d2)
				return d1 < d3 ? rab : rca;

			return d2 < d3 ? rbc : rca;
		}

		//! Check if a point is inside the triangle
		/** \param p Point to test. Assumes that this point is already
		on the plane of the triangle.
		\return True if the point is inside the triangle, otherwise TFALSE. */
		inline TBOOL isPointInside(const Vector3<T>& p) const
		{
			return (isOnSameSide(p, pointA, pointB, pointC) &&
				isOnSameSide(p, pointB, pointA, pointC) &&
				isOnSameSide(p, pointC, pointA, pointB));
		}

		//! Check if a point is inside the triangle.
		/** This method is an implementation of the example used in a
		paper by Kasper Fauerby original written by Keidy from
		Mr-Gamemaker.
		\param p Point to test. Assumes that this point is already
		on the plane of the triangle.
		\return True if point is inside the triangle, otherwise TFALSE. */
		inline TBOOL isPointInsideFast(const Vector3<T>& p) const
		{
			const Vector3<T> f = pointB - pointA;
			const Vector3<T> g = pointC - pointA;

			const T a = f.dotProduct(f);
			const T b = f.dotProduct(g);
			const T c = g.dotProduct(g);

			const Vector3<T> vp = p - pointA;
			const T d = vp.dotProduct(f);
			const T e = vp.dotProduct(g);

			T x = (d*c)-(e*b);
			T y = (e*a)-(d*b);
			const T ac_bb = (a*c)-(b*b);
			T z = x+y-ac_bb;

			// return sign(z) && !(sign(x)||sign(y))
			return (( (IR(z)) & ~((IR(x))|(IR(y))) ) & 0x80000000)!=0;
		}

		//! Get an intersection with a 3d line.
		/** Please note that also points are returned as intersection which
		are on the line, but not between the start and end point of the line.
		If you want the returned point be between start and end
		use getIntersectionWithLimitedLine().
		\param linePoint Point of the line to intersect with.
		\param lineVect Vector of the line to intersect with.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, TFALSE if there was not. */
		inline TBOOL getIntersectionWithLine(const Vector3<T>& linePoint,
			const Vector3<T>& lineVect, Vector3<T>& outIntersection, TF32& outTValue) const
		{
			if (getIntersectionOfPlaneWithLine(linePoint, lineVect, outIntersection, outTValue))
				return isPointInside(outIntersection);

			return TFALSE;
		}


		//! Calculates the intersection between a 3d line and the plane the triangle is on.
		/** \param lineVect Vector of the line to intersect with.
		\param linePoint Point of the line to intersect with.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, else TFALSE. */
		inline TBOOL getIntersectionOfPlaneWithLine(const Vector3<T>& linePoint,
			const Vector3<T>& lineVect, Vector3<T>& outIntersection) const
		{
			const Vector3<T> normal = getNormal().normalize();
			T t2;

			if ( iszero ( t2 = normal.dotProduct(lineVect) ) )
				return TFALSE;

			T d = pointA.dotProduct(normal);
			T t = -(normal.dotProduct(linePoint) - d) / t2;
			outIntersection = linePoint + (lineVect * t);
			return TTRUE;
		}


		//! Get the normal of the triangle.
		/** Please note: The normal is not always normalized. */
		inline Vector3<T> getNormal() const
		{
			return (pointB - pointA).crossProduct(pointC - pointA);
		}

		//! Test if the triangle would be front or backfacing from any point.
		/** Thus, this method assumes a camera position from which the
		triangle is definitely visible when looking at the given direction.
		Do not use this method with points as it will give wrong results!
		\param lookDirection Look direction.
		\return True if the plane is front facing and TFALSE if it is backfacing. */
		inline TBOOL isFrontFacing(const Vector3<T>& lookDirection) const
		{
			const Vector3<T> n = getNormal().normalize();
			const T d = n.dotProduct(lookDirection);
			return F32_LOWER_EQUAL_0(d);
		}

		////! Get the plane of this triangle.
		//plane3d<T> getPlane() const
		//{
		//	return plane3d<T>(pointA, pointB, pointC);
		//}

		//! Get the area of the triangle
		inline T getArea() const
		{
			return (pointB - pointA).crossProduct(pointC - pointA).getLength() * 0.5;

		}

		//! sets the triangle's points
		inline TVOID set(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c)
		{
			pointA = a;
			pointB = b;
			pointC = c;
		}

		//! the three points of the triangle
		Vector3<T> pointA;
		Vector3<T> pointB;
		Vector3<T> pointC;

		// p1是否在p2,a,b的内部
		inline TBOOL isOnSameSide(const Vector3<T>& p1, const Vector3<T>& p2,
			const Vector3<T>& a, const Vector3<T>& b) const
		{
			Vector3<T> bminusa = b - a;
			Vector3<T> cp1 = bminusa.crossProduct(p1 - a);
			Vector3<T> cp2 = bminusa.crossProduct(p2 - a);
			return (cp1.dotProduct(cp2) >= 0.0f);
		}
	};

	// define float triangle
	typedef Triangle<TF32> TriangleFloat;
	// define double triangle
	typedef Triangle<TF64> TriangleDouble;
	// define int triangle
	typedef Triangle<TS32> TriangleInt;
	// define uint triangle
	typedef Triangle<TU32> TriangleUInt;
}

#endif