#pragma once
#include "vstcontrols.h"
#include <string>
#include <algorithm>
#include <vector>

class Plugin;
//struct ControlPair
//{
//	ControlPair(Plugin* plugin, std::wstring id):
//		plugin(plugin),
//		id(id)
//	{
//
//	}
//	ControlPair(const ControlPair& pair) :
//		plugin(pair.plugin),
//		id(pair.id)
//	{
//
//	}
//
//
//	Plugin* plugin;
//	std::wstring id;
//};


class CBitmapNumber : public CViewContainer
{
public:
	CBitmapNumber(const CRect& firstLetterSize, CFrame* frame, CBitmap* numberImage, int digits);
	virtual ~CBitmapNumber();

	void setNumber(int number);

private:
	CMovieBitmap** _digits;
	int _numDigits;
};

class CBitmapText : public CView
{
public:
	CBitmapText(const CRect& size, const CPoint& letterSize, CBitmap* font, std::wstring inputCaption = L"", std::wstring prefix = L"", std::wstring suffix = L"");
	virtual ~CBitmapText();
	virtual CMouseEventResult onMouseDown(CPoint& where, const long& buttons) override;

	void setText(std::wstring t, bool lockText = false) { _text = _prefix + t + _suffix; invalid(); _lockText = lockText; }
	void setAlign(CHoriTxtAlign align) { _align = align; }
	virtual void setParameter(PlugParameter* param) override;
	virtual void draw(CDrawContext* pContext) override;
	CLASS_METHODS(CBitmapText, CView)

private:
	CPoint _letterSize;
	CBitmap* _image;
	std::wstring _text;
	std::wstring _inputCaption;
	std::wstring _prefix;
	std::wstring _suffix;
	CHoriTxtAlign _align;
	int _numDigits;
	bool _lockText;
};

class CSpinner : public CControl, CControlListener
{
public:
	CSpinner(const CRect& size, CFrame* frame, CControlListener* listener, int tag, int digits, int min, int max, bool textMode = false, CHoriTxtAlign align = CHoriTxtAlign::kRightText, bool showButtons = true);
	//~CSpinner();
	virtual void valueChanged(VSTGUI::CControl* pControl);
	virtual void setValue(float value) override;
	virtual void setParameter(PlugParameter* param) override;
	virtual void draw(CDrawContext* pContext) override;
	virtual bool onWheel(const CPoint& where, const float& distance, const long& buttons) override;
	virtual CMouseEventResult onMouseDown(CPoint& where, const long& buttons) override;
	CLASS_METHODS(CSpinner, CControl)

private:
	CBitmapNumber* _number;
	CBitmapText* _text;
	CControlListener* _listener;
	//ControlPair _control;
};


struct GraphPart
{
	GraphPart()
		:widthParam(nullptr),
		heightParam(nullptr),
		fixedWidth(0),
		fixedHeight(0)
	{

	}
	PlugParameter* widthParam;
	PlugParameter* heightParam;
	float fixedWidth;
	float fixedHeight;

	float getWidthValue();
	float getHeightValue();
};

class CGraph : public CView
{
public:
	CGraph(const CRect& size);

	virtual void setParameter(PlugParameter* param) override;
	virtual void draw(CDrawContext* context) override;
	CLASS_METHODS(CGraph, CView)

	std::vector<GraphPart> parts;
};