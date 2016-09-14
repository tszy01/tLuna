#include "stdafx.h"
#include "TRadian.h"
#include "TDegree.h"
#include "TFPU.h"

namespace TLunaEngine
{
	TRadian::TRadian ( const TDegree& d ) : mRad(d.valueRadians())
	{
	}

	TRadian& TRadian::operator = ( const TDegree& d ) {
		mRad = d.valueRadians(); return *this;
	}

	TRadian TRadian::operator + ( const TDegree& d ) const {
		return TRadian ( mRad + d.valueRadians() );
	}

	TRadian& TRadian::operator += ( const TDegree& d ) {
		mRad += d.valueRadians();
		return *this;
	}

	TRadian TRadian::operator - ( const TDegree& d ) const {
		return TRadian ( mRad - d.valueRadians() );
	}

	TRadian& TRadian::operator -= ( const TDegree& d ) {
		mRad -= d.valueRadians();
		return *this;
	}

	TF32 TRadian::valueDegrees() const
	{
		return mRad * RADTODEG;
	}
}