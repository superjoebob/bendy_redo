#pragma once
#include "vstcontrols.h"
#include <string>
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
	CBitmapText(const CRect& size, const CPoint& letterSize, CBitmap* font);
	virtual ~CBitmapText();

	void setText(std::wstring t) { _text = t; invalid(); }
	virtual void draw(CDrawContext* pContext) override;

private:
	CPoint _letterSize;
	CBitmap* _image;
	std::wstring _text;
	int _numDigits;
};