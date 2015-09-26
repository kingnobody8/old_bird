#pragma once

//TODO make proxy (this is the same a provider)
//most providers just are used to specify a directory for assets
//some are networked and don't load an asset syncronisly
//
//#include <string>
//
//namespace Engine
//{
//	namespace Asset
//	{
//		class CAssetProxy
//		{
//			/*Data*/
//		private:
//			/*Func*/
//		public:
//			virtual ~CAssetProxy(void) = 0 {/*LEAVE EMPTY - ABC*/}
//			void* LoadAsset(std::string szFileName) = 0;
//			bool HasAsset(std::string szFileName) = 0;
//		};
//
//		class CAssetProxyFilePath : public CAssetProxy
//		{
//			/*Data*/
//		private:
//			std::string m_szPath;
//
//			/*Func*/
//		public:
//			void* LoadAsset(std::string szFileName);
//			bool HasAsset(std::string szFileName);
//
//			void SetPath(std::string szPath);
//		};
//	}
//}