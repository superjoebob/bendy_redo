#pragma once

#include "plugguieditor.h"
#include <string>
#include <map>
#include <vector>
#include <unordered_set>



class Plugin;
class CGraph;
class CBitmapNumber;
class PluginGUI : public PluginGUIEditor, public CControlListener
{
public:
	PluginGUI(void*);

	//void rebuild(void* ptr = nullptr);
	bool open(void* ptr);
	void close(); 
	virtual void idle();

	void setParameter(int index, float value);
	void valueChanged(CControl* pControl);

	void setExpanded(bool expanded);

	CPoint getPopupLocation();

	int beginPopupMenu();
	void popupMenuAdd(int menu, std::wstring text, int id, bool horizontalBreak);
	int endPopupMenu(int menu);

	void showTooltip(std::wstring text);
	std::wstring getInput(std::wstring caption, std::wstring defaultText);
	void showDefaultControlMenu(PlugParameter* param, bool noteControlAssigned);
	int showDefaultCCMenu();

	void reconnect();
	Plugin* getPlugin() { return _plugin; }
private:
	Plugin* _plugin;

	template <typename T>
	T* addControl(CView* control, std::wstring param = L"", bool setMouseEnabled = false);
	std::map<int, CControl*> _hashMap;
	std::map<int, CControl*> _enableSection;
	std::vector<CMovieBitmap*> _letters;

	std::map<int, CBaseObject*> _paramsMap;

	CGraph* _graph;
	std::unordered_set<CControl*> _graphUpdaters;
	CKickButton* _expandButton;
	CKickButton* _contractButton;
	CControl* _currentEnableButton;
	bool _expanded;

	HMENU ContextMenu;
	HMENU NoteControl1SubMenu;
	HMENU NoteControl2SubMenu;
	int PopupParameter; 
	
	CBitmapNumber* _versionNumber;
};
