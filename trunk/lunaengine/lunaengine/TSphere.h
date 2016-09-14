#pragma once
#include "TVector3.h"
namespace TLunaEngine{

	template<typename T>
	class TSphere
	{
	public:
		//! Constructor
		TSphere():center(0,0,0), Radius(0) {}
		//! Constructor
		TSphere(const TVector3<T>& center, T radius) : Center(center), Radius(radius)	{}
		//! Copy constructor
		TSphere(const TSphere<T>& sphere) : Center(sphere.Center), Radius(sphere.Radius)	{}
		//! Destructor
		~TSphere() {}

		// Access methods
		inline TVector3<T>&	GetCenter() { return Center; }
		inline T	GetRadius() { return Radius; }

		inline TSphere<T>& Set(const TVector3<T>& center, T radius)		{ Center = center; Radius = radius; return *this; }
		inline TSphere<T>& SetCenter(const TVector3<T>& center)				{ Center = center; return *this; }
		inline TSphere<T>& SetRadius(T radius)						{ Radius = radius; return *this; }

		inline TSphere<T>& operator=(const TSphere<T>& other)
		{
			Center = other.Center;
			Radius = other.Radius;
			return *this;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Tests if a point is contained within the sphere.
		 *	\param		p	[in] the point to test
		 *	\return		true if inside the sphere
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline bool Contains(const TVector3<T>& p)
		{
			return Center.getDistanceFrom(p) <= Radius;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Tests if a sphere is contained within the sphere.
		 *	\param		sphere	[in] the sphere to test
		 *	\return		true if inside the sphere
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline bool Contains(const TSphere<T>& sphere)
		{
			// If our radius is the smallest, we can't possibly contain the other sphere
			if(Radius < sphere.Radius)	return false;
			// So r is always positive or null now
			T r = Radius - sphere.Radius;
			return Center.getDistanceFromSQ(sphere.Center) <= r*r;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Tests if a box is contained within the sphere.
		 *	\param		aabb	[in] the box to test
		 *	\return		true if inside the sphere
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline bool Contains(const TVector3<T>& min,const TVector3<T>& max)	const
		{
			// I assume if all 8 box vertices are inside the sphere, so does the whole box.
			// Sounds ok but maybe there's a better way?
			T R2 = Radius * Radius;
			//const TVector3<T>& Max = ((ShadowAABB&)&aabb).mMax;
			//const TVector3<T>& Min = ((ShadowAABB&)&aabb).mMin;

			TVector3<T> p;
			p.X=max.X; p.Y=max.Y; p.Z=max.Z;	if(Center.getDistanceFromSQ(p)>=R2)	return false;
			p.X=min.X;							if(Center.getDistanceFromSQ(p)>=R2)	return false;
			p.X=max.X; p.Y=min.Y;				if(Center.getDistanceFromSQ(p)>=R2)	return false;
			p.X=min.X;							if(Center.getDistanceFromSQ(p)>=R2)	return false;
			p.X=max.X; p.Y=max.Y; p.Z=min.Z;	if(Center.getDistanceFromSQ(p)>=R2)	return false;
			p.X=min.X;							if(Center.getDistanceFromSQ(p)>=R2)	return false;
			p.X=max.X; p.Y=min.Y;				if(Center.getDistanceFromSQ(p)>=R2)	return false;
			p.X=min.X;							if(Center.getDistanceFromSQ(p)>=R2)	return false;

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Tests if the sphere intersects another sphere
		 *	\param		sphere	[in] the other sphere
		 *	\return		true if spheres overlap
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline bool Intersect(const TSphere<T>& sphere)	const
		{
			T r = Radius + sphere.Radius;
			return Center.getDistanceFromSQ(sphere.Center) <= r*r;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Checks the sphere is valid.
		 *	\return		true if the box is valid
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline bool isEmpty() const
		{
			// Consistency condition for spheres: Radius >= 0.0f
			if(Radius <= (T)(0.0f))	return false;
			return true;
		}

		TVector3<T>	Center;		//!< TSphere center
		T			Radius;		//!< TSphere radius
	};

	// define float sphere
	typedef TSphere<TF32> TSphereFloat;
	// define double sphere
	typedef TSphere<TF64> TSphereDouble;
	// define int sphere
	typedef TSphere<TS32> TSphereInt;
	// define uint sphere
	typedef TSphere<TU32> TSphereUInt;

}