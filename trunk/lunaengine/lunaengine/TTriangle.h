#pragma once
#include "TVector3.h"
namespace TLunaEngine{

	template<typename T>
	class TTriangle
	{
	public:
		//! Constructor for an all 0 triangle
		TTriangle() {}
		//! Constructor for triangle with given three vertices
		TTriangle(const TVector3<T>& v1,const TVector3<T>& v2,const TVector3<T>& v3) : pointA(v1), pointB(v2), pointC(v3) {}
		// 拷贝
		TTriangle(const TTriangle<T>& other) 
		{
			pointA = other.pointA;
			pointB = other.pointB;
			pointC = other.pointC;
		}
		// 析构
		~TTriangle()
		{
		}

		//! Equality operator
		inline bool operator==(const TTriangle<T>& other) const
		{
			return other.pointA==pointA && other.pointB==pointB && other.pointC==pointC;
		}

		//! Inequality operator
		inline bool operator!=(const TTriangle<T>& other) const
		{
			return !(*this==other);
		}

		// 赋值
		inline TTriangle<T>& operator=(const TTriangle<T>& other)
		{
			pointA = other.pointA;
			pointB = other.pointB;
			pointC = other.pointC;
			return *this;
		}

		//! Get the closest point on a triangle to a point on the same plane.
		/** \param p Point which must be on the same plane as the triangle.
		\return The closest point of the triangle */
		inline TVector3<T> closestPointOnTriangle(const TVector3<T>& p) const
		{
			const TVector3<T> rab = TLine<T>(pointA, pointB).getClosestPoint(p);
			const TVector3<T> rbc = TLine<T>(pointB, pointC).getClosestPoint(p);
			const TVector3<T> rca = TLine<T>(pointC, pointA).getClosestPoint(p);

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
		\return True if the point is inside the triangle, otherwise false. */
		inline bool isPointInside(const TVector3<T>& p) const
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
		\return True if point is inside the triangle, otherwise false. */
		inline bool isPointInsideFast(const TVector3<T>& p) const
		{
			const TVector3<T> f = pointB - pointA;
			const TVector3<T> g = pointC - pointA;

			const T a = f.dotProduct(f);
			const T b = f.dotProduct(g);
			const T c = g.dotProduct(g);

			const TVector3<T> vp = p - pointA;
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
		\return True if there was an intersection, false if there was not. */
		inline bool getIntersectionWithLine(const TVector3<T>& linePoint,
			const TVector3<T>& lineVect, TVector3<T>& outIntersection) const
		{
			if (getIntersectionOfPlaneWithLine(linePoint, lineVect, outIntersection))
				return isPointInside(outIntersection);

			return false;
		}


		//! Calculates the intersection between a 3d line and the plane the triangle is on.
		/** \param lineVect Vector of the line to intersect with.
		\param linePoint Point of the line to intersect with.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, else false. */
		inline bool getIntersectionOfPlaneWithLine(const TVector3<T>& linePoint,
			const TVector3<T>& lineVect, TVector3<T>& outIntersection) const
		{
			const TVector3<T> normal = getNormal().normalize();
			T t2;

			if ( iszero ( t2 = normal.dotProduct(lineVect) ) )
				return false;

			T d = pointA.dotProduct(normal);
			T t = -(normal.dotProduct(linePoint) - d) / t2;
			outIntersection = linePoint + (lineVect * t);
			return true;
		}


		//! Get the normal of the triangle.
		/** Please note: The normal is not always normalized. */
		inline TVector3<T> getNormal() const
		{
			return (pointB - pointA).crossProduct(pointC - pointA);
		}

		//! Test if the triangle would be front or backfacing from any point.
		/** Thus, this method assumes a camera position from which the
		triangle is definitely visible when looking at the given direction.
		Do not use this method with points as it will give wrong results!
		\param lookDirection Look direction.
		\return True if the plane is front facing and false if it is backfacing. */
		inline bool isFrontFacing(const TVector3<T>& lookDirection) const
		{
			const TVector3<T> n = getNormal().normalize();
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
		inline void set(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c)
		{
			pointA = a;
			pointB = b;
			pointC = c;
		}

		//! the three points of the triangle
		TVector3<T> pointA;
		TVector3<T> pointB;
		TVector3<T> pointC;

		// p1是否在p2,a,b的内部
		inline bool isOnSameSide(const TVector3<T>& p1, const TVector3<T>& p2,
			const TVector3<T>& a, const TVector3<T>& b) const
		{
			TVector3<T> bminusa = b - a;
			TVector3<T> cp1 = bminusa.crossProduct(p1 - a);
			TVector3<T> cp2 = bminusa.crossProduct(p2 - a);
			return (cp1.dotProduct(cp2) >= 0.0f);
		}
	};

	// define float triangle
	typedef TTriangle<TF32> TTriFloat;
	// define double triangle
	typedef TTriangle<TF64> TTriDouble;
	// define int triangle
	typedef TTriangle<TS32> TTriInt;
	// define uint triangle
	typedef TTriangle<TU32> TTriUInt;
}