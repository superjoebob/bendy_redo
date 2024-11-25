#include "PluginGUI.h"
#include "Plugin.h"
#include "Parameter.h"
#include "vstcontrols_ext.h"
#include "../resource.h"
//------------------------------------------------------------------------------------
//AEffGUIEditor* createEditor(AudioEffectX* effect)
//{
//	return new TutorialEditor(effect);
//}

//------------------------------------------------------------------------------------
PluginGUI::PluginGUI(void* ptr)
	: PluginGUIEditor(ptr)
{
	_plugin = (Plugin*)ptr;

	ContextMenu = 0;
	HMENU NoteControl1SubMenu = 0;
	HMENU NoteControl2SubMenu = 0;
	PopupParameter = -1;
	_currentEnableButton = nullptr;
	_expandButton = nullptr;
	_expanded = false;
	_graph = nullptr;
}

//void PluginGUI::rebuild(void* ptr)
//{
//	if (frame != nullptr)
//		ptr = frame->getSystemWindow();
//
//	close();
//	open(ptr);
//}

void PluginGUI::reconnect()
{
	for (auto it = _paramsMap.begin(); it != _paramsMap.end(); it++)
	{
		it->second->setParameter(_plugin->getParameter(it->first), _plugin);
	}
	_graph->reconnect();
	setExpanded(_plugin->getParameter(L"expanded")->getFloat() > 0.5f);
}

template <typename T>
T* PluginGUI::addControl(CView* control, std::wstring param, bool setMouseEnabled)
{
	if (param != L"")
	{
		PlugParameter* parameter = _plugin->getParameter(param);
		if (parameter != nullptr)
		{
			control->setParameter(parameter, _plugin);

			if (dynamic_cast<CControl*>(control) != nullptr)
				_hashMap[parameter->hash] = dynamic_cast<CControl*>(control);

			_paramsMap[parameter->hash] = control;

			if (_currentEnableButton != nullptr)
				_enableSection[parameter->hash] = _currentEnableButton;
		}
	}

	frame->addView(control);
	if (setMouseEnabled)
		control->setMouseEnabled(true);


	return (T*)control;
}
//------------------------------------------------------------------------------------
bool PluginGUI::open(void* ptr)
{
	//-- first we create the frame with a size of 300, 300 and set the background to white
	CBitmap* background = new CBitmap(PNG_bendyBack);
	CRect frameSize(0, 0, background->getWidth(), background->getHeight());
	CFrame* newFrame = new CFrame(frameSize, ptr, this);
	newFrame->setBackgroundColor(kWhiteCColor);
	newFrame->setBackground(background);
	frame = newFrame;

	background->forget(); 

	CBitmap* font = new CBitmap(PNG_biosFontCaseSensitive_halfres);
	CBitmap* knobHandle = new CBitmap(PNG_knob);
	CBitmap* tinyCheckbox = new CBitmap(PNG_tinyCheckboxSimple);

	CBitmap* expandButtonImage = new CBitmap(PNG_buttonExpand);
	_expandButton = addControl<CKickButton>(new CKickButton(CRect(sPoint(117, 34), sPoint(23, 7)), this, 0, expandButtonImage, 0));
	expandButtonImage->forget();

	CBitmap* contractButtonImage = new CBitmap(PNG_buttonContract);
	_contractButton = addControl<CKickButton>(new CKickButton(CRect(sPoint(117, 34), sPoint(23, 7)), this, 0, contractButtonImage, 0));
	_contractButton->setVisible(false);
	contractButtonImage->forget();



	CBitmap* letter = new CBitmap(PNG_bendyLetterB);
	CMovieBitmap* bendyLetter = new CMovieBitmap(CRect(sPoint(6, 5), sPoint(7, 6)), this, 0, 3, sCoord(6), letter);
	newFrame->addView(bendyLetter);
	_letters.push_back(bendyLetter);

	letter = new CBitmap(PNG_bendyLetterE);
	bendyLetter = new CMovieBitmap(CRect(sPoint(6, 11), sPoint(7, 6)), this, 0, 3, sCoord(6), letter);
	newFrame->addView(bendyLetter);
	_letters.push_back(bendyLetter);

	letter = new CBitmap(PNG_bendyLetterN);
	bendyLetter = new CMovieBitmap(CRect(sPoint(6, 17), sPoint(7, 6)), this, 0, 3, sCoord(6), letter);
	newFrame->addView(bendyLetter);
	_letters.push_back(bendyLetter);

	letter = new CBitmap(PNG_bendyLetterD);
	bendyLetter = new CMovieBitmap(CRect(sPoint(6, 23), sPoint(7, 6)), this, 0, 3, sCoord(6), letter);
	newFrame->addView(bendyLetter);
	_letters.push_back(bendyLetter);

	letter = new CBitmap(PNG_bendyLetterY);
	bendyLetter = new CMovieBitmap(CRect(sPoint(6, 29), sPoint(7, 6)), this, 0, 3, sCoord(6), letter);
	newFrame->addView(bendyLetter);
	_letters.push_back(bendyLetter);


	//Main settings
	//----------------------------------------------------
	addControl<CSpinner>(new CSpinner(CRect(sPoint(50, 7), sPoint(25, 11)), newFrame, this, 0, 2, 0, 16), L"port");
	addControl<CSpinner>(new CSpinner(CRect(sPoint(117, 7), sPoint(25, 11)), newFrame, this, 0, 2, 0, 64), L"channel");
	addControl<CSpinner>(new CSpinner(CRect(sPoint(50, 21), sPoint(25, 11)), newFrame, this, 0, 2, 1, 64), L"range");

	CBitmap* checkbox = new CBitmap(PNG_checkboxTriggerNotesSimple);
	addControl<COnOffButton>(new COnOffButton(CRect(sPoint(85, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNoteTrigger"), checkbox, 0), L"enableNoteTrigger");
	checkbox->forget();

	checkbox = new CBitmap(PNG_checkboxVelocitySimple);
	addControl<COnOffButton>(new COnOffButton(CRect(sPoint(100, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNoteVelocity"), checkbox, 0), L"enableNoteVelocity");
	checkbox->forget();

	checkbox = new CBitmap(PNG_checkboxPanningSimple);
	addControl<COnOffButton>(new COnOffButton(CRect(sPoint(115, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNotePanning"), checkbox, 0), L"enableNotePanning");
	checkbox->forget();

	checkbox = new CBitmap(PNG_checkboxPitchSimple);
	addControl<COnOffButton>(new COnOffButton(CRect(sPoint(130, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNotePitch"), checkbox, 0), L"enableNotePitch");
	checkbox->forget();
	//----------------------------------------------------
	
	//Midi patch
	//----------------------------------------------------
	addControl<CSpinner>(new CSpinner(CRect(sPoint(4, 42), sPoint(88, 11)), newFrame, this, 0, 2, 0, 128, true, CHoriTxtAlign::kLeftText), L"midiPatch");
	addControl<CSpinner>(new CSpinner(CRect(sPoint(93, 42), sPoint(25, 11)), newFrame, this, 0, 2, 0, 64), L"midiBankCoarse");
	addControl<CSpinner>(new CSpinner(CRect(sPoint(119, 42), sPoint(25, 11)), newFrame, this, 0, 2, 0, 64), L"midiBankFine");
	//----------------------------------------------------

	//Vibrato
	//----------------------------------------------------
	_currentEnableButton = addControl<COnOffButton>(new COnOffButton(CRect(sPoint(39, 56), sPoint(5, 5)), this, _plugin->getCID(L"vibratoEnabled"), tinyCheckbox, 0), L"vibratoEnabled");
	addControl<CAnimKnob>(new CAnimKnob(CRect(sPoint(70, 58), sPoint(11, 11)), this, _plugin->getCID(L"vibratoSpeed"), 24, 11 * sScaleFactor, knobHandle), L"vibratoSpeed", true);
	addControl<CAnimKnob>(new CAnimKnob(CRect(sPoint(132, 58), sPoint(11, 11)), this, _plugin->getCID(L"vibratoDepth"), 24, 11 * sScaleFactor, knobHandle), L"vibratoDepth", true);
	_currentEnableButton = nullptr;

	//ASDR
	//----------------------------------------------------
	_currentEnableButton = addControl<COnOffButton>(new COnOffButton(CRect(sPoint(2, 79), sPoint(5, 5)), this, 0, tinyCheckbox, 0), L"asdrCCEnable");

	CBitmapText* asdrText = addControl<CBitmapText>(new CBitmapText(this, CRect(sPoint(8, 81), sPoint(38, 6)), CPoint(12, 12), font, L"Enter ASDR Name"), L"asdrCCName");
	asdrText->setText(L"ASDR CC");

	asdrText = addControl<CBitmapText>(new CBitmapText(this, CRect(sPoint(51, 81), sPoint(5, 6)), CPoint(12, 12), font, L"Enter MIDI CC"), L"asdrCC0");
	asdrText->setText(L"A", true);

	asdrText = addControl<CBitmapText>(new CBitmapText(this, CRect(sPoint(63, 81), sPoint(5, 6)), CPoint(12, 12), font, L"Enter MIDI CC"), L"asdrCC1");
	asdrText->setText(L"S", true);
	
	asdrText = addControl<CBitmapText>(new CBitmapText(this, CRect(sPoint(75, 81), sPoint(5, 6)), CPoint(12, 12), font, L"Enter MIDI CC"), L"asdrCC2");
	asdrText->setText(L"D", true);

	asdrText = addControl<CBitmapText>(new CBitmapText(this, CRect(sPoint(87, 81), sPoint(5, 6)), CPoint(12, 12), font, L"Enter MIDI CC"), L"asdrCC3");
	asdrText->setText(L"R", true);

	_graphUpdaters.emplace(addControl<CAnimKnob>(new CAnimKnob(CRect(sPoint(48, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue0"), 24, 11 * sScaleFactor, knobHandle), L"asdrCCValue0", true));
	_graphUpdaters.emplace(addControl<CAnimKnob>(new CAnimKnob(CRect(sPoint(60, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue1"), 24, 11 * sScaleFactor, knobHandle), L"asdrCCValue1", true));
	_graphUpdaters.emplace(addControl<CAnimKnob>(new CAnimKnob(CRect(sPoint(72, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue2"), 24, 11 * sScaleFactor, knobHandle), L"asdrCCValue2", true));
	_graphUpdaters.emplace(addControl<CAnimKnob>(new CAnimKnob(CRect(sPoint(84, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue3"), 24, 11 * sScaleFactor, knobHandle), L"asdrCCValue3", true));
	_currentEnableButton = nullptr;
	//----------------------------------------------------

	//Harmony
	//----------------------------------------------------
	_currentEnableButton = addControl<COnOffButton>(new COnOffButton(CRect(sPoint(99, 79), sPoint(5, 5)), this, 0, tinyCheckbox, 0), L"harmonizerEnabled");
	addControl<CSpinner>(new CSpinner(CRect(sPoint(101, 85), sPoint(11, 11)), newFrame, this, 0, 2, 0, 128, true, CHoriTxtAlign::kCenterText, false), L"harmonyKey");
	addControl<CSpinner>(new CSpinner(CRect(sPoint(113, 85), sPoint(20, 11)), newFrame, this, 0, 2, 0, 128, true, CHoriTxtAlign::kCenterText, false), L"harmonyType");
	addControl<CSpinner>(new CSpinner(CRect(sPoint(134, 85), sPoint(11, 11)), newFrame, this, 0, 2, 0, 128, true, CHoriTxtAlign::kCenterText, false), L"harmony");

	//Midi CC stuff
	//----------------------------------------------------
	CPoint knobOffset = sPoint(15, 110);
	for (int i = 0; i < 8; i++)
	{
		std::wstring indexString = std::to_wstring(i);
		_currentEnableButton = addControl<COnOffButton>(new COnOffButton(CRect(knobOffset + sPoint(-13, -10), sPoint(5, 5)), this, _plugin->getCID(L"midiKnobEnabled"), tinyCheckbox, 0), L"midiKnobEnabled" + indexString);
		addControl<CAnimKnob>(new CAnimKnob(CRect(knobOffset, sPoint(11, 11)), this, 0, 24, 11 * sScaleFactor, knobHandle), L"midiKnobValue" + indexString, true);
		CBitmapText* ccButton  = addControl<CBitmapText>(new CBitmapText(this, CRect(knobOffset + sPoint(-7, -6), sPoint(25, 4)), CPoint(12, 12), font, L"Enter MIDI CC Number", L"CC-"), L"midiKnobCC" + indexString);
		ccButton->isCCButton = true;

		addControl<CBitmapText>(new CBitmapText(this, CRect(knobOffset + sPoint(-12, 13), sPoint(34, 4)), CPoint(12, 12), font, L"Enter Name"), L"midiKnobName" + indexString);
		_currentEnableButton = nullptr;

		knobOffset.x += sCoord(36);
		if (i == 3)
		{
			knobOffset.x -= sCoord(36 * 4);
			knobOffset.y += sCoord(30);
		}
	}
	//----------------------------------------------------


	_graph = new CGraph(CRect(sPoint(6, 88), sPoint(39, 9)), _plugin);
	addControl<CGraph>(_graph);


	knobHandle->forget();
	font->forget();
	tinyCheckbox->forget();

	setExpanded(_plugin->getParameter(L"expanded")->getFloat() > 0.5f);

	return true;

//------------------------------------------------------------------------------------


}
void PluginGUI::setExpanded(bool expanded)
{
	CBitmap* background = nullptr;
	_expanded = expanded;
	if (!expanded)
	{
		background = new CBitmap(PNG_bendyBackShort);
		_expandButton->setVisible(true);
		_contractButton->setVisible(false);
	}
	else
	{
		background = new CBitmap(PNG_bendyBack);
		_expandButton->setVisible(false);
		_contractButton->setVisible(true);
	}

	frame->setBackground(background);
	frame->setSize(background->getWidth(), background->getHeight());
	frame->setDirty(true);

	_plugin->PlugHost->Dispatcher(_plugin->HostTag, FHD_EditorResized, 0, 0);
	background->forget();
}


void PluginGUI::close()
{
	//-- on close we need to delete the frame object.
	//-- once again we make sure that the member frame variable is set to zero before we delete it
	//-- so that calls to setParameter won't crash.
	CFrame* oldFrame = frame;
	frame = 0;
	delete oldFrame;

	_letters.clear();
	_graph = nullptr;
	_currentEnableButton = nullptr;
	_expandButton = nullptr;
	_graphUpdaters.clear();
}

void PluginGUI::idle()
{
	for (int i = 0; i < 5; i++)
	{
		if (_plugin->letters[i] == 0)
		{
			float val = _letters[i]->getValue();
			if (val > 0)
			{
				val = max(val - 0.1f, 0);
				_letters[i]->setValue(val);
				_letters[i]->invalid();
			}
		}
		else
		{
			_letters[i]->setValue(1);
			_letters[i]->invalid();
		}
	}

	for(auto it = _hashMap.begin(); it != _hashMap.end(); it++)
	{ 
		CControl* c = (*it).second;
		if (c->needsUpdate)
		{
			c->setParameter(c->getParameter(), _plugin);
			auto graphUpdater = _graphUpdaters.find(c);
			if (graphUpdater != _graphUpdaters.end())
				_graph->invalid();

			c->needsUpdate = false;
		}
	}
}


//------------------------------------------------------------------------------------
void PluginGUI::valueChanged(CControl* pControl)
{
	if (pControl == _expandButton)
	{
		if (pControl->getValue() > 0.5f)
		{
			setExpanded(true);
			_plugin->getParameter(L"expanded")->setFloat(1.0f);
		}
		return;
	}
	else if (pControl == _contractButton)
	{
		if (pControl->getValue() > 0.5f)
		{
			setExpanded(false);
			_plugin->getParameter(L"expanded")->setFloat(0.0f);
		}
		return;
	}

	//Pass it through as an "int" but make it back into a float later.
	float realValue = pControl->getValue();
	PlugParameter* param = pControl->getParameter();
	if (param != nullptr)
	{
		realValue = param->rangeToNormalized(realValue);

		bool uninitialized = ((int)param->getFloat()) == -1;
		_plugin->ProcessParam(param->index, (int)(realValue * FromMIDI_Max), REC_UpdateValue | REC_FromMIDI);

		auto graphUpdater = _graphUpdaters.find(pControl);
		if (graphUpdater != _graphUpdaters.end())
			_graph->invalid();

		//Enable uninitialized sections when their knobs move for the first time
		//if (uninitialized)
		//{
		//	auto controlEnable = _enableSection.find(pControl->getTag());
		//	if (controlEnable != _enableSection.end() && (*controlEnable).second->getValue() < 0.5f)
		//	{
		//		float val = 1.0f;
		//		if ((*controlEnable).second->getParameter() != nullptr)
		//			_plugin->ProcessParam((*controlEnable).second->getParameter()->index, FromMIDI_Max, REC_UpdateValue | REC_UpdateControl | REC_FromMIDI);
		//	}
		//}
	}
}

CPoint  PluginGUI::getPopupLocation()
{
	POINT _where;
	GetCursorPos(&_where);
	CPoint mousePos(static_cast<CCoord> (_where.x), static_cast<CCoord> (_where.y));
	if (auto hwnd = (HWND)frame->getSystemWindow())
	{
		ScreenToClient(hwnd, &_where);
		mousePos(static_cast<CCoord> (_where.x), static_cast<CCoord> (_where.y));
	}

	RECT r;
	GetWindowRect((HWND)frame->getSystemWindow(), &r);

	CPoint globalPos(static_cast<CCoord> (r.left), static_cast<CCoord> (r.top));
	globalPos.x = r.left;
	globalPos.y = r.top;

	mousePos = mousePos + globalPos;
	return mousePos;
}

int PluginGUI::beginPopupMenu()
{
	if (frame != nullptr)
	{
		//Win32Frame* winFrame = (Win32Frame*)_interface->getFrame()->getPlatformFrame();
		//if (winFrame != nullptr)
		//{
			HMENU hMenuPopup = CreatePopupMenu();
			return (int)hMenuPopup;
			//AppendMenu(hMenuPopup, MF_STRING, 0, L"Line");
			//AppendMenu(hMenuPopup, MF_STRING, 1, L"Rectangle");
			//AppendMenu(hMenuPopup, MF_STRING, 2, L"Circle");
			//AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
			//AppendMenu(hMenuPopup, MF_STRING, 3, L"Help");
			//BOOL val = TrackPopupMenu(hMenuPopup,
			//	TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
			//	mousePos.x, mousePos.y, 0, winFrame->getPlatformWindow(), NULL);
			//DestroyMenu(hMenuPopup);
		//}
	}
	return 0;
}

void PluginGUI::popupMenuAdd(int menu, std::wstring text, int id, bool horizontalBreak)
{
	if (horizontalBreak)
		AppendMenu((HMENU)menu, MF_STRING | MF_MENUBREAK, id, text.c_str());
	else
		AppendMenu((HMENU)menu, MF_STRING, id, text.c_str());
}

int PluginGUI::endPopupMenu(int menu)
{
	if (frame != nullptr)
	{
		CPoint mousePos = getPopupLocation();

		BOOL val = TrackPopupMenu((HMENU)menu,
			TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
			mousePos.x, mousePos.y, 0, (HWND)frame->getSystemWindow(), NULL);
		DestroyMenu((HMENU)menu);
		return (int)val;
	}
	return 0;
}

//------------------------------------------------------------------------------------
void PluginGUI::setParameter(int index, float value)
{
	auto it = _hashMap.find(index);
	if (it != _hashMap.end())
		(*it).second->needsUpdate = true;
}


void PluginGUI::showTooltip(std::wstring text)
{
	if (text == L"")
		_plugin->ShowHintMsg(nullptr);
	else
	{
		char* asciiTooltip = new char[(text.length() + 1)];
		wcstombs(asciiTooltip, text.c_str(), (text.length() + 1));
		_plugin->ShowHintMsg(asciiTooltip);
		delete asciiTooltip;
	}
}

std::wstring PluginGUI::getInput(std::wstring caption, std::wstring defaultText)
{
	char cap[256];
	wcstombs(cap, caption.c_str(), caption.length() + 1);

	char result[256];
	wcstombs(result, defaultText.c_str(), defaultText.length() + 1);

	int col = -1;
	if (_plugin->PlugHost->PromptEdit(-1, -1, cap, result, col))
	{
		std::string s(result);
		std::wstring ws(s.begin(), s.end());
		return ws;
	}

	return defaultText;
}

void PluginGUI::showDefaultControlMenu(PlugParameter* param, bool noteControlAssigned)
{
	if (frame != nullptr && param != nullptr)
	{
		bool showMinMax = dynamic_cast<ParameterFloat*>(param) != nullptr || dynamic_cast<ParameterInt*>(param) != nullptr;

		CPoint mousePos = getPopupLocation();
		ContextMenu = CreatePopupMenu();
		NoteControl1SubMenu = 0;
		NoteControl2SubMenu = 0;

		int submen1Index = 100000;
		int submen2Index = 200000;

		NoteControl1SubMenu = CreatePopupMenu();
		AppendMenu(NoteControl1SubMenu, MF_STRING, submen1Index, L"Assign Full");
		AppendMenu(NoteControl1SubMenu, MF_STRING, submen1Index + 1, L"Assign Upper");
		AppendMenu(NoteControl1SubMenu, MF_STRING, submen1Index + 2, L"Assign Lower");

		NoteControl2SubMenu = CreatePopupMenu();
		AppendMenu(NoteControl2SubMenu, MF_STRING, submen2Index, L"Assign Full");
		AppendMenu(NoteControl2SubMenu, MF_STRING, submen2Index + 1, L"Assign Upper");
		AppendMenu(NoteControl2SubMenu, MF_STRING, submen2Index + 2, L"Assign Lower");
		_plugin->AdjustParamPopup(ContextMenu, param->index, 0, DefaultMenuID);

		if (showMinMax)
		{
			AppendMenu(ContextMenu, MF_SEPARATOR, 0, NULL);
			AppendMenu(ContextMenu, MF_STRING, 1, L"Set Range Min");
			AppendMenu(ContextMenu, MF_STRING, 2, L"Set Range Max");


			if (_plugin->HasNoteControl(param->hash))
				AppendMenu(ContextMenu, MF_STRING, 3, L"Unassign Note Control");
			else
			{

				AppendMenu(ContextMenu, MF_POPUP, (UINT_PTR)NoteControl1SubMenu, L"Assign Note Control 1");
				AppendMenu(ContextMenu, MF_POPUP, (UINT_PTR)NoteControl2SubMenu, L"Assign Note Control 2");
			}
		}

		PopupParameter = param->index;
		BOOL r = TrackPopupMenu(ContextMenu, TPM_RETURNCMD | TPM_RIGHTBUTTON, mousePos.x, mousePos.y, 0, (HWND)frame->getSystemWindow(), NULL);

		if (r >= submen1Index)
		{
			int menIndex = (int)((float)r / submen1Index) - 1;
			int menSel = r % submen1Index;

			_plugin->AssignNoteControl(param->index, (NoteControlIndex)menIndex, (NoteControlType)menSel);
			int qq = 1;
		}
		else if (r == 1 || r == 2)
		{
			std::wstring input = L"";			
			if (r == 1)
			{
				input = getInput(L"Enter Min Value", std::to_wstring(param->getMin()));
				param->setMin(_wtof(input.c_str()));
			}
			else
			{
				input = getInput(L"Enter Max Value", std::to_wstring(param->getMax()));
				param->setMax(_wtof(input.c_str()));
			}
			_plugin->ProcessParam(param->index, static_cast<int>(param->getFloat()), REC_UpdateControl);
		}
		else if (r == 3)
			_plugin->AssignNoteControl(param->index, NoteControlIndex::NoControl, NoteControlType::Full);
		else if (r)
			_plugin->PlugHost->Dispatcher(_plugin->HostTag, FHD_ParamMenu, PopupParameter, r - DefaultMenuID);

		DestroyMenu(ContextMenu);
		DestroyMenu(NoteControl1SubMenu);
		DestroyMenu(NoteControl2SubMenu);

		ContextMenu = 0;
	}
}


struct MidiCCDefault
{
	MidiCCDefault(std::wstring t, int ccval)
	{
		text = t;
		cc = ccval;
	}
	std::wstring text;
	int cc;
};

int numCCDefaults = 13;
MidiCCDefault* kCCDefaults = new MidiCCDefault[]
{
	MidiCCDefault(L"Reverb (91)", 91),
	MidiCCDefault(L"Tremolo (92)", 92),
	MidiCCDefault(L"Chorus (93)", 93),
	MidiCCDefault(L"Detune (94)", 94),
	MidiCCDefault(L"Phaser (95)", 95),
	MidiCCDefault(L"Portamento Toggle (65)", 65),
	MidiCCDefault(L"Portamento Amount (84)", 84),
	MidiCCDefault(L"Sound Variance (70)", 70),
	MidiCCDefault(L"Envelope Release (72)", 72),
	MidiCCDefault(L"Envelope Attack (73)", 73),
	MidiCCDefault(L"Filter Cutoff (74)", 74),
	MidiCCDefault(L"Filter Reso (71)", 71),
	MidiCCDefault(L"Expression Pedal (11)", 11),
};


int PluginGUI::showDefaultCCMenu()
{
	if (frame != nullptr)
	{
		CPoint mousePos = getPopupLocation();
		ContextMenu = CreatePopupMenu();
		NoteControl1SubMenu = 0;
		NoteControl2SubMenu = 0;

		for (int i = 0; i < numCCDefaults; i++)
		{
			AppendMenu(ContextMenu, MF_STRING, i + 1, kCCDefaults[i].text.c_str());
		}

		BOOL r = TrackPopupMenu(ContextMenu, TPM_RETURNCMD | TPM_RIGHTBUTTON, mousePos.x, mousePos.y, 0, (HWND)frame->getSystemWindow(), NULL);
		int ret = -1;
		if (r > 0)
			ret = kCCDefaults[r - 1].cc;

		DestroyMenu(ContextMenu);
		ContextMenu = 0;
		return ret;
	}
}

