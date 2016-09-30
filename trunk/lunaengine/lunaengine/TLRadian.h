#ifndef _TLRADIAN_H_
#define _TLRADIAN_H_
#include "TLCommonTypeDef.h"

namespace TLunaEngine
{
	class Degree;
	// radian class from ogre
	class Radian
	{
	private:
		TF32 mRad;
	public:
		Radian ( TF32 r=0 ) : mRad(r) {}
		Radian ( const Degree& d );
		Radian& operator = ( const TF32& f ) { mRad = f; return *this; }
		Radian& operator = ( const Radian& r ) { mRad = r.mRad; return *this; }
		Radian& operator = ( const Degree& d );

		TF32 valueDegrees() const; // see bottom of this file
		TF32 valueRadians() const { return mRad; }
		//TF32 valueAngleUnits() const;

        const Radian& operator + () const { return *this; }
		Radian operator + ( const Radian& r ) const { return Radian ( mRad + r.mRad ); }
		Radian operator + ( const Degree& d ) const;
		Radian& operator += ( const Radian& r ) { mRad += r.mRad; return *this; }
		Radian& operator += ( const Degree& d );
		Radian operator - () const { return Radian(-mRad); }
		Radian operator - ( const Radian& r ) const { return Radian ( mRad - r.mRad ); }
		Radian operator - ( const Degree& d ) const;
		Radian& operator -= ( const Radian& r ) { mRad -= r.mRad; return *this; }
		Radian& operator -= ( const Degree& d );
		Radian operator * ( TF32 f ) const { return Radian ( mRad * f ); }
        Radian operator * ( const Radian& f ) const { return Radian ( mRad * f.mRad ); }
		Radian& operator *= ( TF32 f ) { mRad *= f; return *this; }
		Radian operator / ( TF32 f ) const { return Radian ( mRad / f ); }
		Radian& operator /= ( TF32 f ) { mRad /= f; return *this; }

		TBOOL operator <  ( const Radian& r ) const { return mRad <  r.mRad; }
		TBOOL operator <= ( const Radian& r ) const { return mRad <= r.mRad; }
		TBOOL operator == ( const Radian& r ) const { return mRad == r.mRad; }
		TBOOL operator != ( const Radian& r ) const { return mRad != r.mRad; }
		TBOOL operator >= ( const Radian& r ) const { return mRad >= r.mRad; }
		TBOOL operator >  ( const Radian& r ) const { return mRad >  r.mRad; }
	};
}

#endif