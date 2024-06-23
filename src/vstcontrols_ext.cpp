#include "vstcontrols_ext.h"
#include "PluginGUI.h"
#include "Parameter.h"
#include "Plugin.h"
#include "../resource.h"
#include <mutex>

CBitmapNumber::CBitmapNumber(const CRect& firstLetterSize, CFrame* frame, CBitmap* numberImage, int digits)
	: CViewContainer(CRect(firstLetterSize.getTopLeft(), CPoint((firstLetterSize.getWidth() + sScaleFactor) * digits, firstLetterSize.getHeight())), frame)
{
	CRect letterPosition = CRect(sPoint(0, 0), firstLetterSize.getSize());
	setBackgroundColor(MakeCColor(0, 0, 0, 0));

	_numDigits = digits;
	_digits = new CMovieBitmap*[_numDigits];
	for (int i = 0; i < _numDigits; i++)
	{
		_digits[i] = new CMovieBitmap(CRect(letterPosition.getTopLeft(), letterPosition.getSize()), nullptr, 0, numberImage->getHeight() / firstLetterSize.getHeight(), firstLetterSize.getHeight(), numberImage, sPoint(0, letterPosition.getHeight()));
		letterPosition = letterPosition.offset(letterPosition.getWidth() + sScaleFactor, 0);
		addView(_digits[i]);
	}

	setMouseableArea(size);
}

int numDigits(int number)
{
	if (number == 0)
		return 1;

	int digits = 0;
	while (number) {
		number /= 10;
		digits++;
	}
	return digits;
}

void CBitmapNumber::setNumber(int number)
{
	int numberOfDigits = numDigits(number);
	char* numbers = new char[32];
	_itoa(number, numbers, 10);

	if (numberOfDigits > _numDigits)
		memset(numbers, '-', sizeof(char) * numberOfDigits);

	for (int i = _numDigits - 1; i >= 0; i--)
	{
		if (numberOfDigits - 1 < 0)
			_digits[i]->setOffset(CPoint(0, 0));
		else
		{
			char c = numbers[numberOfDigits - 1];
			if (c >= 48 && c <= 57)
				_digits[i]->setOffset(CPoint(0, ((c - 48) + 1) * _digits[i]->getHeightOfOneImage()));
			else if (c == 45)
				_digits[i]->setOffset(CPoint(0, 11 * _digits[i]->getHeightOfOneImage()));
		}

		numberOfDigits--;
	}

	delete[] numbers;
}

CBitmapNumber::~CBitmapNumber()
{
	for (int i = 0; i < _numDigits; i++)
	{
		removeView(_digits[i]);
		_digits[i] = nullptr;
	}
	delete[] _digits;
	_digits = nullptr;
}

int _charactersLength = 317;
char* _characters = new char[]
{
	' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '>', '=', '<', '?',
		'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
		'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
		'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
		'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', '`',
		'À', 'Á', 'Â', 'Ã', 'Ä', 'Å', 'Æ', 'Ç', 'È', 'É', 'Ê', 'Ë', 'Ì', 'Í', 'Î', 'Ï',
		'Ð', 'Ñ', 'Ò', 'Ó', 'Ô', 'Õ', 'Ö', 'Ø', 'Ù', 'Ú', 'Û', 'Ü', 'Ý', 'Þ', 'ß', 'à',
		'á', 'â', 'ã', 'ä', 'å', 'æ', 'ç', 'è', 'é', 'ê', 'ë', 'ì', 'í', 'î', 'ï', 'ð',
		'ñ', 'ò', 'ó', 'ô', 'õ', 'ö', 'ø', 'ù', 'ú', 'û', 'ü', 'ý', 'þ', 'ÿ', 'Ā', 'ā',
		'Ă', 'ă', 'Ą', 'ą', 'Ć', 'ć', 'Ċ', 'ċ', 'Č', 'č', 'Ď', 'ď', 'Ē', 'ē', 'Ę', 'ę',
		'Ě', 'ě', 'Ğ', 'ğ', 'Ġ', 'ġ', 'Ģ', 'ģ', 'Ħ', 'ħ', 'Ī', 'ī', 'Į', 'į', 'İ', 'ı',
		'Ĳ', 'ĳ', 'Ķ', 'ķ', 'Ĺ', 'ĺ', 'Ļ', 'ļ', 'Ľ', 'ľ', 'Ł', 'ł', 'Ń', 'ń', 'Ņ', 'ņ',
		'Ň', 'ň', 'Ō', 'ō', 'Ő', 'ő', 'Œ', 'œ', 'Ŕ', 'ŕ', 'Ř', 'ř', 'Ś', 'ś', 'Ş', 'ş',
		'Š', 'š', 'Ţ', 'ţ', 'Ť', 'ť', 'Ū', 'ū', 'Ů', 'ů', 'Ű', 'ű', 'Ų', 'ų', 'Ÿ', 'Ź',
		'ź', 'Ż', 'ż', 'Ž', 'ž', 'ǅ', 'ǆ', 'ǲ', 'ǳ', 'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ж',
		'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц',
		'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я', 'а', 'б', 'в', 'г', 'д', 'е', 'ж',
		'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц',
		'ч', 'ш', 'щ', 'ъ', 'ы', 'ь', 'э', 'ю', 'я', '¡', '¿', 'Ё', 'ё'
};

std::mutex _charmapMutex;
int* _characterMap = nullptr;
CBitmapText::CBitmapText(const CRect& size, const CPoint& letterSize, CBitmap* font, std::wstring inputCaption, std::wstring prefix, std::wstring suffix)
	: CView(size),
	_numDigits(2),
	_align(CHoriTxtAlign::kCenterText),
	_lockText(false)
{
	_letterSize = letterSize;
	_image = font;
	_image->remember();

	_charmapMutex.lock();
	if (_characterMap == nullptr)
	{
		_characterMap = new int[WCHAR_MAX / 4];
		for (int i = 0; i < WCHAR_MAX / 4; i++)
		{
			char c = (char)i;
			_characterMap[i] = '[';
			for (int iChar = 0; iChar < _charactersLength; iChar++)
			{
				if (_characters[iChar] == c)
				{
					_characterMap[i] = iChar;
					break;
				}
			}
		}
	}
	_charmapMutex.unlock();
	_inputCaption = inputCaption;
	_prefix = prefix;
	_suffix = suffix;
}

CBitmapText::~CBitmapText()
{
	_image->forget();
	_image = nullptr;
}

CMouseEventResult CBitmapText::onMouseDown(CPoint& where, const long& buttons)
{
	if (_inputCaption != L"" && getParameter() != nullptr)
	{
		char caption[256];
		wcstombs(caption, _inputCaption.c_str(), _inputCaption.length() + 1);

		std::wstring valueString = getParameter()->toString();
		char result[256];
		wcstombs(result, valueString.c_str(), valueString.length() + 1);

		int col = -1;
		if (((PluginGUI*)getEditor())->getPlugin()->PlugHost->PromptEdit(-1, -1, caption, result, col))
		{
			std::string s(result);
			std::wstring ws(s.begin(), s.end());
			getParameter()->setFromString(ws);

			if(_lockText == false)
				setText(getParameter()->toString());
		}
	}
	return CMouseEventResult::kMouseEventHandled;
}

void CBitmapText::setParameter(PlugParameter* param)
{
	if(param != nullptr && _lockText == false)
		setText(param->toString());

	CBaseObject::setParameter(param);
}

void CBitmapText::draw(CDrawContext* pContext)
{
	if (_image)
	{
		CPoint where = this->size.getTopLeft();

		if(_align == CHoriTxtAlign::kCenterText)
			where.x += (size.getWidth() / 2) - ((_text.length() * _letterSize.x) / 2.0f);
		else if(_align == CHoriTxtAlign::kRightText)
			where.x += (size.getWidth()) - (_text.length() * _letterSize.x);

		int charsWide = _image->getWidth() / _letterSize.x;
		for (int i = 0; i < _text.length(); i++)
		{
			wchar_t c = _text[i];
			if (c > 0 && c < _charactersLength)
			{
				int charIndex = _characterMap[c];

				CPoint charOffset = CPoint((charIndex % charsWide) * _letterSize.x, (charIndex / charsWide) * _letterSize.y);
				if (bTransparencyEnabled)
					_image->drawTransparent(pContext, CRect(where, _letterSize), charOffset);
				else
					_image->draw(pContext, CRect(where, _letterSize), charOffset);

				where.x += _letterSize.x;
			}
		}
	}

	setDirty(false);
}

CSpinner::CSpinner(const CRect& size, CFrame* frame, CControlListener* listener, int tag, int digits, int min, int max, bool textMode, CHoriTxtAlign align, bool showButtons) :
	CControl(size, listener, tag)
{
	//setBackgroundColor(MakeCColor(0, 0, 0, 0));
	CBitmap* letter = new CBitmap(PNG_bendyNumbers);
	vmin = min;
	vmax = max;



	//CBitmapText::CBitmapText(const CRect & size, const CPoint & letterSize, CBitmap * font, std::wstring inputCaption, std::wstring prefix, std::wstring suffix)
	if (textMode)
	{
		float widthSub = 38;
		if (!showButtons)
			widthSub -= (9 * 2);

		CBitmap* font = new CBitmap(PNG_biosFontCaseSensitive_halfres);
		_text = new CBitmapText(CRect(CPoint(size.getTopLeft().x + 8, size.getTopLeft().y + ((size.getHeight() / 2) - 4)), CPoint(size.getWidth() - widthSub, size.getHeight())), CPoint(12, 12), font);
		_text->setAlign(align);

		frame->addView(_text);

		_number = nullptr;
		font->forget();
	}
	else
	{
		_number = new CBitmapNumber(CRect(CPoint((size.getWidth() - sCoord(11) - (digits * letter->getWidth())), sCoord(2)), sPoint(6, 7)).offset(size.x, size.y), frame, letter, 2);
		frame->addView(_number);

		_text = nullptr;
	}

	if (showButtons)
	{
		CBitmap* spinnerUpBitmap = new CBitmap(PNG_bendySpinnerUp);
		CBitmap* spinnerDownBitmap = new CBitmap(PNG_bendySpinnerDown);
		CKickButton* spinnerUp = new CKickButton(CRect(CPoint(size.getWidth() - sCoord(9), 0), sPoint(9, 6)).offset(size.x, size.y), this, 1, spinnerUpBitmap);
		frame->addView(spinnerUp);
		CKickButton* spinnerDown = new CKickButton(CRect(CPoint(size.getWidth() - sCoord(9), sCoord(5)), sPoint(9, 6)).offset(size.x, size.y), this, 2, spinnerDownBitmap);
		frame->addView(spinnerDown);
		spinnerUpBitmap->forget();
		spinnerDownBitmap->forget();
	}

	letter->forget();

	CRect r2 = size;

	if(showButtons)
		r2.right -= sCoord(9);

	setMouseableArea(r2);

	_listener = listener;
}


void CSpinner::valueChanged(VSTGUI::CControl* pControl)
{
	if (pControl->getValue() == 1)
	{
		if (pControl->getTag() == 1)
			setValue(value + 1);
		else if (pControl->getTag() == 2)
			setValue(value - 1);
	}
	else
		setValue(value);

	if(_listener != nullptr)
		_listener->valueChanged(this);
}

void CSpinner::setValue(float value)
{
	if (value < getMin())
		value = getMin();
	if (value > getMax())
		value = getMax();

	CControl::setValue(value);

	//if (getParameter() != nullptr)
	//	getParameter()->setFromString(std::to_wstring((int)value));

	if (_number != nullptr)
		_number->setNumber((int)this->value);
	else if(_text != nullptr && getParameter() != nullptr)
		_text->setText(getParameter()->toReadable());

	invalid();
}

void CSpinner::setParameter(PlugParameter* param)
{
	CControl::setParameter(param);
	if (param != nullptr)
		setValue(value);
}

void CSpinner::draw(CDrawContext* pContext) {}

bool CSpinner::onWheel(const CPoint& where, const float& distance, const long& buttons)
{
	if (!bMouseEnabled)
		return false;

	if (buttons & kShift)
		value += 10 * distance;
	else
		value += 1 * distance;

	bounceValue();

	if (isDirty())
	{
		// begin of edit parameter
		beginEdit();

		valueChanged(this);

		// end of edit parameter
		endEdit();

		invalid();
	}

	setValue(value);

	return true;
}

CMouseEventResult CSpinner::onMouseDown(CPoint& where, const long& buttons)
{
	if (_text != nullptr)
	{
		ParameterStringSelection* sel = dynamic_cast<ParameterStringSelection*>(getParameter());
		if (sel != nullptr)
		{
			int menu = ((PluginGUI*)getEditor())->beginPopupMenu();

			for (int i = 0; i < sel->strings.size(); i++)
			{
				((PluginGUI*)getEditor())->popupMenuAdd(menu, sel->toReadable(i, false), i, i % 32 == 0);
			}

			int selection = ((PluginGUI*)getEditor())->endPopupMenu(menu);
			value = selection;
			beginEdit();
			valueChanged(this);
			endEdit();
			setValue(value);
		}
	}

	return CMouseEventResult::kMouseEventHandled;
}