#include "plugin.h"
#include <assert.h>

namespace engine
{
	STATIC std::list<IPlugin*> IPlugin::s_pluginList;

	STATIC int IPlugin::s_nextPluginTypeId = 0;

	STATIC void IPlugin::AddPlugin(IPlugin* const plugin)
	{
		assert(!FindPlugin(plugin->GetType()));
		plugin->Init();
		s_pluginList.push_back(plugin);
	}

	STATIC IPlugin* IPlugin::FindPlugin(const int& type)
	{
		for (auto iter = s_pluginList.begin(); iter != s_pluginList.end(); ++iter)
		{
			if ((*iter)->GetType() == type)
			{
				return (*iter);
			}
		}
		return null;
	}

	STATIC void IPlugin::UpdatePlugins(const util::Time& dt)
	{
		for (auto iter = s_pluginList.begin(); iter != s_pluginList.end(); ++iter)
		{
			(*iter)->Update(dt);
		}
	}

	STATIC void IPlugin::DestroyPlugins()
	{
		s_pluginList.reverse(); //we want to remove them in reverse order
		for (auto iter = s_pluginList.begin(); iter != s_pluginList.end(); ++iter)
		{
			(*iter)->Exit();
		}
	}

	//------------------------------------------------------------------------------

	IPlugin::IPlugin()
	{
	}

	VIRTUAL void IPlugin::Init()
	{
	}
	
	VIRTUAL void IPlugin::Exit()
	{
	}
	
	VIRTUAL void IPlugin::Update(const util::Time& dt)
	{
	}
}