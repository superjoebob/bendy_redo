#pragma once

#include "plugguieditor.h"
#include <string>
class Plugin;
class PluginGUI : public PluginGUIEditor, public CControlListener
{
public:
	PluginGUI(void*);

	bool open(void* ptr);
	void close();
	void setParameter(int index, float value);
	void valueChanged(CControl* pControl);

	int beginPopupMenu();
	void popupMenuAdd(int menu, std::wstring text, int id, bool horizontalBreak);
	int endPopupMenu(int menu);

	Plugin* getPlugin() { return _plugin; }

private:
	Plugin* _plugin;
};
