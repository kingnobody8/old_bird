#pragma once
#include "math/type4.h"
#include "func.h"
#include "sdl.h"
using namespace util::math;

//https://en.wikipedia.org/wiki/Lists_of_colors

namespace util
{
	struct Color
	{
	public:
		union
		{
			struct { vec4 type; };							//Float4
			struct { float r; float g; float b; float a; };	//Easy access as color values
		};
	public:
		Color(void){}
		Color(const float& r, const float& g, const float& b, const float& a = 1.0f) : type(r, g, b, a){}
		Color(const vec4& type) : type(type) {}

		Color(const Color& that);
		Color& operator= (const Color& that);

		const SDL_Color SDL(void) const;
		void SDL(const SDL_Color clr);
		const vec4 HSV(void) const;
		void HSV(const vec4& fHSV);

		// STATIC
		//Blends two colors, lerp value is between 0.0f and 1.0f, 0.0f == all one clr, 1.0f == all two clr
		static Color Blend(const Color& one, const Color& two, float lerp) ;
		//Modifies a color by each value (rgba) or the modder
		static Color Mod(const Color& clr, const vec4& modder);

		//Full
		static const Color WHITE;
		static const Color BLACK;
		static const Color CLEAR;
		static const Color GREY;
		//Primary
		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
		//Secondary
		static const Color YELLOW;
		static const Color MAGENTA;
		static const Color CYAN;
		//Tertiary
		static const Color ROSE;
		static const Color ORANGE;
		static const Color CHARTREUSE;
		static const Color SPRING_GREEN;
		static const Color AZURE;
		static const Color VIOLET;
		//Quaternary
		//Browns
		//Other
		struct other
		{
			//static const Color ACID_GREEN;
			//static const Color AERO;
			//static const Color AERO_BLUE;
			//static const Color AFRICAN_VIOLET;
			//static const Color AIR_FORCE_BLUE_RAF;
			//static const Color AIR_FORE_BLUE_USAF;
			//static const Color AIR_SUPERIORITY_BLUE;
			//static const Color ALABAMA_CRIMSON;
			//static const Color ALICE_BLUE;
			//static const Color ALIZARIN_CRIMSON;
			//static const Color ALLOY_ORANGE;
			//static const Color ALMOND;
			//static const Color AMARANTH;
			static const Color AMARANTH_DEEP_PURPLE;
			//static const Color AMARANTH_PINK;
			//static const Color AMARANTH_PURPLE;
			//static const Color AMARANTH_RED;
			static const Color AMAZON;
			//static const Color AMBER;
			//static const Color AMBER_SAE_ECE;
			//static const Color AMERICAN_ROSE;
			//static const Color AMETHYST;
			static const Color ANDROID_GREEN;
			//static const Color ANTI_FLASH_WHITE;
			//static const Color ANTIQUE_BRASS;
			//static const Color ANTIQUE_BRONZE;
			//static const Color ANTIQUE_FUCHSIA;
			static const Color ANTIQUE_RUBY;
			//static const Color ANTIQUE_WHITE;
			//static const Color AO;
			//static const Color APPLE_GREEN;
			//static const Color APRICOT;
			//static const Color AQUA;
			//static const Color AQUAMARINE;
			//static const Color ARTIC_LIME;
			//static const Color ARMY_GREEN;
			static const Color ARSENIC;
			//static const Color ARTICHOKE;
			//static const Color ARYLIDE_YELLOW;
			//static const Color ASH_GREY;
			static const Color ASPARAGUS;
			//static const Color ATOMIC_TANGERINE;
			static const Color AUBURN;
			//static const Color AUREOLIN;
			//static const Color AUROMETALSAURUS;
			//static const Color AVOCADO;
			//static const Color AZURE_MIST;
			//static const Color BABY_BLUE;
			//static const Color BABY_BLUE_EYES;
			//static const Color BABY_PINK;
			//static const Color BABY_POWDER;
			//static const Color BAKER_MILLER_PINK;
			//static const Color BALL_BLUE;
			//static const Color BANANA_MANIA;
			//static const Color BANANA_YELLOW;
			//static const Color BANGLADESH_GREEN;
			//static const Color BARBIE_PINK;
			//static const Color BARN_RED;
			//static const Color BATTLESHIP_GREY;
			static const Color BAZAAR;
			//static const Color BEAU_BLUE;
			//static const Color BEAVER;
			//static const Color BEIGE;
			//static const Color BDAZZLED_BLUE;
			//static const Color BIG_DIP_O_RUBY;
			//static const Color BISQUE;
			//static const Color BISTRE;
			//static const Color BISTRE_BROWN;
			//static const Color BITTER_LEMON;
			//static const Color BITTER_LIME;
			//static const Color BITTERSWEET;
			//static const Color BITTERSWEET_SHIMMER;
			//static const Color BLACK_BEAN;
			//static const Color BLACK_LEATHER_JACKET;
			static const Color BLACK_OLIVE;
			//static const Color BLANCHED_ALMOND;
			//static const Color BLAST_OFF_BRONZE;
			//static const Color BLUE_DE_FRANCE;
			//static const Color BLIZZARD_BLUE;
			//static const Color BLOND;
			//static const Color BLUE_CRAYOLA;
			//static const Color BLUE_MUNSELL;
			//static const Color BLUE_NCS;
			//static const Color BLUE_PANTONE;
			//static const Color BLUE_PIGMENT;
			//static const Color BLUE_RYB;
			//static const Color BLUE_BELL;
			//static const Color BLUE_GRAY;
			//static const Color BLUE_GREEN;
			//static const Color BLUE_LAGOON;
			//static const Color BLUE_MAGENTA_VIOLET;
			static const Color BLUE_SAPPHIRE;
			//static const Color BLUE_VIOLET;
			//static const Color BLUE_YONDER;
			//static const Color BLUEBERRY;
			//static const Color BLUEBONNET;
			//static const Color BLUSH;
			//static const Color BOLE;
			//static const Color BONDI_BLUE;
			//static const Color BONE;
			//static const Color BOSTON_UNIVERSITY_RED;
			//static const Color BOTTLE_GREEN;
			//static const Color BOYSENBERRY;
			//static const Color BRANDEIS_BLUE;
			//static const Color BRASS;
			//static const Color BRICK_RED;
			//static const Color BRIGHT_CERULEAN;
			//static const Color BRIGHT_GREEN;
			//static const Color BRIGHT_LAVENDER;
			//static const Color BRIGHT_LILAC;
			//static const Color BRIGHT_MAROON;
			//static const Color BRIGHT_NAVY_BLUE;
			//static const Color BRIGHT_PINK;
			//static const Color BRIGHT_TURQUOISE;
			//static const Color BRIGHT_UBE;
			//static const Color BRILLIANT_AZURE;
			//static const Color BRILLIANT_LAVENDER;
			//static const Color BRILLIANT_ROSE;
			//static const Color BRILLIANT_PINK;
			//static const Color BRITISH_RACING_GREEN;
			//static const Color BRONZE;
			//static const Color BRONZE_YELLOW;
			static const Color BROWN;
			//static const Color BROWN_WEB;
			//static const Color BROWN_NOSE;
			//static const Color BROWN_YELLOW;
			//static const Color BRUNSWICK_GREEN;
			//static const Color BUBBLE_GUM;
			//static const Color BUGGLES;
			//static const Color BUD_GREEN;
			//static const Color BUFF;
			//static const Color BULGARIAN_ROSE;
			//static const Color BURGUNDY;
			//static const Color BURLYWOOD;
			//static const Color BURNT_ORANGE;
			//static const Color BURNT_SIENNA;
			//static const Color BURNT_UMBER;
			//static const Color BYZANTINE;
			//static const Color BYANTIUM;
		};
	};
}
