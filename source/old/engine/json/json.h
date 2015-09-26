#pragma once
#include "engine/util/util.h"
#include "engine/asdf.h"
#include "rapidjson/document.h"
#include "engine/math/math.h"
#include "engine/util/color.h"

namespace Engine
{
	namespace Json
	{
		class JSON
		{
		public:
			JSON();
			JSON(const rapidjson::Document& doc);
			JSON(const rapidjson::Value& val);
			~JSON();
			JSON(const JSON& that);						//Copy Ctor
			JSON& operator= (const JSON& that);			//Assignment Operator

			void Parse(const std::string& input);
			const bool TryParse(const std::string& input);
			const std::string Read(void) const;

			const bool HasMember(const std::string& member) const;
			const bool IsNull(void) const;
			const bool IsString(void) const;
			const bool IsNumber(void) const;
			const bool IsBool(void) const;
			const bool IsArray(void) const;
			const bool IsObject(void) const;

			const int GetInt(void) const;
			const bool GetBool(void) const;
			const double GetDouble(void) const;
			const std::string GetString(void) const;
			const Math::vec2 GetVec2(void) const;
			const Math::vec3 GetVec3(void) const;
			const CLR::TColor GetColor(void) const;
			const Math::CMatrix2D GetMatrix(void) const;
			//rect
			template<typename type>
			const std::vector<type> GetArray() const;

			const JSON operator[](const std::string& member) const;

		private:
			rapidjson::Document rdoc;
		};
	}
}