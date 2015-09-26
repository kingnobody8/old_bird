#include "json.h"
#include "rapidjson/prettywriter.h"

/*https://miloyip.github.io/rapidjson/index.html*/
//this isn't used but may be useful in the future for making the c++ syntax of json work like javascript syntax
//http://programmaticallyspeaking.blogspot.se/2012/09/infer-return-type-for-templated.html

namespace util
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
	const Color JSON::GetColor() const
	{
		if (HasMember("a"))
		{
			return Color(
				(*this)["r"].GetDouble(),
				(*this)["g"].GetDouble(),
				(*this)["b"].GetDouble(),
				(*this)["a"].GetDouble());
		}
		else
		{
			return Color(
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
	const math::Matrix2D JSON::GetMatrix() const
	{
		const JSON j = static_cast<const JSON>(((*this)["position"]));

		Matrix2D mat;
		const util::math::vec2 pos = static_cast<const JSON>((*this)["position"]).GetVec2();
		const util::math::vec2 scale = static_cast<const JSON>((*this)["scale"]).GetVec2();
		const float rot = (double)static_cast<const JSON>((*this)["rotation"]).GetDouble();

		mat.SetPosition(pos);
		mat.SetScale(scale);
		mat.SetRotationZ(rot);

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
	const std::vector<Color> JSON::GetArray<Color>(void) const
	{
		assert(IsArray());
		std::vector<Color> vec;
		vec.reserve(Size());
		for (int i = 0; i < (int)Size(); ++i)
			vec.push_back(((JSON)((*this)[i])).GetColor());
		return vec;
	}
	template<>
	const std::vector<vec2> JSON::GetArray<vec2>(void) const
	{
		assert(IsArray());
		std::vector<vec2> vec;
		vec.reserve(Size());
		for (int i = 0; i < (int)Size(); ++i)
			vec.push_back(((JSON)((*this)[i])).GetVec2());
		return vec;
	}
	template<>
	const std::vector<vec3> JSON::GetArray<vec3>(void) const
	{
		assert(IsArray());
		std::vector<vec3> vec;
		vec.reserve(Size());
		for (int i = 0; i < (int)Size(); ++i)
			vec.push_back(((JSON)((*this)[i])).GetVec3());
		return vec;
	}
	template<>
	const std::vector<vec4> JSON::GetArray<vec4>(void) const
	{
		assert(IsArray());
		std::vector<vec4> vec;
		vec.reserve(Size());
		for (int i = 0; i < (int)Size(); ++i)
			vec.push_back(((JSON)((*this)[i])).GetVec4());
		return vec;
	}
	template<>
	const std::vector<Matrix2D> JSON::GetArray<Matrix2D>(void) const
	{
		assert(IsArray());
		std::vector<Matrix2D> vec;
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
	void JSON::SetColor(const Color& val, rapidjson::Document::AllocatorType& allocator)
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
		AddMember("z", val.y, allocator);
	}
	void JSON::SetVec4(const math::vec4& val, rapidjson::Document::AllocatorType& allocator)
	{
		SetObject();
		AddMember("x", val.x, allocator);
		AddMember("y", val.y, allocator);
		AddMember("z", val.y, allocator);
		AddMember("w", val.y, allocator);
	}
	void JSON::SetMatrix(const math::Matrix2D& val, rapidjson::Document::AllocatorType& allocator)
	{
		JSON vpos, vscale, vrot;
		vpos.SetVec2(val.GetPosition(), allocator);
		vscale.SetVec2(val.GetScale(), allocator);
		vrot.SetDouble((double)val.GetRotationZ());

		SetObject();
		AddMember("position", vpos, allocator);
		AddMember("scale", vscale, allocator);
		AddMember("rotation", vrot, allocator);
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
	const void JSON::WriteArray(std::vector<Color> val, rapidjson::Document::AllocatorType& allocator)
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
	const void JSON::WriteArray(std::vector<vec2> val, rapidjson::Document::AllocatorType& allocator)
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
	const void JSON::WriteArray(std::vector<vec3> val, rapidjson::Document::AllocatorType& allocator)
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
	const void JSON::WriteArray(std::vector<vec4> val, rapidjson::Document::AllocatorType& allocator)
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
	const void JSON::WriteArray(std::vector<Matrix2D> val, rapidjson::Document::AllocatorType& allocator)
	{
		SetArray();
		for (size_t i = 0; i < val.size(); ++i)
		{
			JSON j;//
			j.SetMatrix(val[i], allocator);
			PushBack(j, allocator);
		}
	}

	__todo() //remove this old stuff when you are reasonablly sure that you don't need it


	////TODO finish the templates
	////template<>
	////const JSON JSON::operator[]<JSON>(const std::string& member) const
	////{
	////	return JSON(this->rdoc[member.c_str()]);
	////}
	////template<>
	////const int JSON::operator[]<int>(const std::string& member) const
	////{
	////	return this->rdoc[member.c_str()].GetInt();
	////}
	////template<>
	////const bool JSON::operator[]<bool>(const std::string& member) const
	////{
	////	return this->rdoc[member.c_str()].GetBool();
	////}
	//const JSON JSON::operator[](const std::string& member) const
	//{
	//	return JSON(this->rdoc[member.c_str()]);
	//}

	//template<>
	//const std::vector<JSON> JSON::GetArray<JSON>(void) const
	//{
	//	assert(rdoc.IsArray());
	//	std::vector<JSON> vec;
	//	vec.reserve(rdoc.Size());
	//	for (int i = 0; i < (int)rdoc.Size(); ++i)
	//		vec.push_back(JSON(rdoc[i]));
	//	return vec;
	//}

	//template<>
	//const std::vector<int> JSON::GetArray<int>(void) const
	//{
	//	assert(rdoc.IsArray());
	//	std::vector<int> vec;
	//	vec.reserve(rdoc.Size());
	//	for (int i = 0; i < (int)rdoc.Size(); ++i)
	//		vec.push_back(rdoc[i].GetInt());
	//	return vec;
	//}

	//template<>
	//const std::vector<double> JSON::GetArray<double>(void) const
	//{
	//	assert(rdoc.IsArray());
	//	std::vector<double> vec;
	//	vec.reserve(rdoc.Size());
	//	for (int i = 0; i < (int)rdoc.Size(); ++i)
	//		vec.push_back(rdoc[i].GetDouble());
	//	return vec;
	//}

	//template<>
	//const std::vector<bool> JSON::GetArray<bool>(void) const
	//{
	//	assert(rdoc.IsArray());
	//	std::vector<bool> vec;
	//	vec.reserve(rdoc.Size());
	//	for (int i = 0; i < (int)rdoc.Size(); ++i)
	//		vec.push_back(rdoc[i].GetBool());
	//	return vec;
	//}

	//template<>
	//const std::vector<std::string> JSON::GetArray<std::string>(void) const
	//{
	//	assert(rdoc.IsArray());
	//	std::vector<std::string> vec;
	//	vec.reserve(rdoc.Size());
	//	for (int i = 0; i < (int)rdoc.Size(); ++i)
	//		vec.push_back(rdoc[i].GetString());
	//	return vec;
	//}

	//const JSON JSON::GetArrayValue(const size_t& index) const
	//{
	//	return JSON(rdoc[rapidjson::SizeType(index)]);
	//}

	//const size_t JSON::GetArraySize(void) const
	//{
	//	assert(rdoc.IsArray());
	//	return rdoc.Size();
	//}

	//JSON::JSON(void)
	//{
	//}
	//JSON::JSON(const rapidjson::Document& doc)
	//{
	//	//Perform deep copy of dom tree
	//	rapidjson::Document::AllocatorType& a = this->rdoc.GetAllocator();
	//	this->rdoc.CopyFrom(doc, a);
	//}
	//JSON::JSON(const rapidjson::Value& val)
	//{
	//	//Perform deep copy of dom tree
	//	rapidjson::Document::AllocatorType& a = this->rdoc.GetAllocator();
	//	this->rdoc.CopyFrom(val, a);
	//}
	//JSON::~JSON(void)
	//{
	//}
	//JSON::JSON(const JSON& that)
	//{
	//	*this = that;
	//}
	//JSON& JSON::operator= (const JSON& that)
	//{
	//	//Check for self assignment
	//	if (this == &that)
	//		return *this;
	//	//Perform deep copy of dom tree
	//	rapidjson::Document::AllocatorType& a = this->rdoc.GetAllocator();
	//	this->rdoc.CopyFrom(that.rdoc, a);
	//	return *this;
	//}

	//template<>
	//JSON& JSON::operator=<int> (const int& v)
	//{
	//	SetInt(v);
	//	return *this;
	//}
	//template<>
	//JSON& JSON::operator=<bool> (const bool& v)
	//{
	//	SetBool(v);
	//	return *this;
	//}
	//template<>
	//JSON& JSON::operator=<double> (const double& v)
	//{
	//	SetDouble(v);
	//	return *this;
	//}
	//template<>
	//JSON& JSON::operator=<std::string> (const std::string& v)
	//{
	//	SetString(v);
	//	return *this;
	//}
	//template<>
	//JSON& JSON::operator=<math::vec2> (const math::vec2& v)
	//{
	//	SetVec2(v);
	//	return *this;
	//}
	//template<>
	//JSON& JSON::operator=<math::vec3> (const math::vec3& v)
	//{
	//	SetVec3(v);
	//	return *this;
	//}
	//template<>
	//JSON& JSON::operator=<math::vec4> (const math::vec4& v)
	//{
	//	SetVec4(v);
	//	return *this;
	//}
	//template<>
	//JSON& JSON::operator=<Color> (const Color& v)
	//{
	//	SetColor(v);
	//	return *this;
	//}
	//template<>
	//JSON& JSON::operator=<Matrix2D> (const Matrix2D& v)
	//{
	//	SetMatrix(v);
	//	return *this;
	//}
	//template<>
	//JSON& JSON::operator=<Time> (const Time& v)
	//{
	//	SetTime(v);
	//	return *this;
	//}

	//void JSON::Parse(const std::string& input)
	//{
	//	rdoc.Parse(input.c_str());
	//	assert(!rdoc.HasParseError());
	//}
	//const bool JSON::TryParse(const std::string& input)
	//{
	//	rdoc.Parse(input.c_str());
	//	return rdoc.HasParseError();
	//}
	//const std::string JSON::Read(void) const
	//{
	//	rapidjson::StringBuffer sbuff;
	//	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sbuff);
	//	this->rdoc.Accept(writer);
	//	return sbuff.GetString();
	//}

	//const bool JSON::HasMember(const std::string& member) const
	//{
	//	return rdoc.HasMember(member.c_str());
	//}

	//const bool JSON::IsNull(void) const
	//{
	//	return rdoc.IsNull();
	//}
	//const bool JSON::IsString(void) const
	//{
	//	return rdoc.IsString();
	//}
	//const bool JSON::IsNumber(void) const
	//{
	//	return rdoc.IsNumber();
	//}
	//const bool JSON::IsBool(void) const
	//{
	//	return rdoc.IsBool();
	//}
	//const bool JSON::IsArray(void) const
	//{
	//	return rdoc.IsArray();
	//}
	//const bool JSON::IsObject(void) const
	//{
	//	return rdoc.IsObject();
	//}

	//const int JSON::GetInt(void) const
	//{
	//	return rdoc.GetInt();
	//}
	//const bool JSON::GetBool(void) const
	//{
	//	return rdoc.GetBool();
	//}
	//const double JSON::GetDouble(void) const
	//{
	//	return rdoc.GetDouble();
	//}
	//const std::string JSON::GetString(void) const
	//{
	//	return rdoc.GetString();
	//}
	//const math::vec2 JSON::GetVec2(void) const
	//{
	//	if (rdoc.HasMember("x")) // X & Y
	//		return math::vec2(rdoc["x"].GetDouble(), rdoc["y"].GetDouble());
	//	else if (rdoc.HasMember("w"))// W & H
	//		return math::vec2(rdoc["w"].GetDouble(), rdoc["h"].GetDouble());
	//	assert(false);
	//	return math::vec2(); //this is for 'not all control paths return a value' warning
	//}
	//const math::vec3 JSON::GetVec3(void) const
	//{
	//	if (rdoc.HasMember("x")) // X & Y & Z
	//		return math::vec3(rdoc["x"].GetDouble(), rdoc["y"].GetDouble(), rdoc["z"].GetDouble());
	//	else if (rdoc.HasMember("w")) // W & H & D
	//		return math::vec3(rdoc["w"].GetDouble(), rdoc["h"].GetDouble(), rdoc["d"].GetDouble());
	//	assert(false);
	//	return math::vec3(); //this is for 'not all control paths return a value' warning
	//}
	//const math::vec4 JSON::GetVec4(void) const
	//{
	//	__not_implemented(JSON::GetVec4);
	//	return math::vec4();
	//}
	//const Color JSON::GetColor(void) const
	//{
	//	if (rdoc.HasMember("a"))
	//	{
	//		return Color(rdoc["r"].GetDouble(),
	//			rdoc["g"].GetDouble(),
	//			rdoc["b"].GetDouble(),
	//			rdoc["a"].GetDouble());
	//	}
	//	else
	//	{
	//		return Color(rdoc["r"].GetDouble(),
	//			rdoc["g"].GetDouble(),
	//			rdoc["b"].GetDouble(),
	//			1.0f);
	//	}
	//}
	//const math::Matrix2D JSON::GetMatrix(void) const
	//{
	//	Matrix2D mat;
	//	const JSON jpos = JSON(rdoc["position"]);
	//	const JSON jscale = JSON(rdoc["scale"]);
	//	const JSON jrot = JSON(rdoc["rotation"]);

	//	mat.SetPosition(jpos.GetVec2());
	//	mat.SetScale(jscale.GetVec2());
	//	mat.SetRotationZ((float)jrot.GetDouble());

	//	return mat;
	//}
	//const Time JSON::GetTime(void) const
	//{
	//	return Time(rdoc.GetUint64());
	//}

	//void JSON::SetNull(void)
	//{
	//	rdoc = nullptr;
	//}
	//void JSON::SetInt(const int& v)
	//{
	//	rdoc.SetInt(v);
	//}
	//void JSON::SetBool(const bool& v)
	//{
	//	rdoc.SetBool(v);
	//}
	//void JSON::SetDouble(const double& v)
	//{
	//	rdoc.SetDouble(v);
	//}
	//void JSON::SetString(const std::string& v)
	//{
	//	rdoc.SetString(rapidjson::StringRef(v.c_str()));
	//}
	//void JSON::SetVec2(const math::vec2& v)
	//{
	//	__todo() //make JSON be able to write using []
	//	//example
	//	//JSON json;
	//	//json["vec"]["x"] = 1.0f;
	//	//json["vec"]["y"] = 1.0f;
	//	//vec and x and y were create when they were accessed (THEY CAN"T BE A COPY FOR THIS TO WORK, the return json object from [] has to be the reference to the real one)

	//	/*rapidjson::Document doc;

	//	rapidjson::Value val;
	//	val.AddMember("x", 1.0f, doc.GetAllocator());
	//	val.AddMember("y", 0.5f, doc.GetAllocator());
	//	doc.AddMember("vec", v, doc.GetAllocator());

	//	rapidjson::Value& spot = doc["vec"];

	//	double d = 0.0f;
	//	(*this)["x"] = d;

	//	this->operator=<double>(v.x);
	//	(*this)["x"] = (double)v.x;
	//	(*this)["y"] = (double)v.y;*/
	//}
	//void JSON::SetVec3(const math::vec3& v)
	//{
	//}
	//void JSON::SetColor(const Color& v)
	//{
	//}
	//void JSON::SetMatrix(const math::Matrix2D& v)
	//{
	//}
	//void JSON::SetTime(const Time& v)
	//{
	//	rdoc.SetUint64(v.Milli());
	//}
}