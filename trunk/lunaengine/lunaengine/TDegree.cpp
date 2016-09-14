#include "stdafx.h"
#include "TDegree.h"
#include "TRadian.h"
#include "TFPU.h"

namespace TLunaEngine
{
	TDegree::TDegree ( const TRadian& r ) : mDeg(r.valueDegrees())
	{
	}

	TDegree& TDegree::operator = ( const TRadian& r )
	{
		mDeg = r.valueDegrees(); 
		return *this;
	}

	TF32 TDegree::valueRadians() const
	{
		return mDeg * DEGTORAD;
	}

	TDegree TDegree::operator + ( const TRadian& r ) const
	{
		return TDegree ( mDeg + r.valueDegrees() );
	}

	TDegree& TDegree::operator += ( const TRadian& r )
	{
		mDeg += r.valueDegrees(); 
		return *this;
	}

	TDegree TDegree::operator - ( const TRadian& r ) const
	{
		return TDegree ( mDeg - r.valueDegrees() );
	}

	TDegree& TDegree::operator -= ( const TRadian& r )
	{
		mDeg -= r.valueDegrees(); 
		return *this;
	}
}