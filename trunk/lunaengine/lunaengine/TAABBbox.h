#pragma once
#include "TVector3.h"
#include "TMatrix4x4.h"
namespace TLunaEngine{

	template<typename T>
	class TAABBbox
	{
	public:
		//! Default Constructor.
		TAABBbox(): Min(-1,-1,-1), Max(1,1,1) {}
		//! Constructor with min edge and max edge.
		TAABBbox(const TVector3<T>& min, const TVector3<T>& max): Min(min), Max(max) {}
		//! Constructor with only one point.
		TAABBbox(const TVector3<T>& init): Min(init), Max(init) {}
		//! Constructor with min edge and max edge as single values, not vectors.
		TAABBbox(T minx, T miny, T minz, T maxx, T maxy, T maxz): Min(minx, miny, minz), Max(maxx, maxy, maxz) {}
		// 拷贝
		TAABBbox(const TAABBbox<T>& other)
		{
			Min = other.Min;
			Max = other.Max;
		}
		// 析构
		~TAABBbox()
		{
		}

		// operators
		//! Equality operator
		/** \param other box to compare with.
		\return True if both boxes are equal, else false. */
		inline bool operator==(const TAABBbox<T>& other) const { return (Min == other.Min && other.Max == Max);}
		//! Inequality operator
		/** \param other box to compare with.
		\return True if both boxes are different, else false. */
		inline bool operator!=(const TAABBbox<T>& other) const { return !(Min == other.Min && other.Max == Max);}
		// 赋值
		inline TAABBbox<T>& operator=(const TAABBbox<T>& other)
		{
			Min = other.Min;
			Max = other.Max;
			return *this;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		*	Setups an AABB from center & extents vectors.
		*	\param		c			[in] the center point
		*	\param		e			[in] the extents vector
		*/
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline void SetCenterExtents(const TVector3<T>& c, const TVector3<T>& e)
		{ Min = c - e; Max = c + e;	}

		// functions
		//! Adds a point to the bounding box
		/** The box grows bigger, if point was outside of the box.
		\param p: Point to add into the box. */
		inline void addInternalPoint(const TVector3<T>& p)
		{
			addInternalPoint(p.X, p.Y, p.Z);
		}

		//! Adds another bounding box
		/** The box grows bigger, if the new box was outside of the box.
		\param b: Other bounding box to add into this box. */
		inline void addInternalBox(const TAABBbox<T>& b)
		{
			addInternalPoint(b.Max);
			addInternalPoint(b.Min);
		}

		//! Resets the bounding box to a one-point box.
		/** \param x X coord of the point.
		\param y Y coord of the point.
		\param z Z coord of the point. */
		inline void reset(T x, T y, T z)
		{
			Max.set(x,y,z);
			Min = Max;
		}

		//! Resets the bounding box to a one-point box.
		/** \param initValue New point. */
		inline void reset(const TVector3<T>& initValue)
		{
			Max = initValue;
			Min = initValue;
		}

		//! Adds a point to the bounding box
		/** The box grows bigger, if point is outside of the box.
		\param x X coordinate of the point to add to this box.
		\param y Y coordinate of the point to add to this box.
		\param z Z coordinate of the point to add to this box. */
		inline void addInternalPoint(T x, T y, T z)
		{
			if (x>Max.X) Max.X = x;
			if (y>Max.Y) Max.Y = y;
			if (z>Max.Z) Max.Z = z;

			if (x<Min.X) Min.X = x;
			if (y<Min.Y) Min.Y = y;
			if (z<Min.Z) Min.Z = z;
		}

		//! Determines if a point is within this box.
		/** \param p: Point to check.
		\return True if the point is within the box and false if not */
		inline bool isPointInside(const TVector3<T>& p) const
		{
			return (p.X >= Min.X && p.X <= Max.X &&
				p.Y >= Min.Y && p.Y <= Max.Y &&
				p.Z >= Min.Z && p.Z <= Max.Z);
		}

		//! Determines if a point is within this box and its borders.
		/** \param p: Point to check.
		\return True if the point is within the box and false if not. */
		inline bool isPointTotalInside(const TVector3<T>& p) const
		{
			return (p.X > Min.X && p.X < Max.X &&
				p.Y > Min.Y && p.Y < Max.Y &&
				p.Z > Min.Z && p.Z < Max.Z);
		}

		//! Determines if the box intersects with another box.
		/** \param other: Other box to check a intersection with.
		\return True if there is an intersection with the other box,
		otherwise false. */
		inline bool intersectsWithBox(const TAABBbox<T>& other) const
		{
			return (Min <= other.Max && Max >= other.Min);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		*	Computes the 1D-intersection between two AABBs, on a given axis.
		*	\param		a		[in] the other AABB
		*	\param		axis	[in] the axis (0, 1, 2)
		*	\return		true on intersection
		*/
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline bool intersectsWithBox(const TAABBbox<T>& other, int axis)	const
		{
			if(Max[axis] < other.Min[axis] || other.Max[axis] < Min[axis])	return false;
			return true;
		}

		//! Check if this box is completely inside the 'other' box.
		/** \param other: Other box to check against.
		\return True if this box is completly inside the other box,
		otherwise false. */
		inline bool isFullInside(const TAABBbox<T>& other) const
		{
			return Min >= other.Min && Max <= other.Max;
		}

		//! Tests if the box intersects with a line
		/** \param linemiddle Center of the line.
		\param linevect Vector of the line.
		\param halflength Half length of the line.
		\return True if there is an intersection, else false. */
		inline bool intersectsWithLine(const TVector3<T>& linemiddle,
					const TVector3<T>& linevect,
					T halflength) const
		{
			const TVector3<T> e = getExtent() * (T)0.5;
			const TVector3<T> t = getCenter() - linemiddle;

			if ((abs_(t.X) > e.X + halflength * abs_(linevect.X)) ||
				(abs_(t.Y) > e.Y + halflength * abs_(linevect.Y)) ||
				(abs_(t.Z) > e.Z + halflength * abs_(linevect.Z)) )
				return false;

			T r = e.Y * abs_(linevect.Z) + e.Z * abs_(linevect.Y);
			if (abs_(t.Y*linevect.Z - t.Z*linevect.Y) > r )
				return false;

			r = e.X * abs_(linevect.Z) + e.Z * abs_(linevect.X);
			if (abs_(t.Z*linevect.X - t.X*linevect.Z) > r )
				return false;

			r = e.X * abs_(linevect.Y) + e.Y * abs_(linevect.X);
			if (abs_(t.X*linevect.Y - t.Y*linevect.X) > r)
				return false;

			return true;
		}

		//! Classifies a relation with a plane.
		/** \param plane Plane to classify relation to.
		\return Returns ISREL3D_FRONT if the box is in front of the plane,
		ISREL3D_BACK if the box is behind the plane, and
		ISREL3D_CLIPPED if it is on both sides of the plane. */
		inline EIntersectionRelation3D classifyPlaneRelation(const TVector3<T>& Normal,T D) const
		{
			TVector3<T> nearPoint(Max);
			TVector3<T> farPoint(Min);

			if (Normal.X > (T)0)
			{
				nearPoint.X = Min.X;
				farPoint.X = Max.X;
			}

			if (Normal.Y > (T)0)
			{
				nearPoint.Y = Min.Y;
				farPoint.Y = Max.Y;
			}

			if (Normal.Z > (T)0)
			{
				nearPoint.Z = Min.Z;
				farPoint.Z = Max.Z;
			}

			if (Normal.dotProduct(nearPoint) + D > (T)0)
				return ISREL3D_FRONT;

			if (Normal.dotProduct(farPoint) + D > (T)0)
				return ISREL3D_CLIPPED;

			return ISREL3D_BACK;
		}


		//! Get center of the bounding box
		/** \return Center of the bounding box. */
		inline TVector3<T> getCenter() const
		{
			return (Min + Max) * (T)0.5f;
		}


		//! Get extent of the box
		/** \return Extent of the bounding box. */
		inline TVector3<T> getExtent() const
		{
			return Max - Min;
		}

		// Get half extent
		inline TVector3<T> getHalfExtent() const
		{
			return (Max - Min) * (T)0.5f;
		}

		// X,Y,Z的大小差
		inline T	GetWidth() const
		{ return Max.X - Min.X;					}
		inline T	GetHeight()	const
		{ return Max.Y - Min.Y;					}
		inline T	GetDepth() const
		{ return Max.Z - Min.Z;					}

		//! Volume
		inline T	GetVolume()	const
		{ return GetWidth() * GetHeight() * GetDepth();	}


		//! Stores all 8 edges of the box into an array
		/** \param edges: Pointer to array of 8 edges. */
		inline void getEdges(TVector3<T> *edges) const
		{
			if(!edges)
			{
				return;
			}

			const TVector3<T> middle = getCenter();
			const TVector3<T> diag = middle - Max;

			/*
			Edges are stored in this way:
			Hey, am I an ascii artist, or what? :) niko.
				  /3--------/7
				 /  |      / |
				/   |     /  |
				1---------5  |
				|   2- - -| -6
				|  /      |  /
				|/        | /
				0---------4/
			*/

			edges[0].set(middle.X + diag.X, middle.Y + diag.Y, middle.Z + diag.Z);
			edges[1].set(middle.X + diag.X, middle.Y - diag.Y, middle.Z + diag.Z);
			edges[2].set(middle.X + diag.X, middle.Y + diag.Y, middle.Z - diag.Z);
			edges[3].set(middle.X + diag.X, middle.Y - diag.Y, middle.Z - diag.Z);
			edges[4].set(middle.X - diag.X, middle.Y + diag.Y, middle.Z + diag.Z);
			edges[5].set(middle.X - diag.X, middle.Y - diag.Y, middle.Z + diag.Z);
			edges[6].set(middle.X - diag.X, middle.Y + diag.Y, middle.Z - diag.Z);
			edges[7].set(middle.X - diag.X, middle.Y - diag.Y, middle.Z - diag.Z);
		}


		//! Check if the box is empty.
		/** This means that there is no space between the min and max
		edge.
		\return True if box is empty, else false. */
		inline bool isEmpty() const
		{
			return Min.equals ( Max );
		}


		//! Repairs the box.
		/** Necessary if for example Min and Max are swapped. */
		inline void repair()
		{
			T t;

			if (Min.X > Max.X)
				{ t=Min.X; Min.X = Max.X; Max.X=t; }
			if (Min.Y > Max.Y)
				{ t=Min.Y; Min.Y = Max.Y; Max.Y=t; }
			if (Min.Z > Max.Z)
				{ t=Min.Z; Min.Z = Max.Z; Max.Z=t; }
		}

		//! Operator for AABB *= float. Scales the extents, keeps same center.
		inline TAABBbox<T>& scale(T s)
		{
			TVector3<T> Center = getCenter();
			TVector3<T> Extents = getHalfExtent();
			SetCenterExtents(Center, Extents * s);
			return *this;
		}

		//! Calculates a new interpolated bounding box.
		/** \param other: other box to interpolate between
		\param d: value between 0.0f and 1.0f.
		\return Interpolated box. */
		inline TAABBbox<T> getInterpolated(const TAABBbox<T>& other, TF32 d) const
		{
			TF32 inv = 1.0f - d;
			return TAABBbox<T>((other.Min*inv) + (Min*d),
				(other.Max*inv) + (Max*d));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Recomputes the AABB after an arbitrary transform by a 4x4 matrix.
		 *	Original code by Charles Bloom on the GD-Algorithm list. (I slightly modified it)
		 *	\param		mtx			[in] the transform matrix
		 *	\param		aabb		[out] the transformed AABB [can be *this]
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline void Rotate(const TMatrix4x4<T>& mtx, TAABBbox<T>& aabb)	const
		{
			// The three edges transformed: you can efficiently transform an X-only vector
			// by just getting the "X" column of the matrix
			TVector3<T> vx,vy,vz;
			vx = TVector3<T>(mtx[0][0],mtx[0][1],mtx[0][2]);
			vy = TVector3<T>(mtx[1][0],mtx[1][1],mtx[1][2]);
			vz = TVector3<T>(mtx[2][0],mtx[2][1],mtx[2][2])
			vx *= (Max.X - Min.X);
			vy *= (Max.Y - Min.Y);
			vz *= (Max.Z - Min.Z);

			// Transform the min point
			aabb.Min = aabb.Max = mtx.TransformPoint(Min);

			// Take the transformed min & axes and find new extents
			// Using CPU code in the right place is faster...
			if(IS_NEGATIVE_FLOAT(vx.X))	aabb.Min.X += vx.X; else aabb.Max.X += vx.X;
			if(IS_NEGATIVE_FLOAT(vx.Y))	aabb.Min.Y += vx.Y; else aabb.Max.Y += vx.Y;
			if(IS_NEGATIVE_FLOAT(vx.Z))	aabb.Min.Z += vx.Z; else aabb.Max.Z += vx.Z;
			if(IS_NEGATIVE_FLOAT(vy.X))	aabb.Min.X += vy.X; else aabb.Max.X += vy.X;
			if(IS_NEGATIVE_FLOAT(vy.Y))	aabb.Min.Y += vy.Y; else aabb.Max.Y += vy.Y;
			if(IS_NEGATIVE_FLOAT(vy.Z))	aabb.Min.Z += vy.Z; else aabb.Max.Z += vy.Z;
			if(IS_NEGATIVE_FLOAT(vz.X))	aabb.Min.X += vz.X; else aabb.Max.X += vz.X;
			if(IS_NEGATIVE_FLOAT(vz.Y))	aabb.Min.Y += vz.Y; else aabb.Max.Y += vz.Y;
			if(IS_NEGATIVE_FLOAT(vz.Z))	aabb.Min.Z += vz.Z; else aabb.Max.Z += vz.Z;
		}

		//! The near edge
		TVector3<T> Min;
		//! The far edge
		TVector3<T> Max;
	};

	// define float aabbBox
	typedef TAABBbox<TF32> TAABBFloat;
	// define double aabbBox
	typedef TAABBbox<TF64> TAABBDouble;
	// define int aabbBox
	typedef TAABBbox<TS32> TAABBInt;
	// define uint aabbBox
	typedef TAABBbox<TU32> TAABBUInt;
}