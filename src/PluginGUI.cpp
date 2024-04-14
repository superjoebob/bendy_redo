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

	CBitmap* checkbox = new CBitmap(PNG_checkboxTriggerNotesSimple);
	COnOffButton* enableButton = new COnOffButton(CRect(sPoint(85, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNoteTrigger"), checkbox, 0);
	enableButton->setValue(_plugin->getValue(L"enableNoteTrigger"));
	newFrame->addView(enableButton);
	checkbox->forget();

	checkbox = new CBitmap(PNG_checkboxVelocitySimple);
	enableButton = new COnOffButton(CRect(sPoint(100, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNoteVelocity"), checkbox, 0);
	enableButton->setValue(_plugin->getValue(L"enableNoteVelocity"));
	newFrame->addView(enableButton);
	checkbox->forget();

	checkbox = new CBitmap(PNG_checkboxPanningSimple);
	enableButton = new COnOffButton(CRect(sPoint(115, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNotePanning"), checkbox, 0);
	enableButton->setValue(_plugin->getValue(L"enableNotePanning"));
	newFrame->addView(enableButton);
	checkbox->forget();

	checkbox = new CBitmap(PNG_checkboxPitchSimple);
	enableButton = new COnOffButton(CRect(sPoint(130, 21), sPoint(11, 11)), this, _plugin->getCID(L"enableNotePitch"), checkbox, 0);
	enableButton->setValue(_plugin->getValue(L"enableNotePitch"));
	newFrame->addView(enableButton);
	checkbox->forget(); 


	CBitmap* letter = new CBitmap(PNG_bendyNumbers);

	CBitmapNumber* spinnerNumber = new CBitmapNumber(CRect(sPoint(52, 9), sPoint(6, 7)), newFrame, letter, 2);
	newFrame->addView(spinnerNumber);
	spinnerNumber->setNumber(69);	
	
	spinnerNumber = new CBitmapNumber(CRect(sPoint(119, 9), sPoint(6, 7)), newFrame, letter, 2);
	newFrame->addView(spinnerNumber);
	spinnerNumber->setNumber(420);	
	
	spinnerNumber = new CBitmapNumber(CRect(sPoint(52, 23), sPoint(6, 7)), newFrame, letter, 2);
	newFrame->addView(spinnerNumber);
	spinnerNumber->setNumber(0);


	letter->forget();



	CBitmap* knobHandle = new CBitmap(PNG_knob);
	CAnimKnob* asdrKnob = new CAnimKnob(CRect(sPoint(48, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue0"), 24, 11 * sScaleFactor, knobHandle);
	asdrKnob->setValue(_plugin->getValue(L"asdrCCValue0"));
	asdrKnob->setMouseEnabled(true);
	newFrame->addView(asdrKnob);

	asdrKnob = new CAnimKnob(CRect(sPoint(60, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue1"), 24, 11 * sScaleFactor, knobHandle);
	asdrKnob->setValue(_plugin->getValue(L"asdrCCValue1"));
	asdrKnob->setMouseEnabled(true);
	newFrame->addView(asdrKnob);

	asdrKnob = new CAnimKnob(CRect(sPoint(72, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue2"), 24, 11 * sScaleFactor, knobHandle);
	asdrKnob->setValue(_plugin->getValue(L"asdrCCValue2"));
	asdrKnob->setMouseEnabled(true);
	newFrame->addView(asdrKnob);

	asdrKnob = new CAnimKnob(CRect(sPoint(84, 87), sPoint(11, 11)), this, _plugin->getCID(L"asdrCCValue3"), 24, 11 * sScaleFactor, knobHandle);
	asdrKnob->setValue(_plugin->getValue(L"asdrCCValue3"));
	asdrKnob->setMouseEnabled(true);
	newFrame->addView(asdrKnob);

	knobHandle->forget();



	CBitmap* font = new CBitmap(PNG_biosFontCaseSensitive_halfres);
	CBitmapText* text = new CBitmapText(CRect(sPoint(3, 123), sPoint(34, 4)), CPoint(12, 12), font);
	text->setText(L"BOBBAWOB");
	newFrame->addView(text);
	font->forget();
	////-- load some bitmaps we need
	//CBitmap* background = new CBitmap("KnobBackground.png");
	//CBitmap* handle = new CBitmap("KnobHandle.png");

	////-- creating a knob and adding it to the frame
	//CRect r(0, 0, background->getWidth(), background->getHeight());
	//CKnob* knob1 = new CKnob(r, this, kLeftVolumeParameter, background, handle, CPoint(0, 0));
	//newFrame->addView(knob1);

	////-- creating another knob, we are offsetting the rect, so that the knob is next to the previous knob
	//r.offset(background->getWidth() + 5, 0);
	//CKnob* knob2 = new CKnob(r, this, kRightVolumeParameter, background, handle, CPoint(0, 0));
	//newFrame->addView(knob2);

	////-- forget the bitmaps
	//background->forget();
	//handle->forget();

	////-- remember our controls so that we can sync them with the state of the effect
	//controls[kLeftVolumeParameter] = knob1;
	//controls[kRightVolumeParameter] = knob2;

	//-- set the member frame to the newly created frame
	//-- we do it this way because it is possible that the setParameter method is called 
	//-- in between of constructing the frame and it's controls
	frame = newFrame;

	//-- sync parameters
	//for (int i = 0; i < kNumParameters; i++)
	//	setParameter(i, effect->getParameter(i));

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
	//-- valueChanged is called whenever the user changes one of the controls in the User Interface (UI)
	//effect->setParameterAutomated(pControl->getTag(), pControl->getValue());
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
