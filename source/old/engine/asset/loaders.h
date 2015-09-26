#pragma once
#include "engine/asset/asset_container.h"
#include "engine/util/util.h"
#include "engine/platform/inc_sdl.h"
#include "rapidjson/document.h"

namespace Engine
{
	namespace Asset
	{
		TData FileLoaderData(std::string szFilePath);

		rapidjson::Document FileLoaderJson(std::string szFilePath);
		rapidjson::Document DataLoaderJson(const TData& data);

		SDL_Texture* DataLoaderSdlTexture(const TData& data);
		SDL_Texture* FileLoaderSdlTexture(std::string szFilePath);
	}
}