#include "vstcontrols_ext.h"
#include <mutex>
CBitmapNumber::CBitmapNumber(const CRect& firstLetterSize, CFrame* frame, CBitmap* numberImage, int digits)
	: CViewContainer(CRect(firstLetterSize.getTopLeft(), CPoint((firstLetterSize.getWidth() + sScaleFactor) * digits, firstLetterSize.getHeight())), frame)
{
	CRect letterPosition = CRect(sPoint(0, 0), firstLetterSize.getSize());

	_numDigits = digits;
	_digits = new CMovieBitmap*[_numDigits];
	for (int i = 0; i < _numDigits; i++)
	{
		_digits[i] = new CMovieBitmap(CRect(letterPosition.getTopLeft(), letterPosition.getSize()), nullptr, 0, numberImage->getHeight() / firstLetterSize.getHeight(), firstLetterSize.getHeight(), numberImage, sPoint(0, letterPosition.getHeight()));
		letterPosition = letterPosition.offset(letterPosition.getWidth() + sScaleFactor, 0);
		addView(_digits[i]);
	}
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
CBitmapText::CBitmapText(const CRect& size, const CPoint& letterSize, CBitmap* font)
	: CView(size)
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
}

CBitmapText::~CBitmapText()
{
	_image->forget();
	_image = nullptr;
}


void CBitmapText::draw(CDrawContext* pContext)
{
	if (_image)
	{
		CPoint where = this->size.getTopLeft();
		where.x += (size.getWidth() / 2) - ((_text.length() * _letterSize.x) / 2.0f); //centered

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
