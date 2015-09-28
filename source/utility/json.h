#pragma once
#include "rapidjson/document.h"
#include "math/matrix_2d.h"
#include "util_time.h"
#include "shape/aabb.h"
#include "color.h"
#include <vector>
#include "macro.h"

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
		const shape::AABB		GetAabb() const;
		const math::Matrix2D	GetMatrix() const;
		template<typename type>
		const std::vector<type> GetArray(void) const;

		void SetTime(const Time& val, rapidjson::Document::AllocatorType& allocator);
		void SetColor(const Color& val, rapidjson::Document::AllocatorType& allocator);
		void SetVec2(const math::vec2& val, rapidjson::Document::AllocatorType& allocator);
		void SetVec3(const math::vec3& val, rapidjson::Document::AllocatorType& allocator);
		void SetVec4(const math::vec4& val, rapidjson::Document::AllocatorType& allocator);
		void SetAabb(const shape::AABB& val, rapidjson::Document::AllocatorType& allocator);
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
	const void JSON::WriteArray(std::vector<vec2> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<vec3> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<vec4> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<Matrix2D> val, rapidjson::Document::AllocatorType& allocator);

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


	__todo() //remove this old stuff when you are reasonablly sure that you don't need it
	//class JSON
	//{
	//public:
	//	JSON();
	//	JSON(const rapidjson::Document& doc);
	//	JSON(const rapidjson::Value& val);
	//	~JSON();
	//	JSON(const JSON& that);						//Copy Ctor
	//	JSON& operator= (const JSON& that);			//Assignment Operator

	//	template<typename type>
	//	JSON& operator= (const type& v);

	//	void					Parse(const std::string& input);
	//	const bool				TryParse(const std::string& input);
	//	const std::string		Read(void) const;

	//	const bool				HasMember(const std::string& member) const;
	//	const bool				IsNull(void) const;
	//	const bool				IsString(void) const;
	//	const bool				IsNumber(void) const;
	//	const bool				IsBool(void) const;
	//	const bool				IsArray(void) const;
	//	const bool				IsObject(void) const;

	//	const int				GetInt(void) const;
	//	const bool				GetBool(void) const;
	//	const double			GetDouble(void) const;
	//	const std::string		GetString(void) const;
	//	const math::vec2		GetVec2(void) const;
	//	const math::vec3		GetVec3(void) const;
	//	const math::vec4		GetVec4(void) const;
	//	const Color				GetColor(void) const;
	//	const math::Matrix2D	GetMatrix(void) const;
	//	const Time				GetTime(void) const;
	//	__todo() //rect
	//	template<typename type>
	//	const std::vector<type> GetArray(void) const;
	//	const JSON				GetArrayValue(const size_t& index) const;
	//	const size_t			GetArraySize(void) const;

	//	void					SetNull(void);
	//	void					SetInt(const int& v);
	//	void					SetBool(const bool& v);
	//	void					SetDouble(const double& v);
	//	void					SetString(const std::string& v);
	//	void					SetVec2(const math::vec2& v);
	//	void					SetVec3(const math::vec3& v);
	//	void					SetVec4(const math::vec4& v);
	//	void					SetColor(const Color& v);
	//	__todo() //rect
	//	void					SetMatrix(const math::Matrix2D& v);
	//	void					SetTime(const Time& v);

	//	const JSON operator[](const std::string& member) const;
	//	const JSON operator[](const size_t& index) const { return GetArrayValue(index); }

	//private:
	//	//rapidjson::Document rdoc;
	//	rapidjson::Value* val;
	//};
}