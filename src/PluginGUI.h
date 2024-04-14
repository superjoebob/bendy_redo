#pragma once

#include "plugguieditor.h"
class Plugin;
class PluginGUI : public PluginGUIEditor, public CControlListener
{
public:
	PluginGUI(void*);

	bool open(void* ptr);
	void close();
	void setParameter(int index, float value);
	void valueChanged(CControl* pControl);

private:
	Plugin* _plugin;
};
