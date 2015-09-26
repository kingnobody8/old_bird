//#pragma once
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
//			void* LoadAsset(std::string szFileName);
//			bool HasAsset(std::string szFileName);
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