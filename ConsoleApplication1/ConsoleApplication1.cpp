z// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class CData
{
public:
	bool _b;
	std::wstring _name;
};

void Show(std::vector<std::shared_ptr<CData>> vec)
{
	for (auto itr = vec.begin(); itr != vec.end(); itr++)
	{
		std::shared_ptr<CData> pObj = *itr;
		TCHAR sz[255];
		_stprintf_s(sz, _T("Data _b:%d _name=%s\n"), pObj->_b, pObj->_name.c_str());
		_tprintf(sz);
	}
}
	
void Change(std::vector<std::shared_ptr<CData>> vec)
{
	for (auto itr = vec.begin(); itr != vec.end(); itr++)
	{
		std::shared_ptr<CData> pObj = *itr;

		if (pObj->_name == _T("element 3"))
		{
			pObj->_b = true;
			pObj->_name += _T("_modified");
			*itr = pObj;
			break;
		}
	}
}

int main()
{
	std::vector<std::shared_ptr<CData>> _vec;

	std::shared_ptr<CData> d1 = std::make_shared<CData>();
	d1->_b = false;
	d1->_name = _T("element 1");

	_vec.push_back(d1);

	std::shared_ptr<CData> d2 = std::make_shared<CData>();
	d1->_b = false;
	d2->_name = _T("element 2");

	_vec.push_back(d2);

	std::shared_ptr<CData> d3 = std::make_shared<CData>();
	d3->_b = false;
	d3->_name = _T("element 3");

	_vec.push_back(d3);

	std::shared_ptr<CData> d4 = std::make_shared<CData>();
	d4->_b = false;
	d4->_name = _T("element 4");

	_vec.push_back(d4);

	Show(_vec);
	Change(_vec);
	Show(_vec);

    return 0;
}

