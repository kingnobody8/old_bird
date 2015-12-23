#pragma once
#include "rapidjson/document.h"
#include "math/matrix_2d.h"
#include "math/type2.h"
#include "math/type3.h"
#include "math/type4.h"
#include "util_time.h"
#include "color.h"
#include "macro.h"
#include <vector>
#include "box2d/box2d.h"

namespace util
{
	//this class extends the functionality of rapidjson::Document for use with our custom types
	class JSON : public rapidjson::Document
	{
	public:
		JSON() : ptr_alloc(null){}
		JSON(const JSON& that) : ptr_alloc(null) { *this = that; }
		JSON(const rapidjson::Document& that) : ptr_alloc(null) { this->CopyFrom(that, GetAllocator()); }
		JSON(const rapidjson::Value& that) : ptr_alloc(null) { this->CopyFrom(that, GetAllocator()); }
		JSON& operator= (const JSON& that)
		{
			if (this == &that) //self assignment check
				return *this;
			if (that.ptr_alloc == null)
				this->CopyFrom(that, GetAllocator());
			else
				this->CopyFrom(that, *(that.ptr_alloc));
			this->ptr_alloc = that.ptr_alloc;
			return *this;
		}

		const std::string Read() const;

		const Time				GetTime() const;
		const Color				GetColor() const;
		const math::vec2		GetVec2() const;
		const math::vec3		GetVec3() const;
		const math::vec4		GetVec4() const;
		const b2AABB			GetAabb() const;
		const math::Matrix2D	GetMatrix() const;
		template<typename type>
		const std::vector<type> GetArray(void) const;

		void SetTime(const Time& val, rapidjson::Document::AllocatorType& allocator);
		void SetColor(const Color& val, rapidjson::Document::AllocatorType& allocator);
		void SetVec2(const math::vec2& val, rapidjson::Document::AllocatorType& allocator);
		void SetVec3(const math::vec3& val, rapidjson::Document::AllocatorType& allocator);
		void SetVec4(const math::vec4& val, rapidjson::Document::AllocatorType& allocator);
		void SetAabb(const b2AABB& val, rapidjson::Document::AllocatorType& allocator);
		void SetMatrix(const math::Matrix2D& val, rapidjson::Document::AllocatorType& allocator);
		template<typename type>
		const void WriteArray(std::vector<type> val, rapidjson::Document::AllocatorType& allocator); //this is called 'write' because 'setarray' is already used by rapidjson::value

		rapidjson::Document::AllocatorType* ptr_alloc;
	};

	template<typename type>
	const void JSON::WriteArray(std::vector<type> val, rapidjson::Document::AllocatorType& allocator)
	{
		SetArray();
		for (size_t i = 0; i < val.size(); ++i)
			PushBack(val[i], allocator);
	}
	template<>
	const void JSON::WriteArray(std::vector<Time> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<Color> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<math::vec2> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<math::vec3> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<math::vec4> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<math::Matrix2D> val, rapidjson::Document::AllocatorType& allocator);

	/*
	EXAMPLES

	//READING
	JSON json;
	json.Parse("{\"name\": \"play\"}");
	bool success = !json.HasParseError();

	//WRITING - allways use the root json instance's allocator
	JSON temp;
	temp.SetObject();
	temp.AddMember("key1", "value1", json.GetAllocator());
	temp.AddMember("key2", "value2", json.GetAllocator());
	json.SetObject();
	json.Set(temp, json.GetAllocator());

	//ARRAY - allways use the root json instance's allocator
	json.SetArray();
	for (size_t i = 0; i < val.size(); ++i)
			json.PushBack(val[i], json.GetAllocator);
	*/
}