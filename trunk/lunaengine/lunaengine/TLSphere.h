#ifndef _TLSPHERE_H_
#define _TLSPHERE_H_
#include "TLVector3.h"
namespace TLunaEngine{

	template<typename T>
	class Sphere
	{
	public:
		//! Constructor
		Sphere():center(0,0,0), Radius(0) {}
		//! Constructor
		Sphere(const Vector3<T>& center, T radius) : Center(center), Radius(radius)	{}
		//! Copy constructor
		Sphere(const Sphere<T>& sphere) : Center(sphere.Center), Radius(sphere.Radius)	{}
		//! Destructor
		~Sphere() {}

		// Access methods
		inline Vector3<T>&	GetCenter() { return Center; }
		inline T	GetRadius() { return Radius; }

		inline Sphere<T>& Set(const Vector3<T>& center, T radius)		{ Center = center; Radius = radius; return *this; }
		inline Sphere<T>& SetCenter(const Vector3<T>& center)				{ Center = center; return *this; }
		inline Sphere<T>& SetRadius(T radius)						{ Radius = radius; return *this; }

		inline Sphere<T>& operator=(const Sphere<T>& other)
		{
			Center = other.Center;
			Radius = other.Radius;
			return *this;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Tests if a point is contained within the sphere.
		 *	\param		p	[in] the point to test
		 *	\return		TTRUE if inside the sphere
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline TBOOL Contains(const Vector3<T>& p)
		{
			return Center.getDistanceFrom(p) <= Radius;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Tests if a sphere is contained within the sphere.
		 *	\param		sphere	[in] the sphere to test
		 *	\return		TTRUE if inside the sphere
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline TBOOL Contains(const Sphere<T>& sphere)
		{
			// If our radius is the smallest, we can't possibly contain the other sphere
			if(Radius < sphere.Radius)	return TFALSE;
			// So r is always positive or null now
			T r = Radius - sphere.Radius;
			return Center.getDistanceFromSQ(sphere.Center) <= r*r;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Tests if a box is contained within the sphere.
		 *	\param		aabb	[in] the box to test
		 *	\return		TTRUE if inside the sphere
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline TBOOL Contains(const Vector3<T>& min,const Vector3<T>& max)	const
		{
			// I assume if all 8 box vertices are inside the sphere, so does the whole box.
			// Sounds ok but maybe there's a better way?
			T R2 = Radius * Radius;
			//const Vector3<T>& Max = ((ShadowAABB&)&aabb).mMax;
			//const Vector3<T>& Min = ((ShadowAABB&)&aabb).mMin;

			Vector3<T> p;
			p.X=max.X; p.Y=max.Y; p.Z=max.Z;	if(Center.getDistanceFromSQ(p)>=R2)	return TFALSE;
			p.X=min.X;							if(Center.getDistanceFromSQ(p)>=R2)	return TFALSE;
			p.X=max.X; p.Y=min.Y;				if(Center.getDistanceFromSQ(p)>=R2)	return TFALSE;
			p.X=min.X;							if(Center.getDistanceFromSQ(p)>=R2)	return TFALSE;
			p.X=max.X; p.Y=max.Y; p.Z=min.Z;	if(Center.getDistanceFromSQ(p)>=R2)	return TFALSE;
			p.X=min.X;							if(Center.getDistanceFromSQ(p)>=R2)	return TFALSE;
			p.X=max.X; p.Y=min.Y;				if(Center.getDistanceFromSQ(p)>=R2)	return TFALSE;
			p.X=min.X;							if(Center.getDistanceFromSQ(p)>=R2)	return TFALSE;

			return TTRUE;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Tests if the sphere intersects another sphere
		 *	\param		sphere	[in] the other sphere
		 *	\return		TTRUE if spheres overlap
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline TBOOL Intersect(const Sphere<T>& sphere)	const
		{
			T r = Radius + sphere.Radius;
			return Center.getDistanceFromSQ(sphere.Center) <= r*r;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Checks the sphere is valid.
		 *	\return		TTRUE if the box is valid
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline TBOOL isEmpty() const
		{
			// Consistency condition for spheres: Radius >= 0.0f
			if(Radius <= (T)(0.0f))	return TFALSE;
			return TTRUE;
		}

		Vector3<T>	Center;		//!< Sphere center
		T			Radius;		//!< Sphere radius
	};

	// define float sphere
	typedef Sphere<TF32> SphereFloat;
	// define double sphere
	typedef Sphere<TF64> SphereDouble;
	// define int sphere
	typedef Sphere<TS32> SphereInt;
	// define uint sphere
	typedef Sphere<TU32> SphereUInt;

}

#endif