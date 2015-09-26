#include "color.h"

namespace util
{
	Color::Color(const Color& that)
	{
		*this = that;
	}
	Color& Color::operator = (const Color& that)
	{
		this->type = that.type;
		return *this;
	}

	const SDL_Color Color::SDL(void) const
	{
		SDL_Color ret;
		ret.a = Uint8(Clamp(this->a * 255, 0, 255));
		ret.r = Uint8(Clamp(this->r * 255, 0, 255));
		ret.g = Uint8(Clamp(this->g * 255, 0, 255));
		ret.b = Uint8(Clamp(this->b * 255, 0, 255));
		return ret;
	}
	void Color::SDL(const SDL_Color clr)
	{
		this->type = vec4(clr.r / 255.0f, clr.g / 255.0f, clr.b / 255.0f, clr.a / 255.0f);
	}
	const vec4 Color::HSV(void) const
	{
		vec4 ret;
		ret.a = this->a;

		float fMin = Min(this->r, Min(this->g, this->b));		//Min. value of RGB
		float fMax = Max(this->r, Max(this->g, this->b));		//Max. value of RGB
		float fDelta = fMax - fMin;								//Delta RGB value 

		ret.val = fMax;											//V component

		if (fDelta == 0)										//This is a gray, no chroma...
		{
			ret.hue = 0;										//HSV results from 0 to 1
			ret.sat = 0;
		}
		else													//Chromatic data...
		{
			ret.sat = fDelta / fMax;							//S component

			float del_R = (((fMax - r) / 6) + (fDelta / 2)) / fDelta;
			float del_G = (((fMax - g) / 6) + (fDelta / 2)) / fDelta;
			float del_B = (((fMax - b) / 6) + (fDelta / 2)) / fDelta;


			if (this->r == fMax)
				ret.hue = del_B - del_G;
			else if (this->g == fMax)
				ret.hue = (1.0f / 3.0f) + del_R - del_B;
			else if (this->b == fMax)
				ret.hue = (2.0f / 3.0f) + del_G - del_R;

			if (ret.hue < 0)
				ret.hue += 1.0f;
			if (ret.hue > 1)
				ret.hue -= 1.0f;
		}

		return ret;
	}
	void Color::HSV(const vec4& fHSV)
	{
		int i;
		float f, p, q, t;

		this->a = fHSV.a; //Alpha

		if (fHSV.sat == 0)
		{
			// achromatic (grey)
			this->r = this->g = this->b = fHSV.val;
			return;
		}

		float h = fHSV.hue * 360.0f;
		h /= 60;			// sector 0 to 5
		i = int(floor(h));
		f = h - i;			// factorial part of h
		p = fHSV.val * (1.0f - fHSV.sat);
		q = fHSV.val * (1.0f - fHSV.sat * f);
		t = fHSV.val * (1.0f - fHSV.sat * (1.0f - f));
		switch (i)
		{
		case 0:
			this->r = fHSV.val;
			this->g = t;
			this->b = p;
			break;
		case 1:
			this->r = q;
			this->g = fHSV.val;
			this->b = p;
			break;
		case 2:
			this->r = p;
			this->g = fHSV.val;
			this->b = t;
			break;
		case 3:
			this->r = p;
			this->g = q;
			this->b = fHSV.val;
			break;
		case 4:
			this->r = t;
			this->g = p;
			this->b = fHSV.val;
			break;
		default:
			this->r = fHSV.val;
			this->g = p;
			this->b = q;
			break;
		}
	}

	// STATIC
	STATIC Color Color::Blend(const Color& one, const Color& two, float lerp) //Blends two colors, lerp value is between 0.0f and 1.0f, 0.0f == all one clr, 1.0f == all two clr
	{
		//Clamp lerp value
		lerp = Clamp(lerp, 0.0f, 1.0f);
		//Calculate return color
		Type4<float> diff = two.type - one.type;
		Color ret = Color(one.type + diff * lerp);

		//Clamp channels
		ret.r = Clamp(ret.r, 0.0f, 1.0f);
		ret.g = Clamp(ret.g, 0.0f, 1.0f);
		ret.b = Clamp(ret.b, 0.0f, 1.0f);
		ret.a = Clamp(ret.a, 0.0f, 1.0f);
		return ret;
	}
	STATIC Color Color::Mod(const Color& clr, const vec4& modder)
	{
		Color ret = clr;
		ret.type *= modder;
		return ret;
	}

	//Full
	STATIC const Color Color::WHITE							= Color(1.0f, 1.0f, 1.0f);
	STATIC const Color Color::BLACK							= Color(0.0f, 0.0f, 0.0f);
	STATIC const Color Color::CLEAR							= Color(0.0f, 0.0f, 0.0f, 0.0f);
	STATIC const Color Color::GREY							= Color(0.5f, 0.5f, 0.5f);
	//Primary
	STATIC const Color Color::RED							= Color(1.0f, 0.0f, 0.0f);
	STATIC const Color Color::GREEN							= Color(0.0f, 1.0f, 0.0f);
	STATIC const Color Color::BLUE							= Color(0.0f, 0.0f, 1.0f);
	//Secondary
	STATIC const Color Color::YELLOW						= Color(1.0f, 1.0f, 0.0f);
	STATIC const Color Color::MAGENTA						= Color(1.0f, 0.0f, 1.0f);
	STATIC const Color Color::CYAN							= Color(0.0f, 1.0f, 1.0f);
	//Tertiary
	STATIC const Color Color::ROSE							= Color(1.0f, 0.0f, 0.5f);
	STATIC const Color Color::ORANGE						= Color(1.0f, 5.0f, 0.0f);
	STATIC const Color Color::CHARTREUSE					= Color(0.5f, 1.0f, 0.0f);
	STATIC const Color Color::SPRING_GREEN					= Color(0.0f, 1.0f, 0.5f);
	STATIC const Color Color::AZURE							= Color(0.0f, 0.5f, 1.0f);
	STATIC const Color Color::VIOLET						= Color(0.5f, 0.0f, 1.0f);

	STATIC const Color Color::other::AMARANTH_DEEP_PURPLE	= Color(0.67f, 0.15f, 0.31f);
	STATIC const Color Color::other::AMAZON					= Color(0.23f, 0.48f, 0.34f);
	STATIC const Color Color::other::ANDROID_GREEN			= Color(0.64f, 0.78f, 0.22f);
	STATIC const Color Color::other::ANTIQUE_RUBY			= Color(0.52f, 0.11f, 0.18f);
	STATIC const Color Color::other::ARSENIC				= Color(0.23f, 0.27f, 0.29f);
	STATIC const Color Color::other::ASPARAGUS				= Color(0.53f, 0.66f, 0.42f);
	STATIC const Color Color::other::AUBURN					= Color(0.65f, 0.16f, 0.16f);
	STATIC const Color Color::other::BAZAAR					= Color(0.60f, 0.47f, 0.48f);
	STATIC const Color Color::other::BLACK_OLIVE			= Color(0.23f, 0.24f, 0.21f);
	STATIC const Color Color::other::BLUE_SAPPHIRE			= Color(0.07f, 0.38f, 0.50f);
	STATIC const Color Color::other::BROWN					= Color(0.59f, 0.29f, 0.00f);

}