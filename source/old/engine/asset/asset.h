#pragma once
#include <vector>
#include "engine/asset/asset_container.h"
#include "engine/platform/inc_sdl.h"

namespace Engine
{
	namespace Asset
	{
		class CAsset
		{
			/*Data*/
		private:
			static CAsset* instance;
			SDL_Renderer*			m_pRen;
			std::vector<IAsset*>	m_vAssets;
			std::vector<IAsset*>	m_vContexts;


			/*Func*/
		private:
			CAsset(void);
			~CAsset(void);

			slong	FindAsset(const std::string& szAsset);

		public:
			static CAsset* Get(void);
			static void DeleteInstance(void);

			void LoadAsset(const std::string& szAsset);			//Loads an asset into memory
			void LoadContext(const std::string& szContext);		//Loads a context of assets into memory
			void Push(IAsset* const m_pAsset);					//Pushes an asset that was created by the game
			void UnloadAsset(const std::string& szAsset);		//Unloads an asset from memory
			void UnloadContext(const std::string& szContext);	//Unloads a context of assets from memory
			void Reload(void);									//Unloads then loads all contexts and assets
			void UnloadAll(void);

			slong Aquire(const std::string& szAsset);			//Returns a handle to an asset, and incs the ref count
			void Release(const std::string& szAsset);			//Implies a release of ownership of an asset, decs the ref count
			void Release(const slong& slAsset);					//Implies a release of ownership of an asset, decs the ref count

			void* Get(const slong& slAsset);					//Returns a pointer to an asset, no conversions are done

			//void Data(std::string szFilePath, void* buffer, slong& size);

			inline void SetSdlRenderer(SDL_Renderer* const pRen) { this->m_pRen = pRen; }
			inline SDL_Renderer* GetSdlRenderer(void) const { return this->m_pRen; }
		};


		//class CAsset
		//{
		//	//Data
		//private:
		//	static CAsset* instance;
		//	SDL_Renderer*			m_pRen;
		//	std::vector<IAsset*>	m_vAssets;
		//
		//	std::vector<TImage>		m_vImages;
		//	std::vector<TRune>		m_vFonts;
		//
		//	//Func
		//private:
		//	/*Shared*/
		//	slong FindOpenSlot(const EAssetType eAssetType);
		//	/*Images*/
		//	slong LoadImage_List(const char* szAsset);
		//	slong LoadImage_Pak(const char* szAsset);
		//	slong LoadImage_File(const char* szAsset);
		//	void UnloadImage(const slong slIndex);
		//	/*Fonts*/
		//	slong LoadFont_List(const char* szAsset);
		//	slong LoadFont_Pak(const char* szAsset);
		//	slong LoadFont_File(const char* szAsset);
		//	void UnloadFont(const slong slIndex);
		//
		//	/*Func*/
		//private:
		//	CAsset(void);
		//	~CAsset(void);
		//
		//public:
		//	static CAsset* GetInstance(void);
		//	static void DeleteInstance(void);
		//
		//	/*Interface*/
		//	bool		Init(const char* szPakName);
		//	slong		LoadAsset(const char* szAsset, const EAssetType eAssetType, bool bUsePak = true);
		//	slong		PushAsset(IAsset pAsset, const EAssetType eAssetType);
		//	void		UnloadAsset(const slong slIdentifier, const EAssetType eAssetType);
		//	void		ReloadAssets(const EAssetType eAssetType);
		//
		//	//Gets (Grabs from vectors)
		//	inline SDL_Texture* GrabSDLTexture(const slong slIdentifier) { return this->m_vImages[slIdentifier].m_pTex; }
		//	inline SDL_Surface* GrabSDLSurface(const slong slIdentifier) { return this->m_vImages[slIdentifier].m_pSurface; }
		//	//inline Proprietary::TFont* GrabFont(const slong slIdentifer) { return this->m_vFonts[slIdentifer].m_pFont; }
		//
		//	inline void SetSdlRenderer(SDL_Renderer* const pRen) { this->m_pRen = pRen; }
		//};
	}
}