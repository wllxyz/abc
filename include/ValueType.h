/*
 * ValueType.h
 *
 *  Created on: Sep 2, 2015
 *      Author: wll
 */

#ifndef VALUE_TYPE_H_
#define VALUE_TYPE_H_

#include <map>
#include <string>
#include "WllString.h"
#include <ostream>
#include <assert.h>
#include <algorithm>
#include "Exception.h"

class DivZeroException : public Exception
{
public:
	DivZeroException():Exception("DivZeroException"){}
};

struct ValueType
{
	std::string value;

	ValueType (){ }

	ValueType(const char* value)
	{
		this->value = value;
	}

	ValueType(const std::string& value)
	{
		this->value = value;
	}

	ValueType(const ValueType& that)
	{
		this->value = that.value;
	}

	const ValueType& operator = (const ValueType& that)
	{
		this->value = that.value;
		return (*this);
	}

	bool operator == (const ValueType& that) const
	{
		return (this->value == that.value);
	}

	virtual bool operator < (const ValueType& that) const
	{
		return (this->value < that.value);
	}

	bool operator <= (const ValueType& that) const
	{
		return (*this < that || *this == that);
	}

	bool operator > (const ValueType& that) const
	{
		return (!(*this < that || *this == that));
	}

	bool operator >= (const ValueType& that) const
	{
		return (!(*this < that));
	}

	bool operator != (const ValueType& that) const
	{
		return (!(*this == that));
	}

	bool IsNature()
	{
		if(this->value.empty()) return false;

		for(std::string::const_iterator i = this->value.begin(); i != this->value.end(); ++i)
		{
			if(!('0' <= *i && *i <= '9'))
			{
				return false;
			}
		}
		return true;
	}

};
inline std::ostream& operator<< (std::ostream& o, const ValueType& record)
{
	return o<<record.value;
}

struct Nature : public ValueType
{

	Nature (){}

	Nature(const char* value) : ValueType(value)
	{
	}

	Nature(const std::string& value) : ValueType(value)
	{
	}

	Nature(const Nature& that)
	{
		this->value = that.value;
	}

	const Nature& operator = (const Nature& that)
	{
		this->value = that.value;
		return (*this);
	}

	virtual bool operator< (const ValueType& that) const
	{
		if(this->value.size()<that.value.size()) return true;
		if(this->value.size()>that.value.size()) return false;
		std::string::const_iterator i1 = this->value.begin();
		std::string::const_iterator i2 = that.value.begin();
		while(i1 != this->value.end() && i2 != that.value.end())
		{
			if(*i1 < *i2) return true;
			if(*i1 > *i2) return false;
			++i1;
			++i2;
		}
		return false;
	}

	Nature operator+ (const Nature& that) const
	{
		std::string result;
		std::string::const_reverse_iterator a = this->value.rbegin();
		std::string::const_reverse_iterator b = that.value.rbegin();
		char cf = 0;
		for(; a != this->value.rend() && b != that.value.rend(); ++a,++b)
		{
			char s = *a - '0' + *b - '0' + cf;
			cf = (s>=10?1:0);
			s = (s>=10?s-10:s);
			result.push_back(s+'0');
		}
		for(; a != this->value.rend(); ++a)
		{
			char s = *a - '0' + cf;
			cf = (s>=10?1:0);
			s = (s>=10?s-10:s);
			result.push_back(s+'0');
		}
		for(; b != that.value.rend(); ++b)
		{
			char s = *b - '0' + cf;
			cf = (s>=10?1:0);
			s = (s>=10?s-10:s);
			result.push_back(s+'0');
		}
		if(cf)
		{
			result.push_back(cf+'0');
		}

		reverse(result.begin(),result.end());
		return Nature(result);
	}

	const Nature& operator += (const Nature& that)
	{
		return *this = *this + that;
	}

	Nature operator- (const Nature& that) const
	{
		assert(that <= *this);

		std::string result;
		std::string::const_reverse_iterator a = this->value.rbegin();
		std::string::const_reverse_iterator b = that.value.rbegin();
		char cf = 0;
		for(; a != this->value.rend() && b != that.value.rend(); ++a,++b)
		{
			char s = *a - *b - cf;
			cf = (s>=0?0:1);
			s = (s>=0?s:s+10);
			result.push_back(s+'0');
		}
		for(; a != this->value.rend(); ++a)
		{
			char s = *a - '0' - cf;
			cf = (s>=0?0:1);
			s = (s>=0?s:s+10);
			result.push_back(s+'0');
		}

		reverse(result.begin(),result.end());
		Nature r(result);
		r.RemoveLeadingZero();
		return r;
	}

	const Nature& operator -= (const Nature& that)
	{
		return *this = *this - that;
	}

	void RemoveLeadingZero()
	{
		std::string result;
		bool skip_flag = true;
		for(std::string::const_iterator i = this->value.begin(); i != this->value.end(); ++i)
		{
			if(!(skip_flag && *i == '0'))
			{
				skip_flag = false;
				result.push_back(*i);;
			}
		}
		if(result.empty())
		{
			result.push_back('0');
		}
		this->value = result;
	}

	Nature operator * (const Nature& that) const
	{
		using namespace Wll::Util;
		long a,b;
		std::string result;
		String2Long(this->value,a);
		String2Long(that.value,b);
		Long2String(a*b,result);
		return Nature(result);
	}

	const Nature& operator *= (const Nature& that)
	{
		return *this = *this * that;
	}


	Nature operator / (const Nature& that) const
	{
		using namespace Wll::Util;
		long a,b;
		std::string result;
		String2Long(this->value,a);
		String2Long(that.value,b);
		if(b==0) throw DivZeroException();
		Long2String(a/b,result);
		return Nature(result);		
	}

	const Nature& operator /= (const Nature& that)
	{
		return *this = *this / that;
	}
};

struct Integer : public ValueType
{
	Integer (){}

	Integer(const char* value) : ValueType(value)
	{
	}

	Integer(const std::string& value) : ValueType(value)
	{
	}

	Integer(const Integer& that)
	{
		this->value = that.value;
	}

	Integer(const Nature& absolute_value, int sign=1)
	{
		if(sign == -1)
		{
			this->value = "-";
			this->value += absolute_value.value;
		}
		else
		{
			this->value = absolute_value.value;
		}
	}

	const Integer& operator = (const Integer& that)
	{
		this->value = that.value;
		return (*this);
	}

	int sign() const
	{
		assert(!this->value.empty());
		if (this->value.size() == 1 && this->value[0] == '0') return 0;
		if (this->value[0] == '-') return -1;
		return +1;
	}

	Nature AbsoluteValue() const
	{
		switch(this->sign())
		{
		case 1:
			return Nature(this->value);
			break;
		case -1:
			return Nature(this->value.substr(1));
			break;
		case 0:
		defalut:
			return Nature("0");
			break;
		}
		return Nature("0");
	}

	virtual bool operator< (const ValueType& that)
	{
		int sign1 = this->sign();
		int sign2 = ((Integer&)that).sign();
		if(sign1<sign2) return true;
		if(sign1>sign2) return false;
		return this->AbsoluteValue()<((Integer&)(that)).AbsoluteValue();
	}

	Integer operator+ (const Integer& that) const
	{
		int sign1 = this->sign();
		int sign2 = that.sign();

		if(sign1 == 0)
		{
			return that;
		}

		if(sign2 == 0)
		{
			return *this;
		}

		if(sign1 == 1)
		{
			if(sign2 == 1)
			{
				return Integer(this->AbsoluteValue()+that.AbsoluteValue());
			}
			else /*sign2 == -1*/
			{
				Nature n1 = this->AbsoluteValue();
				Nature n2 = that.AbsoluteValue();
				if(n1>=n2)
				{
					return Integer(n1-n2);
				}
				else
				{
					return Integer(n2-n1,-1);
				}
			}
		}
		else /*sign1 == -1*/
		{
			if(sign2 == -1)
			{
				return Integer(this->AbsoluteValue()+that.AbsoluteValue(),-1);
			}
			else /*sign2 == 1*/
			{
				Nature n1 = this->AbsoluteValue();
				Nature n2 = that.AbsoluteValue();
				if(n1>=n2)
				{
					return Integer(n1-n2,-1);
				}
				else
				{
					return Integer(n2-n1);
				}
			}
		}
	}

	const Integer& operator+= (const Integer& that)
	{
		return *this = *this + that;
	}

	Integer operator- (const Integer& that) const
	{
		return *this + Integer(that.AbsoluteValue(),-1*that.sign());
	}

	const Integer& operator-= (const Integer& that)
	{
		return *this = *this - that;
	}

	Integer operator * (const Integer& that) const
	{
		return Integer(this->AbsoluteValue()*that.AbsoluteValue(),this->sign()*that.sign());
	}

	const Integer& operator *= (const Integer& that)
	{
		return *this = *this * that;
	}

	Integer operator / (const Integer& that) const
	{
		return Integer(this->AbsoluteValue()/that.AbsoluteValue(),this->sign()*that.sign());
	}

	const Integer& operator /= (const Integer& that)
	{
		return *this = *this / that;
	}
};

struct Real : public ValueType
{
	
};

#endif /* VALUE_TYPE_H_ */
