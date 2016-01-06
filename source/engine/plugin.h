#pragma once
#include "utility/util_time.h"
#include <list>

namespace engine
{
	class IPlugin
	{
	public:
		//Internal
	private:
		static std::list<IPlugin*> s_pluginList;

		//External
	public:
		static void AddPlugin(IPlugin* const plugin);
		static IPlugin* FindPlugin(const int& type);
		static void UpdatePlugins(const util::Time& dt);
		static void DestroyPlugins();

		static const int Type = 0;
		virtual int GetType() const { return Type; }

		//------------------------------------------------------------------------------

	public:
		IPlugin();
		virtual ~IPlugin() = 0;

		virtual void Init();
		virtual void Exit();
		virtual void Update(const util::Time& dt);

#define DECLARE_PLUGIN_TYPE_INFO(CLASS)									\
			typedef CLASS class_t;										\
			static const int Type;										\
			virtual inline int GetType() const { return Type; }			
#define DEFINE_PLUGIN_TYPE_INFO(CLASS)									\
			const int CLASS::Type = ++IPart::s_nextPartTypeId;			
	};
}