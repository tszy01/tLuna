#pragma once
#include "TCommonTypeDef.h"

namespace TLunaEngine
{
	class TDegree;
	// radian class from ogre
	class TRadian
	{
	private:
		TF32 mRad;
	public:
		TRadian ( TF32 r=0 ) : mRad(r) {}
		TRadian ( const TDegree& d );
		TRadian& operator = ( const TF32& f ) { mRad = f; return *this; }
		TRadian& operator = ( const TRadian& r ) { mRad = r.mRad; return *this; }
		TRadian& operator = ( const TDegree& d );

		TF32 valueDegrees() const; // see bottom of this file
		TF32 valueRadians() const { return mRad; }
		//TF32 valueAngleUnits() const;

        const TRadian& operator + () const { return *this; }
		TRadian operator + ( const TRadian& r ) const { return TRadian ( mRad + r.mRad ); }
		TRadian operator + ( const TDegree& d ) const;
		TRadian& operator += ( const TRadian& r ) { mRad += r.mRad; return *this; }
		TRadian& operator += ( const TDegree& d );
		TRadian operator - () const { return TRadian(-mRad); }
		TRadian operator - ( const TRadian& r ) const { return TRadian ( mRad - r.mRad ); }
		TRadian operator - ( const TDegree& d ) const;
		TRadian& operator -= ( const TRadian& r ) { mRad -= r.mRad; return *this; }
		TRadian& operator -= ( const TDegree& d );
		TRadian operator * ( TF32 f ) const { return TRadian ( mRad * f ); }
        TRadian operator * ( const TRadian& f ) const { return TRadian ( mRad * f.mRad ); }
		TRadian& operator *= ( TF32 f ) { mRad *= f; return *this; }
		TRadian operator / ( TF32 f ) const { return TRadian ( mRad / f ); }
		TRadian& operator /= ( TF32 f ) { mRad /= f; return *this; }

		bool operator <  ( const TRadian& r ) const { return mRad <  r.mRad; }
		bool operator <= ( const TRadian& r ) const { return mRad <= r.mRad; }
		bool operator == ( const TRadian& r ) const { return mRad == r.mRad; }
		bool operator != ( const TRadian& r ) const { return mRad != r.mRad; }
		bool operator >= ( const TRadian& r ) const { return mRad >= r.mRad; }
		bool operator >  ( const TRadian& r ) const { return mRad >  r.mRad; }
	};
}