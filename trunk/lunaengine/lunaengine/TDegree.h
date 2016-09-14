#pragma once
#include "TCommonTypeDef.h"

namespace TLunaEngine
{
	class TRadian;
	// degree class from ogre
	class TDegree
	{
		TF32 mDeg; // if you get an error here - make sure to define/typedef 'TF32' first

	public:
		TDegree ( TF32 d=0 ) : mDeg(d) {}
		TDegree ( const TRadian& r );
		TDegree& operator = ( const TF32& f ) { mDeg = f; return *this; }
		TDegree& operator = ( const TDegree& d ) { mDeg = d.mDeg; return *this; }
		TDegree& operator = ( const TRadian& r );

		TF32 valueDegrees() const { return mDeg; }
		TF32 valueRadians() const; // see bottom of this file
		//TF32 valueAngleUnits() const;

		const TDegree& operator + () const { return *this; }
		TDegree operator + ( const TDegree& d ) const { return TDegree ( mDeg + d.mDeg ); }
		TDegree operator + ( const TRadian& r ) const;
		TDegree& operator += ( const TDegree& d ) { mDeg += d.mDeg; return *this; }
		TDegree& operator += ( const TRadian& r );
		TDegree operator - () const { return TDegree(-mDeg); }
		TDegree operator - ( const TDegree& d ) const { return TDegree ( mDeg - d.mDeg ); }
		TDegree operator - ( const TRadian& r ) const;
		TDegree& operator -= ( const TDegree& d ) { mDeg -= d.mDeg; return *this; }
		TDegree& operator -= ( const TRadian& r );
		TDegree operator * ( TF32 f ) const { return TDegree ( mDeg * f ); }
        TDegree operator * ( const TDegree& f ) const { return TDegree ( mDeg * f.mDeg ); }
		TDegree& operator *= ( TF32 f ) { mDeg *= f; return *this; }
		TDegree operator / ( TF32 f ) const { return TDegree ( mDeg / f ); }
		TDegree& operator /= ( TF32 f ) { mDeg /= f; return *this; }

		bool operator <  ( const TDegree& d ) const { return mDeg <  d.mDeg; }
		bool operator <= ( const TDegree& d ) const { return mDeg <= d.mDeg; }
		bool operator == ( const TDegree& d ) const { return mDeg == d.mDeg; }
		bool operator != ( const TDegree& d ) const { return mDeg != d.mDeg; }
		bool operator >= ( const TDegree& d ) const { return mDeg >= d.mDeg; }
		bool operator >  ( const TDegree& d ) const { return mDeg >  d.mDeg; }
	};
}