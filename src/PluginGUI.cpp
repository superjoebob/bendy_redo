#include "PluginGUI.h"
#include "Plugin.h"
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
	background->forget(); 

	CBitmap* font = new CBitmap(PNG_biosFontCaseSensitive_halfres);
	CBitmap* knobHandle = new CBitmap(PNG_knob);
	CBitmap* tinyCheckbox = new CBitmap(PNG_tinyCheckboxSimple);

	//Main settings
	//----------------------------------------------------
	CSpinner* spinner = new CSpinner(CRect(sPoint(50, 7), sPoint(25, 11)), newFrame, this, 0, 2, 0, 16);
	spinner->setParameter(_plugin->getParameter(L"port"));
	newFrame->addView(spinner);	
	
	spinner = new CSpinner(CRect(sPoint(117, 7), sPoint(25, 11)), newFrame, this, 0, 2, 0, 64);
	spinner->setParameter(_plugin->getParameter(L"channel"));
	newFrame->addView(spinner);

	spinner = new CSpinner(CRect(sPoint(50, 21), sPoint(25, 11)), newFrame, this, 0, 2, 1, 64);
	spinner->setParameter(_plugin->getParameter(L"range"));
	newFrame->addView(spinner);

	CBitmap* checkbox = new CBitmap(PNG_checkboxTriggerNotesSimple);
	COnOffButton* enableButton = new COnOffButton(CRect(sPoint(85, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNoteTrigger"), checkbox, 0);
	enableButton->setParameter(_plugin->getParameter(L"enableNoteTrigger"));
	newFrame->addView(enableButton);
	checkbox->forget();

	checkbox = new CBitmap(PNG_checkboxVelocitySimple);
	enableButton = new COnOffButton(CRect(sPoint(100, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNoteVelocity"), checkbox, 0);
	enableButton->setParameter(_plugin->getParameter(L"enableNoteVelocity"));
	newFrame->addView(enableButton);
	checkbox->forget();

	checkbox = new CBitmap(PNG_checkboxPanningSimple);
	enableButton = new COnOffButton(CRect(sPoint(115, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNotePanning"), checkbox, 0);
	enableButton->setParameter(_plugin->getParameter(L"enableNotePanning"));
	newFrame->addView(enableButton);
	checkbox->forget();

	checkbox = new CBitmap(PNG_checkboxPitchSimple);
	enableButton = new COnOffButton(CRect(sPoint(130, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNotePitch"), checkbox, 0);
	enableButton->setParameter(_plugin->getParameter(L"enableNotePitch"));
	newFrame->addView(enableButton);
	checkbox->forget();
	//----------------------------------------------------
	
	//Midi patch
	//----------------------------------------------------
	spinner = new CSpinner(CRect(sPoint(4, 42), sPoint(88, 11)), newFrame, this, 0, 2, 0, 128, true, CHoriTxtAlign::kLeftText);
	spinner->setParameter(_plugin->getParameter(L"midiPatch"));
	newFrame->addView(spinner);

	spinner = new CSpinner(CRect(sPoint(93, 42), sPoint(25, 11)), newFrame, this, 0, 2, 0, 64);
	spinner->setParameter(_plugin->getParameter(L"midiBankCoarse"));
	newFrame->addView(spinner);

	spinner = new CSpinner(CRect(sPoint(119, 42), sPoint(25, 11)), newFrame, this, 0, 2, 0, 64);
	spinner->setParameter(_plugin->getParameter(L"midiBankFine"));
	newFrame->addView(spinner);
	//----------------------------------------------------

	//Vibrato
	//----------------------------------------------------
	CAnimKnob* vibratoKnob = new CAnimKnob(CRect(sPoint(70, 58), sPoint(11, 11)), this, _plugin->getCID(L"vibratoSpeed"), 24, 11 * sScaleFactor, knobHandle);
	vibratoKnob->setParameter(_plugin->getParameter(L"vibratoSpeed"));
	vibratoKnob->setMouseEnabled(true);
	newFrame->addView(vibratoKnob);
	
	vibratoKnob = new CAnimKnob(CRect(sPoint(132, 58), sPoint(11, 11)), this, _plugin->getCID(L"vibratoDepth"), 24, 11 * sScaleFactor, knobHandle);
	vibratoKnob->setParameter(_plugin->getParameter(L"vibratoDepth"));
	vibratoKnob->setMouseEnabled(true);
	newFrame->addView(vibratoKnob);	

	COnOffButton* enableVibrato = new COnOffButton(CRect(sPoint(39, 56), sPoint(5, 5)), this, _plugin->getCID(L"vibratoEnabled"), tinyCheckbox, 0);
	enableVibrato->setParameter(_plugin->getParameter(L"vibratoEnabled"));
	newFrame->addView(enableVibrato);


	//ASDR
	//----------------------------------------------------
	CBitmapText* asdrText = new CBitmapText(CRect(sPoint(8, 81), sPoint(38, 6)), CPoint(12, 12), font, L"Enter ASDR Name");
	asdrText->setText(L"ASDR CC");
	asdrText->setParameter(_plugin->getParameter(L"asdrCCName"));
	newFrame->addView(asdrText);

	asdrText = new CBitmapText(CRect(sPoint(51, 81), sPoint(5, 6)), CPoint(12, 12), font, L"Enter MIDI CC");
	asdrText->setText(L"A", true);
	asdrText->setParameter(_plugin->getParameter(L"asdrCC0"));
	newFrame->addView(asdrText);

	asdrText = new CBitmapText(CRect(sPoint(63, 81), sPoint(5, 6)), CPoint(12, 12), font, L"Enter MIDI CC");
	asdrText->setText(L"S", true);
	asdrText->setParameter(_plugin->getParameter(L"asdrCC1"));
	newFrame->addView(asdrText);	
	
	asdrText = new CBitmapText(CRect(sPoint(75, 81), sPoint(5, 6)), CPoint(12, 12), font, L"Enter MIDI CC");
	asdrText->setText(L"D", true);
	asdrText->setParameter(_plugin->getParameter(L"asdrCC2"));
	newFrame->addView(asdrText);

	asdrText = new CBitmapText(CRect(sPoint(87, 81), sPoint(5, 6)), CPoint(12, 12), font, L"Enter MIDI CC");
	asdrText->setText(L"R", true);
	asdrText->setParameter(_plugin->getParameter(L"asdrCC3"));
	newFrame->addView(asdrText);

	CAnimKnob* asdrKnob = new CAnimKnob(CRect(sPoint(48, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue0"), 24, 11 * sScaleFactor, knobHandle);
	asdrKnob->setParameter(_plugin->getParameter(L"asdrCCValue0"));
	asdrKnob->setMouseEnabled(true);
	newFrame->addView(asdrKnob);

	asdrKnob = new CAnimKnob(CRect(sPoint(60, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue1"), 24, 11 * sScaleFactor, knobHandle);
	asdrKnob->setParameter(_plugin->getParameter(L"asdrCCValue1"));
	asdrKnob->setMouseEnabled(true);
	newFrame->addView(asdrKnob);

	asdrKnob = new CAnimKnob(CRect(sPoint(72, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue2"), 24, 11 * sScaleFactor, knobHandle);
	asdrKnob->setParameter(_plugin->getParameter(L"asdrCCValue2"));
	asdrKnob->setMouseEnabled(true);
	newFrame->addView(asdrKnob);

	asdrKnob = new CAnimKnob(CRect(sPoint(84, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue3"), 24, 11 * sScaleFactor, knobHandle);
	asdrKnob->setParameter(_plugin->getParameter(L"asdrCCValue3"));
	asdrKnob->setMouseEnabled(true);
	newFrame->addView(asdrKnob);

	COnOffButton* asdrEnable = new COnOffButton(CRect(sPoint(2, 79), sPoint(5, 5)), this, 0, tinyCheckbox, 0);
	asdrEnable->setParameter(_plugin->getParameter(L"asdrCCEnable"));
	newFrame->addView(asdrEnable);

	//----------------------------------------------------


	CSpinner* harmonySpinner = new CSpinner(CRect(sPoint(101, 85), sPoint(11, 11)), newFrame, this, 0, 2, 0, 128, true, CHoriTxtAlign::kCenterText, false);
	harmonySpinner->setParameter(_plugin->getParameter(L"harmonyKey"));
	newFrame->addView(harmonySpinner);

	harmonySpinner = new CSpinner(CRect(sPoint(113, 85), sPoint(20, 11)), newFrame, this, 0, 2, 0, 128, true, CHoriTxtAlign::kCenterText, false);
	harmonySpinner->setParameter(_plugin->getParameter(L"harmonyType"));
	newFrame->addView(harmonySpinner);

	harmonySpinner = new CSpinner(CRect(sPoint(134, 85), sPoint(11, 11)), newFrame, this, 0, 2, 0, 128, true, CHoriTxtAlign::kCenterText, false);
	harmonySpinner->setParameter(_plugin->getParameter(L"harmony"));
	newFrame->addView(harmonySpinner);

	COnOffButton* enableHarmony = new COnOffButton(CRect(sPoint(99, 79), sPoint(5, 5)), this, 0, tinyCheckbox, 0);
	enableHarmony->setParameter(_plugin->getParameter(L"harmonizerEnabled"));
	newFrame->addView(enableHarmony);

	//Midi CC stuff
	//----------------------------------------------------
	CPoint knobOffset = sPoint(15, 110);
	for (int i = 0; i < 8; i++)
	{
		std::wstring nameString = L"midiKnobValue" + std::to_wstring(i);
		CAnimKnob* midiKnob = new CAnimKnob(CRect(knobOffset, sPoint(11, 11)), this, _plugin->getCID(nameString), 24, 11 * sScaleFactor, knobHandle);
		midiKnob->setParameter(_plugin->getParameter(nameString));
		midiKnob->setMouseEnabled(true);
		newFrame->addView(midiKnob);

		
		CBitmapText* text = new CBitmapText(CRect(knobOffset + sPoint(-7, -6), sPoint(25, 4)), CPoint(12, 12), font, L"Enter MIDI CC Number", L"CC-");
		text->setParameter(_plugin->getParameter(L"midiKnobCC" + std::to_wstring(i)));
		newFrame->addView(text);

		text = new CBitmapText(CRect(knobOffset + sPoint(-12, 13), sPoint(34, 4)), CPoint(12, 12), font, L"Enter Name");
		text->setParameter(_plugin->getParameter(L"midiKnobName" + std::to_wstring(i)));
		newFrame->addView(text);

		COnOffButton* enableButton = new COnOffButton(CRect(knobOffset + sPoint(-13, -10), sPoint(5, 5)), this, _plugin->getCID(L"midiKnobEnabled"), tinyCheckbox, 0);
		enableButton->setParameter(_plugin->getParameter(L"midiKnobEnabled" + std::to_wstring(i)));
		newFrame->addView(enableButton);

		knobOffset.x += sCoord(36);
		if (i == 3)
		{
			knobOffset.x -= sCoord(36 * 4);
			knobOffset.y += sCoord(30);
		}
	}
	//----------------------------------------------------

	knobHandle->forget();
	font->forget();
	tinyCheckbox->forget();

	frame = newFrame;
	return true;
}

//------------------------------------------------------------------------------------
void PluginGUI::close()
{
	//-- on close we need to delete the frame object.
	//-- once again we make sure that the member frame variable is set to zero before we delete it
	//-- so that calls to setParameter won't crash.
	CFrame* oldFrame = frame;
	frame = 0;
	delete oldFrame;
}

//------------------------------------------------------------------------------------
void PluginGUI::valueChanged(CControl* pControl)
{
	_plugin->ProcessParam(pControl->getTag(), static_cast<int>(pControl->getValue()), REC_UpdateValue);
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
		
		//Win32Frame* winFrame = (Win32Frame*)_interface->getFrame()->getPlatformFrame();
		//if (winFrame != nullptr)
		//{

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

			BOOL val = TrackPopupMenu((HMENU)menu,
				TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
				mousePos.x, mousePos.y, 0, (HWND)frame->getSystemWindow(), NULL);
			DestroyMenu((HMENU)menu);
			return (int)val;
		//}
	}
	return 0;
}

//------------------------------------------------------------------------------------
void PluginGUI::setParameter(int index, float value)
{
	//-- setParameter is called when the host automates one of the effects parameter.
	//-- The UI should reflect this state so we set the value of the control to the new value.
	//-- VSTGUI will automaticly redraw changed controls in the next idle (as this call happens to be in the process thread).
	//if (frame && index < kNumParameters)
	//{
	//	controls[index]->setValue(value);
	//}
}
