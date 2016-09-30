#ifndef _TLDEGREE_H_
#define _TLDEGREE_H_
#include "TLCommonTypeDef.h"

namespace TLunaEngine
{
	class Radian;
	// degree class from ogre
	class Degree
	{
		TF32 mDeg; // if you get an error here - make sure to define/typedef 'TF32' first

	public:
		Degree ( TF32 d=0 ) : mDeg(d) {}
		Degree ( const Radian& r );
		Degree& operator = ( const TF32& f ) { mDeg = f; return *this; }
		Degree& operator = ( const Degree& d ) { mDeg = d.mDeg; return *this; }
		Degree& operator = ( const Radian& r );

		TF32 valueDegrees() const { return mDeg; }
		TF32 valueRadians() const; // see bottom of this file
		//TF32 valueAngleUnits() const;

		const Degree& operator + () const { return *this; }
		Degree operator + ( const Degree& d ) const { return Degree ( mDeg + d.mDeg ); }
		Degree operator + ( const Radian& r ) const;
		Degree& operator += ( const Degree& d ) { mDeg += d.mDeg; return *this; }
		Degree& operator += ( const Radian& r );
		Degree operator - () const { return Degree(-mDeg); }
		Degree operator - ( const Degree& d ) const { return Degree ( mDeg - d.mDeg ); }
		Degree operator - ( const Radian& r ) const;
		Degree& operator -= ( const Degree& d ) { mDeg -= d.mDeg; return *this; }
		Degree& operator -= ( const Radian& r );
		Degree operator * ( TF32 f ) const { return Degree ( mDeg * f ); }
        Degree operator * ( const Degree& f ) const { return Degree ( mDeg * f.mDeg ); }
		Degree& operator *= ( TF32 f ) { mDeg *= f; return *this; }
		Degree operator / ( TF32 f ) const { return Degree ( mDeg / f ); }
		Degree& operator /= ( TF32 f ) { mDeg /= f; return *this; }

		TBOOL operator <  ( const Degree& d ) const { return mDeg <  d.mDeg; }
		TBOOL operator <= ( const Degree& d ) const { return mDeg <= d.mDeg; }
		TBOOL operator == ( const Degree& d ) const { return mDeg == d.mDeg; }
		TBOOL operator != ( const Degree& d ) const { return mDeg != d.mDeg; }
		TBOOL operator >= ( const Degree& d ) const { return mDeg >= d.mDeg; }
		TBOOL operator >  ( const Degree& d ) const { return mDeg >  d.mDeg; }
	};
}

#endif