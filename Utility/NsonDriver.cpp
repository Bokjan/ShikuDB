// Naive Binary JSON Driver
#include <string>
#include <cstring>
#include "Json.hpp"
#include "Utility.hpp"
using std::string;
using byte = char;
using Json = nlohmann::json;
using shiku::Log;
/*
	0 Document - (length + doc) - ([uint32_t]32B + n B)
	1 Array - (length + type + arr) - ([uint32_t]32B + [uint8_t]8B + n B)
	2 Null - nil - 0B
	3 Boolean - For array, followed by BT or BF
	4 BooleanTrue - nil - 0B
	5 BooleanFalse - nil - 0B
	6 Integer - int64_t - 64B
	7 UnsignedInteger - uint64_t - 64B
	8 Float - double - 64B
	9 String - (length + value) - ([uint32_t]32B + n B)

	!!! `NsonType` defined in `Utility.hpp`
*/
namespace Internal
{
	using shiku::Utility::NsonType;
	static void WriteString(string &str, byte *&p);
	static string FetchString(const byte *&p);
	static void SerializeDocument(const Json &json, byte *&p);
	static void SerializeArray(const Json &json, byte *&p);
	static Json UnserializeDocument(const byte *&pointer);
	static Json UnserializeArray(const byte *&pointer);
	static void WriteString(string &str, byte *&p)
	{
		// String - (length + value) - ([uint32_t]32B + n B)
		uint32_t *str_length = (uint32_t*)p;
		*str_length = str.length();
		p += sizeof(uint32_t);
		strcpy(p, str.c_str());
		p += *str_length;
	}
	static string FetchString(const byte *&p)
	{
		uint32_t length = *((uint32_t*)p);
		p += sizeof(uint32_t);
		string ret = string(p, length);
		p += length;
		return ret;
	}
	static void SerializeDocument(const Json &json, byte *&p)
	{
		uint32_t *doc_length = (uint32_t*)p;
		p += sizeof(uint32_t);
		byte *doc_start = p;
		for(Json::const_iterator it = json.begin(); it != json.end(); ++it)
		{
			string handle;
			// Write key (String)
			handle = it.key();
			WriteString(handle, p);
			// Write value
			switch(it.value().type())
			{
				case Json::value_t::object: // NsonType::Document
					*((NsonType*)p) = NsonType::Document;
					p += sizeof(NsonType);
					SerializeDocument(it.value(), p);
					break;
				case Json::value_t::array: // // NsonType::Array
					*((NsonType*)p) = NsonType::Array;
					p += sizeof(NsonType);
					SerializeArray(it.value(), p);
					break;
				case Json::value_t::null: // NsonType::Null
					*((NsonType*)p) = NsonType::Null;
					p += sizeof(NsonType);
					break;
				case Json::value_t::boolean: // NsonType::Boolean{}
					if(it.value() == true)
						*((NsonType*)p) = NsonType::BooleanTrue;
					else
						*((NsonType*)p) = NsonType::BooleanFalse;
					p += sizeof(NsonType);
					break;
				case Json::value_t::number_integer: // NsonType::Integer
					*((NsonType*)p) = NsonType::Integer;
					p += sizeof(NsonType);
					*((int64_t*)p) = it.value();
					p += sizeof(int64_t);
					break;
				case Json::value_t::number_unsigned: // NsonType::UnsignedInteger
					*((NsonType*)p) = NsonType::UnsignedInteger;
					p += sizeof(NsonType);
					*((uint64_t*)p) = it.value();
					p += sizeof(uint64_t);
					break;
				case Json::value_t::number_float: // NsonType::Float
					*((NsonType*)p) = NsonType::Float;
					p += sizeof(NsonType);
					*((double*)p) = it.value();
					p += sizeof(double);
					break;
				case Json::value_t::string: // NsonType::String
					*((NsonType*)p) = NsonType::String;
					p += sizeof(NsonType);
					handle = it.value();
					WriteString(handle, p);
					break;
				default:;
			}
		}
		*doc_length = (uint32_t)(p - doc_start);
	}
	static void SerializeArray(const Json &json, byte *&p)
	{
		// Array - (length + type + doc) - ([uint32_t]32B + [uint8_t]8B + n B)
		uint32_t *arr_length = (uint32_t*)p;
		p += sizeof(uint32_t);
		NsonType *elem_type = (NsonType*)p;
		p += sizeof(uint8_t);
		// Empty-array check
		if(json.begin() == json.end())
		{
			*elem_type = NsonType::Null;
			*arr_length = 0;
			return;
		}
		// Determine the type of array
		Json::const_iterator it = json.begin();
		switch(it.value().type())
		{
			case Json::value_t::object: // NsonType::Document
				*elem_type = NsonType::Document;
				break;
			case Json::value_t::array: // NsonType::Array
				*elem_type = NsonType::Array;
				break;
			case Json::value_t::null: // NsonType::Null
				*elem_type = NsonType::Null;
				break;
			case Json::value_t::boolean: // NsonType::Boolean{}
				*elem_type = NsonType::Boolean;
				break;
			case Json::value_t::number_integer: // NsonType::Integer
				*elem_type = NsonType::Integer;
				break;
			case Json::value_t::number_unsigned: // NsonType::UnsignedInteger
				*elem_type = NsonType::UnsignedInteger;
				break;
			case Json::value_t::number_float: // NsonType::Float
				*elem_type = NsonType::Float;
				break;
			case Json::value_t::string: // NsonType::String
				*elem_type = NsonType::String;
			default:;
		}
		const byte *arr_start = p;
		for(it = json.begin(); it != json.end(); ++it)
		{
			string handle;
			// Write value
			switch(it.value().type())
			{
				case Json::value_t::object: // NsonType::Document
					SerializeDocument(it.value(), p);
					break;
				case Json::value_t::array: // // NsonType::Array
					SerializeArray(it.value(), p);
					break;
				case Json::value_t::null: // NsonType::Null
					*((NsonType*)p) = NsonType::Null;
					p += sizeof(NsonType);
					break;
				case Json::value_t::boolean: // NsonType::Boolean{}
					if(it.value() == true)
						*((NsonType*)p) = NsonType::BooleanTrue;
					else
						*((NsonType*)p) = NsonType::BooleanFalse;
					p += sizeof(NsonType);
					break;
				case Json::value_t::number_integer: // NsonType::Integer
					*((int64_t*)p) = it.value();
					p += sizeof(int64_t);
					break;
				case Json::value_t::number_unsigned: // NsonType::UnsignedInteger
					*((uint64_t*)p) = it.value();
					p += sizeof(uint64_t);
					break;
				case Json::value_t::number_float: // NsonType::Float
					*((double*)p) = it.value();
					p += sizeof(double);
					break;
				case Json::value_t::string: // NsonType::String
					handle = it.value();
					WriteString(handle, p);
					break;
				default:;
			}
		}
		*arr_length = (uint32_t)(p - arr_start);
	}
	static Json UnserializeDocument(const byte *&pointer)
	{
		const byte *&p = pointer;
		Json ret = Json::object();
		uint32_t doc_length = *((uint32_t*)p);
		p += sizeof(uint32_t);
		const byte *start = p;
		while(p < start + doc_length)
		{
			// Fetch key
			string key = FetchString(p);
			NsonType value_type = *((NsonType*)p);
			p += sizeof(NsonType);
			switch(value_type)
			{
				case NsonType::Document:
					ret[key] = UnserializeDocument(p);
					break;
				case NsonType::Array:
					ret[key] = UnserializeArray(p);
					break;
				case NsonType::Null:
					ret[key] = nullptr;
					break;
				case NsonType::BooleanTrue:
					ret[key] = true;
					break;
				case NsonType::BooleanFalse:
					ret[key] = false;
					break;
				case NsonType::Integer:
					ret[key] = *((int64_t*)p);
					p += sizeof(int64_t);
					break;
				case NsonType::UnsignedInteger:
					ret[key] = *((uint64_t*)p);
					p += sizeof(uint64_t);
					break;
				case NsonType::Float:
					ret[key] = *((double*)p);
					p += sizeof(double);
					break;
				case NsonType::String:
					ret[key] = FetchString(p);
					break;
				default:;
			}
		}
		return ret;
	}
	static Json UnserializeArray(const byte *&pointer)
	{
		const byte *&p = pointer;
		Json ret = Json::array();
		uint32_t arr_length = *((uint32_t*)p);
		p += sizeof(uint32_t);
		const NsonType arr_type = *((NsonType*)p);
		p += sizeof(NsonType);
		const byte *start = p;
		while(p < start + arr_length)
		{
			switch(arr_type)
			{
				case NsonType::Document:
					ret.push_back(UnserializeDocument(p));
					break;
				case NsonType::Array:
					ret.push_back(UnserializeArray(p));
					break;
				case NsonType::Null:
					ret.push_back(nullptr);
					p += sizeof(NsonType);
					break;
				case NsonType::Boolean:
					if(*((NsonType*)p) == NsonType::BooleanTrue)
						ret.push_back(true);
					else
						ret.push_back(false);
					p += sizeof(NsonType);
					break;
				case NsonType::Integer:
					ret.push_back(*((int64_t*)p));
					p += sizeof(int64_t);
					break;
				case NsonType::UnsignedInteger:
					ret.push_back(*((uint64_t*)p));
					p += sizeof(uint64_t);
					break;
				case NsonType::Float:
					ret.push_back(*((double*)p));
					p += sizeof(double);
					break;
				case NsonType::String:
					ret.push_back(FetchString(p));
					break;
				default:;
			}
		}
		return ret;
	}
}
namespace shiku::Utility
{
	void SerializeNson(const Json &json, void *p)
	{
		byte *mem = (byte*)p;
		Internal::SerializeDocument(json, mem);
	}
	Json UnserializeNson(const void *p)
	{
		const byte *mem = (const byte*)p;
		return Internal::UnserializeDocument(mem);
	}
}