#pragma once
#include "types.h"
#include <assert.h>
#include <climits>

namespace util
{
	template<typename type>
	struct BitFlag
	{
	private:
		type val;

	private:
		inline uchar GetMaxBit(void) const { return sizeof(type) * 8 - 1; }
	public:
		BitFlag(void)
			: val(null)
		{
		}
		BitFlag(const type& v)
		{
			val = v;
		}
		BitFlag & operator= (const type& v)
		{
			val = v;
			return *this;
		}

		// Turns a bit ON
		void BitOn(const uchar& bit)
		{
			assert(bit < this->GetMaxBit());
			val |= (1 << bit);
		}
		// Turns a bit OFF
		void BitOff(const uchar& bit)
		{
			assert(bit < this->GetMaxBit());
			val &= ~(1 << bit);
		}
		// Toggles a bit
		void BitTog(const uchar& bit)
		{
			assert(bit < this->GetMaxBit());
			val ^= (1 << bit);
		}
		// Sets a value
		void BitBool(const uchar& bit, const bool b)
		{
			assert(bit < this->GetMaxBit());
			if (b)
			{
				val |= (1 << bit);
			}
			else
			{
				val &= ~(1 << bit);
			}
		}
		//Returns the result of a bit
		bool Bit(const uchar bit) const
		{
			assert(bit < this->GetMaxBit());
			return (val & (1 << bit)) != 0;
		}


		//Turns on the bits corresponding to this flag
		void FlagOn(const type& flag)
		{
			__not_implemented(BitFlag::FlagOn);
		}
		//Turns off the bits corresponding to this flag
		void FlagOff(const type& flag)
		{
			__not_implemented(BitFlag::FlagOff);
		}
		//Toggles the bits corresonding to this flag
		void FlagTog()
		{
			__not_implemented(BitFlag::FlagTog);
		}
		//Sets a value
		void FlagBool(const type& flag, const bool b)
		{
			__not_implemented(BitFlag::FlagBool);
		}
		//Returns the result of the corresponding bits
		bool Flag(const type& flag) const
		{
			__not_implemented(BitFlag::Flag);
			return false;// (flag & type) != 0;
		}

		// Zeros out the whole flag
		void ZeroOut(void)
		{
			val = 0;
		}
	};

	typedef BitFlag<u08> Flag08;
	typedef BitFlag<u16> Flag16;
	typedef BitFlag<u32> Flag32;
	typedef BitFlag<u64> Flag64;
}
