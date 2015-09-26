#include "engine/json/json.h"
#include "rapidjson/prettywriter.h"

namespace Engine
{
	namespace Json
	{
		//https://miloyip.github.io/rapidjson/index.html
		//http://programmaticallyspeaking.blogspot.se/2012/09/infer-return-type-for-templated.html

		//TODO finish the templates
		//template<>
		//const JSON JSON::operator[]<JSON>(const std::string& member) const
		//{
		//	return JSON(this->rdoc[member.c_str()]);
		//}
		//template<>
		//const int JSON::operator[]<int>(const std::string& member) const
		//{
		//	return this->rdoc[member.c_str()].GetInt();
		//}
		//template<>
		//const bool JSON::operator[]<bool>(const std::string& member) const
		//{
		//	return this->rdoc[member.c_str()].GetBool();
		//}
		const JSON JSON::operator[](const std::string& member) const
		{
			return JSON(this->rdoc[member.c_str()]);
		}

		template<>
		const std::vector<JSON> JSON::GetArray<JSON>(void) const
		{
			assert(rdoc.IsArray());
			std::vector<JSON> vec;
			vec.reserve(rdoc.Size());
			for (int i = 0; i < (int)rdoc.Size(); ++i)
				vec.push_back(JSON(rdoc[i]));
			return vec;
		}

		template<>
		const std::vector<int> JSON::GetArray<int>(void) const
		{
			assert(rdoc.IsArray());
			std::vector<int> vec;
			vec.reserve(rdoc.Size());
			for (int i = 0; i < (int)rdoc.Size(); ++i)
				vec.push_back(rdoc[i].GetInt());
			return vec;
		}

		template<>
		const std::vector<double> JSON::GetArray<double>(void) const
		{
			assert(rdoc.IsArray());
			std::vector<double> vec;
			vec.reserve(rdoc.Size());
			for (int i = 0; i < (int)rdoc.Size(); ++i)
				vec.push_back(rdoc[i].GetDouble());
			return vec;
		}

		template<>
		const std::vector<bool> JSON::GetArray<bool>(void) const
		{
			assert(rdoc.IsArray());
			std::vector<bool> vec;
			vec.reserve(rdoc.Size());
			for (int i = 0; i < (int)rdoc.Size(); ++i)
				vec.push_back(rdoc[i].GetBool());
			return vec;
		}

		template<>
		const std::vector<std::string> JSON::GetArray<std::string>(void) const
		{
			assert(rdoc.IsArray());
			std::vector<std::string> vec;
			vec.reserve(rdoc.Size());
			for (int i = 0; i < (int)rdoc.Size(); ++i)
				vec.push_back(rdoc[i].GetString());
			return vec;
		}

		JSON::JSON(void)
		{
		}
		JSON::JSON(const rapidjson::Document& doc)
		{
			//Perform deep copy of dom tree
			rapidjson::Document::AllocatorType& a = this->rdoc.GetAllocator();
			this->rdoc.CopyFrom(doc, a);
		}
		JSON::JSON(const rapidjson::Value& val)
		{
			//Perform deep copy of dom tree
			rapidjson::Document::AllocatorType& a = this->rdoc.GetAllocator();
			this->rdoc.CopyFrom(val, a);
		}
		JSON::~JSON(void)
		{
		}
		JSON::JSON(const JSON& that)
		{
			*this = that;
		}
		JSON& JSON::operator= (const JSON& that)
		{
			//Check for self assignment
			if (this == &that)
				return *this;
			//Perform deep copy of dom tree
			rapidjson::Document::AllocatorType& a = this->rdoc.GetAllocator();
			this->rdoc.CopyFrom(that.rdoc, a);
			return *this;
		}

		void JSON::Parse(const std::string& input)
		{
			rdoc.Parse(input.c_str());
			assert(!rdoc.HasParseError());
		}
		const bool JSON::TryParse(const std::string& input)
		{
			rdoc.Parse(input.c_str());
			return rdoc.HasParseError();
		}
		const std::string JSON::Read(void) const
		{
			rapidjson::StringBuffer sbuff;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sbuff);
			this->rdoc.Accept(writer);
			return sbuff.GetString();
		}

		const bool JSON::HasMember(const std::string& member) const
		{
			return rdoc.HasMember(member.c_str());
		}

		const bool JSON::IsNull(void) const
		{
			return rdoc.IsNull();
		}
		const bool JSON::IsString(void) const
		{
			return rdoc.IsString();
		}
		const bool JSON::IsNumber(void) const
		{
			return rdoc.IsNumber();
		}
		const bool JSON::IsBool(void) const
		{
			return rdoc.IsBool();
		}
		const bool JSON::IsArray(void) const
		{
			return rdoc.IsArray();
		}
		const bool JSON::IsObject(void) const
		{
			return rdoc.IsObject();
		}

		const int JSON::GetInt(void) const
		{
			return rdoc.GetInt();
		}
		const bool JSON::GetBool(void) const
		{
			return rdoc.GetBool();
		}
		const double JSON::GetDouble(void) const
		{
			return rdoc.GetDouble();
		}
		const std::string JSON::GetString(void) const
		{
			return rdoc.GetString();
		}
		const Math::vec2 JSON::GetVec2(void) const
		{
			if (rdoc.HasMember("x")) // X & Y
				return Math::vec2(rdoc["x"].GetDouble(), rdoc["y"].GetDouble());
			else if (rdoc.HasMember("w"))// W & H
				return Math::vec2(rdoc["w"].GetDouble(), rdoc["h"].GetDouble());
			assert(false);
			return Math::vec2(); //this is for 'not all control paths return a value' warning
		}
		const Math::vec3 JSON::GetVec3(void) const
		{
			if (rdoc.HasMember("x")) // X & Y & Z
				return Math::vec3(rdoc["x"].GetDouble(), rdoc["y"].GetDouble(), rdoc["z"].GetDouble());
			else if (rdoc.HasMember("w")) // W & H & D
				return Math::vec3(rdoc["w"].GetDouble(), rdoc["h"].GetDouble(), rdoc["d"].GetDouble());
			assert(false);
			return Math::vec3(); //this is for 'not all control paths return a value' warning
		}
		const CLR::TColor JSON::GetColor(void) const
		{
			if (rdoc.HasMember("a"))
			{
				return CLR::TColor(rdoc["r"].GetDouble(),
					rdoc["g"].GetDouble(),
					rdoc["b"].GetDouble(),
					rdoc["a"].GetDouble());
			}
			else
			{
				return CLR::TColor(rdoc["r"].GetDouble(),
					rdoc["g"].GetDouble(),
					rdoc["b"].GetDouble(),
					1.0f);
			}
		}
		const Math::CMatrix2D JSON::GetMatrix(void) const
		{
			CMatrix2D mat;
			const JSON jpos = JSON(rdoc["position"]);
			const JSON jscale = JSON(rdoc["scale"]);
			const JSON jrot = JSON(rdoc["rotation"]);

			mat.SetPosition(jpos.GetVec2());
			mat.SetScale(jscale.GetVec2());
			mat.SetRotationZ((float)jrot.GetDouble());

			return mat;
		}
	}
}