#ifndef _TLLINE_H_
#define _TLLINE_H_
#include "TLVector3.h"
namespace TLunaEngine{

	template<typename T>
	class Line
	{
	public:

		//! Default constructor
		/** line from (0,0,0) to (1,1,1) */
		Line() : start(0,0,0), end(1,1,1), dir(1,1,1) { dir.normalize(); }
		//! Constructor with two points
		Line(T xa, T ya, T za, T xb, T yb, T zb) : start(xa, ya, za), end(xb, yb, zb), dir(end-start) { dir.normalize(); }
		//! Constructor with two points as vectors
		Line(const Vector3<T>& start, const Vector3<T>& end) : start(start), end(end), dir(end-start) { dir.normalize(); }
		// 拷贝构造
		Line(const Line<T>& other) 
		{
			start = other.start;
			end = other.end;
			dir = other.dir;
		}
		// 析构
		~Line()
		{
		}

		// operators
		// 把两个向量都加上或者减去参数
		inline Line<T> operator+(const Vector3<T>& point) const { return Line<T>(start + point, end + point); }
		inline Line<T>& operator+=(const Vector3<T>& point) { start += point; end += point; return *this; }
		inline Line<T> operator-(const Vector3<T>& point) const { return Line<T>(start - point, end - point); }
		inline Line<T>& operator-=(const Vector3<T>& point) { start -= point; end -= point; return *this; }
		// 开始和结尾必须都符合
		inline TBOOL operator==(const Line<T>& other) const
		{ return start==other.start && end==other.end;}
		inline TBOOL operator!=(const Line<T>& other) const
		{ return start!=other.start || end!=other.end;}
		// 赋值
		inline Line<T>& operator=(const Line<T>& other)
		{
			start = other.start;
			end = other.end;
			dir = other.dir;
			return *this;
		}

		// functions
		//! Set this line to a new line going through the two points.
		inline TVOID setLine(const T& xa, const T& ya, const T& za, const T& xb, const T& yb, const T& zb)
		{start.set(xa, ya, za); end.set(xb, yb, zb); updateDir();}
		//! Set this line to a new line going through the two points.
		inline TVOID setLine(const Vector3<T>& nstart, const Vector3<T>& nend)
		{start.set(nstart); end.set(nend); updateDir();}
		//! Set this line to new line given as parameter.
		inline TVOID setLine(const Line<T>& line)
		{start.set(line.start); end.set(line.end); updateDir();}

		// 计算方向
		inline Vector3<T>& updateDir()
		{
			Vector3<T> vec = end - start;
			dir = vec.normalize();
			return dir;
		}
		// 得到方向
		inline Vector3<T>& getDir()
		{
			return dir;
		}

		//! Get length of line
		/** \return Length of line. */
		inline T getLength() const { return start.getDistanceFrom(end); }

		//! Get squared length of line
		/** \return Squared length of line. */
		inline T getLengthSQ() const { return start.getDistanceFromSQ(end); }

		//! Get middle of line
		/** \return Center of line. */
		inline Vector3<T> getMiddle() const
		{
			return (start + end) * (T)0.5f;
		}

		//! Get vector of line
		/** \return vector of line. */
		inline Vector3<T> getVector() const
		{
			return end - start;
		}

		//! Check if the given point is between start and end of the line.
		/** Assumes that the point is already somewhere on the line.
		\param point The point to test.
		\return True if point is on the line between start and end, else TFALSE.
		*/
		inline TBOOL isPointBetweenStartAndEnd(const Vector3<T>& point) const
		{
			return point.isBetweenPoints(start, end);
		}

		//! Get the closest point on this line to a point
		/** \param point The point to compare to.
		\return The nearest point which is part of the line. */
		inline Vector3<T> getClosestPoint(const Vector3<T>& point) const
		{
			Vector3<T> c = point - start;
			Vector3<T> v = end - start;
			T d = (T)v.getLength();
			v /= d;
			T t = v.dotProduct(c);

			if (t < (T)0.0f)
				return start;
			if (t > d)
				return end;

			v *= t;
			return start + v;
		}

		//! Check if the line intersects with a shpere
		/** \param sorigin: Origin of the shpere.
		\param sradius: Radius of the sphere.
		\param outdistance: The distance to the first intersection point.
		\return True if there is an intersection.
		If there is one, the distance to the first intersection point
		is stored in outdistance. */
		inline TBOOL getIntersectionWithSphere(const Vector3<T>& sorigin, T sradius, T& outdistance) const
		{
			Vector3<T> q = sorigin - start;
			T c = q.getLength();
			T v = q.dotProduct(getVector().normalize());
			T d = sradius * sradius - (c*c - v*v);

			if (d < (T)0.0f)
				return TFALSE;

			outdistance = (T)((TF32)v - FastSqrt((TF32)d));
			return TTRUE;
		}

		// member variables
		//! Start point of line
		Vector3<T> start;
		//! End point of line
		Vector3<T> end;
		// 方向
		// 方向是设置时自动算出的
		Vector3<T> dir;
	};

	// define float line
	typedef Line<TF32> LineFloat;
	// define double line
	typedef Line<TF64> LineDouble;
	// define int line
	typedef Line<TS32> LineInt;
	// define uint line
	typedef Line<TU32> LineUInt;
}

#endif