#include "loader.h"
#include <fstream>

namespace engine
{
	namespace asset
	{
		__todo() //do some research, but these might be move useful as inherited members of a file type
			//also should assert if fail or should return empty product???

		const TBuffer FileLoaderData(const std::string& szFilePath)
		{
			TBuffer ret;
			std::fstream stream;

			stream.open(szFilePath, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);

			assert(stream.is_open());
			if (!stream.is_open())
				return ret;

			ret.size = stream.tellg();
			ret.buffer = new uchar[ret.size + 1];
			stream.seekg(0, stream.beg);
			stream.read(reinterpret_cast<char*>(ret.buffer), ret.size);
			reinterpret_cast<char*>(ret.buffer)[ret.size] = null;

			stream.close();
			return ret;
		}

		const util::JSON FileLoaderJson(const std::string& szFilePath)
		{
			TBuffer data = FileLoaderData(szFilePath);
			if (data.size == 0)
				return util::JSON();

			const util::JSON ret = DataLoaderJson(data);
			delete[] data.buffer;
			return ret;
		}
		const util::JSON DataLoaderJson(const TBuffer& data)
		{
			util::JSON ret;
			ret.Parse(reinterpret_cast<char*>(data.buffer));
			auto asdf = ret.Read();
			assert(!ret.HasParseError());
			return ret;
		}
	}
}
