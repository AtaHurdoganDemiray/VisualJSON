// FloatingPointNumberToStr.h

#ifndef __FloatingPointNumberToStr_h__
#define __FloatingPointNumberToStr_h__

// based on following
// https://stackoverflow.com/questions/4459987/convert-float-to-string-without-losing-precision

template<typename FloatingPointType>
MilString MilConvert(const FloatingPointType &value)
{
	MilStringStream ss;
	ss << std::setprecision(std::numeric_limits<FloatingPointType>::digits10 + 1);
	ss << value;
	return ss.str();
}

template<typename FloatingPointType>
FloatingPointType MilConvertFrom(const MilString &str)
{
	FloatingPointType value;
	MilStringStream ss;
	ss << str;
	ss >> value;
	return value;
}
#endif