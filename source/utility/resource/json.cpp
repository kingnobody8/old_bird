#include "json.h"
#include "rapidjson/prettywriter.h"

/*https://miloyip.github.io/rapidjson/index.html*/
//this isn't used but may be useful in the future for making the c++ syntax of json work like javascript syntax
//http://programmaticallyspeaking.blogspot.se/2012/09/infer-return-type-for-templated.html

namespace util
{
	namespace resource
	{
		const std::string JSON::Read(void) const
		{
			//NOTE (daniel) - if you get the RAPIDJSON_ASSERT(m->name.IsString())
			//it's probably because you created a temporary json object and used the wrong allocator
			//when the temp object was destroyed all the stuff you allocated with it disappeared too
			//you should always use the root json instance's allocator

			rapidjson::StringBuffer sbuff;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sbuff);
			Accept(writer);
			return sbuff.GetString();
		}

		//GETS

		const Time JSON::GetTime() const
		{
			return Time(GetUint64());
		}
		const types::Color JSON::GetColor() const
		{
			if (HasMember("a"))
			{
				return types::Color(
					(*this)["r"].GetDouble(),
					(*this)["g"].GetDouble(),
					(*this)["b"].GetDouble(),
					(*this)["a"].GetDouble());
			}
			else
			{
				return types::Color(
					(*this)["r"].GetDouble(),
					(*this)["g"].GetDouble(),
					(*this)["b"].GetDouble(),
					1.0f);
			}
		}
		const math::vec2 JSON::GetVec2() const
		{
			if (HasMember("x")) // X & Y
				return math::vec2((*this)["x"].GetDouble(), (*this)["y"].GetDouble());
			else if (HasMember("w"))// W & H
				return math::vec2((*this)["w"].GetDouble(), (*this)["h"].GetDouble());

			assert(false);
			return math::vec2(); //this is for 'not all control paths return a value' warning
		}
		const math::vec3 JSON::GetVec3() const
		{
			if (HasMember("x")) // X & Y & Z
				return math::vec3((*this)["x"].GetDouble(), (*this)["y"].GetDouble(), (*this)["z"].GetDouble());
			else if (HasMember("w")) // W & H & D
				return math::vec3((*this)["w"].GetDouble(), (*this)["h"].GetDouble(), (*this)["d"].GetDouble());

			assert(false);
			return math::vec3(); //this is for 'not all control paths return a value' warning
		}
		const math::vec4 JSON::GetVec4() const
		{
			return math::vec4((*this)["x"].GetDouble(), (*this)["y"].GetDouble(), (*this)["z"].GetDouble(), (*this)["w"].GetDouble());
		}
		const b2AABB JSON::GetAabb() const
		{
			const math::vec2 min = static_cast<const JSON>((*this)["min"]).GetVec2();
			const math::vec2 max = static_cast<const JSON>((*this)["max"]).GetVec2();
			b2AABB ret;
			ret.lowerBound = b2Vec2(min.x, min.y);
			ret.upperBound = b2Vec2(max.x, max.y);
			return ret;
		}
		const math::matrix JSON::GetMatrix() const
		{
			__not_implemented(JSON::GetMatrix);

			math::matrix mat;
			//const math::vec3 pos = static_cast<const JSON>((*this)["position"]).GetVec3();
			//const math::vec3 scale = static_cast<const JSON>((*this)["scale"]).GetVec3();
			//const math::vec3 rot = static_cast<const JSON>((*this)["rotation"]).GetVec3();
			//
			//mat.SetPosition(pos);
			//mat.SetScale(scale);
			//mat.SetRotation(rot);

			return mat;
		}

		//ARRAY GETS

		template<>
		const std::vector<JSON> JSON::GetArray<JSON>(void) const
		{
			assert(IsArray());
			std::vector<JSON> vec;
			vec.reserve(Size());
			for (int i = 0; i < (int)Size(); ++i)
				vec.push_back((JSON)((*this)[i]));
			return vec;
		}
		template<>
		const std::vector<bool> JSON::GetArray<bool>(void) const
		{
			assert(IsArray());
			std::vector<bool> vec;
			vec.reserve(Size());
			for (int i = 0; i < (int)Size(); ++i)
				vec.push_back((*this)[i].GetBool());
			return vec;
		}
		template<>
		const std::vector<int> JSON::GetArray<int>(void) const
		{
			assert(IsArray());
			std::vector<int> vec;
			vec.reserve(Size());
			for (int i = 0; i < (int)Size(); ++i)
				vec.push_back((*this)[i].GetInt());
			return vec;
		}
		template<>
		const std::vector<double> JSON::GetArray<double>(void) const
		{
			assert(IsArray());
			std::vector<double> vec;
			vec.reserve(Size());
			for (int i = 0; i < (int)Size(); ++i)
				vec.push_back((*this)[i].GetDouble());
			return vec;
		}
		template<>
		const std::vector<std::string> JSON::GetArray<std::string>(void) const
		{
			assert(IsArray());
			std::vector<std::string> vec;
			vec.reserve(Size());
			for (int i = 0; i < (int)Size(); ++i)
				vec.push_back((*this)[i].GetString());
			return vec;
		}
		template<>
		const std::vector<Time> JSON::GetArray<Time>(void) const
		{
			assert(IsArray());
			std::vector<Time> vec;
			vec.reserve(Size());
			for (int i = 0; i < (int)Size(); ++i)
				vec.push_back(((JSON)((*this)[i])).GetTime());
			return vec;
		}
		template<>
		const std::vector<types::Color> JSON::GetArray<types::Color>(void) const
		{
			assert(IsArray());
			std::vector<types::Color> vec;
			vec.reserve(Size());
			for (int i = 0; i < (int)Size(); ++i)
				vec.push_back(((JSON)((*this)[i])).GetColor());
			return vec;
		}
		template<>
		const std::vector<math::vec2> JSON::GetArray<math::vec2>(void) const
		{
			assert(IsArray());
			std::vector<math::vec2> vec;
			vec.reserve(Size());
			for (int i = 0; i < (int)Size(); ++i)
				vec.push_back(((JSON)((*this)[i])).GetVec2());
			return vec;
		}
		template<>
		const std::vector<math::vec3> JSON::GetArray<math::vec3>(void) const
		{
			assert(IsArray());
			std::vector<math::vec3> vec;
			vec.reserve(Size());
			for (int i = 0; i < (int)Size(); ++i)
				vec.push_back(((JSON)((*this)[i])).GetVec3());
			return vec;
		}
		template<>
		const std::vector<math::vec4> JSON::GetArray<math::vec4>(void) const
		{
			assert(IsArray());
			std::vector<math::vec4> vec;
			vec.reserve(Size());
			for (int i = 0; i < (int)Size(); ++i)
				vec.push_back(((JSON)((*this)[i])).GetVec4());
			return vec;
		}
		template<>
		const std::vector<math::matrix> JSON::GetArray<math::matrix>(void) const
		{
			assert(IsArray());
			std::vector<math::matrix> vec;
			vec.reserve(Size());
			for (int i = 0; i < (int)Size(); ++i)
				vec.push_back(((JSON)((*this)[i])).GetMatrix());
			return vec;
		}

		//SETS

		void JSON::SetTime(const Time& val, rapidjson::Document::AllocatorType& allocator)
		{
			SetUint64(val.Milli());
		}
		void JSON::SetColor(const types::Color& val, rapidjson::Document::AllocatorType& allocator)
		{
			SetObject();
			AddMember("r", val.r, allocator);
			AddMember("g", val.g, allocator);
			AddMember("b", val.b, allocator);
			AddMember("a", val.a, allocator);
		}
		void JSON::SetVec2(const math::vec2& val, rapidjson::Document::AllocatorType& allocator)
		{
			SetObject();
			rapidjson::Value x, y;
			x.SetDouble(val.x);
			y.SetDouble(val.y);
			AddMember("x", x.Move(), allocator);
			AddMember("y", y.Move(), allocator);
		}
		void JSON::SetVec3(const math::vec3& val, rapidjson::Document::AllocatorType& allocator)
		{
			SetObject();
			AddMember("x", val.x, allocator);
			AddMember("y", val.y, allocator);
			AddMember("z", val.z, allocator);
		}
		void JSON::SetVec4(const math::vec4& val, rapidjson::Document::AllocatorType& allocator)
		{
			SetObject();
			AddMember("x", val.x, allocator);
			AddMember("y", val.y, allocator);
			AddMember("z", val.z, allocator);
			AddMember("w", val.w, allocator);
		}
		void JSON::SetAabb(const b2AABB& val, rapidjson::Document::AllocatorType& allocator)
		{
			math::vec2 vmin, vmax;
			vmin = math::vec2(val.lowerBound.x, val.lowerBound.y);
			vmax = math::vec2(val.upperBound.x, val.upperBound.y);

			JSON min, max;
			min.SetVec2(vmin, allocator);
			max.SetVec2(vmax, allocator);

			SetObject();
			AddMember("min", min, allocator);
			AddMember("max", max, allocator);
		}
		void JSON::SetMatrix(const math::matrix& val, rapidjson::Document::AllocatorType& allocator)
		{
			__not_implemented(JSON::SetMatrix);
			//JSON vpos, vscale, vrot;
			//vpos.SetVec3(val.GetPosition(), allocator);
			//vscale.SetVec3(val.GetScale(), allocator);
			//vrot.SetVec3(val.GetRotation(), allocator);
			//
			//SetObject();
			//AddMember("position", vpos, allocator);
			//AddMember("scale", vscale, allocator);
			//AddMember("rotation", vrot, allocator);
		}

		// ARRAY SETS

		template<>
		const void JSON::WriteArray(std::vector<Time> val, rapidjson::Document::AllocatorType& allocator)
		{
			SetArray();
			for (size_t i = 0; i < val.size(); ++i)
			{
				JSON j;
				j.SetTime(val[i], allocator);
				PushBack(j, allocator);
			}
		}
		template<>
		const void JSON::WriteArray(std::vector<types::Color> val, rapidjson::Document::AllocatorType& allocator)
		{
			SetArray();
			for (size_t i = 0; i < val.size(); ++i)
			{
				JSON j;
				j.SetColor(val[i], allocator);
				PushBack(j, allocator);
			}
		}
		template<>
		const void JSON::WriteArray(std::vector<math::vec2> val, rapidjson::Document::AllocatorType& allocator)
		{
			SetArray();
			for (size_t i = 0; i < val.size(); ++i)
			{
				JSON j;
				j.SetVec2(val[i], allocator);
				PushBack(j, allocator);
			}
		}
		template<>
		const void JSON::WriteArray(std::vector<math::vec3> val, rapidjson::Document::AllocatorType& allocator)
		{
			SetArray();
			for (size_t i = 0; i < val.size(); ++i)
			{
				JSON j;
				j.SetVec3(val[i], allocator);
				PushBack(j, allocator);
			}
		}
		template<>
		const void JSON::WriteArray(std::vector<math::vec4> val, rapidjson::Document::AllocatorType& allocator)
		{
			SetArray();
			for (size_t i = 0; i < val.size(); ++i)
			{
				JSON j;
				j.SetVec4(val[i], allocator);
				PushBack(j, allocator);
			}
		}
		template<>
		const void JSON::WriteArray(std::vector<math::matrix> val, rapidjson::Document::AllocatorType& allocator)
		{
			SetArray();
			for (size_t i = 0; i < val.size(); ++i)
			{
				JSON j;
				j.SetMatrix(val[i], allocator);
				PushBack(j, allocator);
			}
		}
	}
}