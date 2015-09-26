#include "engine/asset/loaders.h"
#include "engine/asset/asset.h"
#include "engine/platform/inc_sdl_img.h"
#include <fstream>
using namespace Engine::Util;
using namespace rapidjson;

namespace Engine
{
	namespace Asset
	{
		TData FileLoaderData(std::string szFilePath)
		{
			TData ret;
			std::fstream stream;

			stream.open(szFilePath, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);

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

		rapidjson::Document FileLoaderJson(std::string szFilePath)
		{
			rapidjson::Document ret;
			TData data = FileLoaderData(szFilePath);
			if (data.size == 0)
				return ret;

			ret = DataLoaderJson(data);
			delete [] data.buffer;
			return ret;
		}
		rapidjson::Document DataLoaderJson(const TData& data)
		{
			Document ret;
			ret.Parse(reinterpret_cast<char*>(data.buffer));
			return ret;
		}

		SDL_Texture* LoaderSdlTexture(void* data, slong size)
		{
			return null;
		}
		SDL_Texture* FileLoaderSdlTexture(std::string szFilePath)
		{
			//Load the texture
			SDL_Texture* pTex = IMG_LoadTexture(CAsset::Get()->GetSdlRenderer(), szFilePath.c_str());
			if (pTex == NULL)
			{
				SDL_Log("Error - IMG_LoadTexture - %s.\n", SDL_GetError());
				assert(false);
			}
			return pTex;
		}
	}
}