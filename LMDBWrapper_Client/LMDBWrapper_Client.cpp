// LMDBWrapper_Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\Include\CWrapper.h"
#include "..\Include\Base64.h"

int main(int argc, char * argv[])
{
	int i = 0, j = 0, rc;
	int count;
	int *values;
	char szDebug[255];
	char szVerbose[255];
	bool verbose = false;

	count = 2; //10;

	if (argc == 2)
	{
		count = atoi(argv[1]);
	}

	if (argc == 3)
	{
		count = atoi(argv[1]);
		strcpy(szVerbose, argv[2]);
		if (strcmp(szVerbose, "true") == 0)
		{
			verbose = true;
		}
	}


	std::string db = "mycache";
	CLMDBWrapperEx we;
	we.Init(db);
	we.BeginTransaction();
	std::string key = "key_v000";
	std::string value = "value_v000";
	we.Set(key, value);
	std::string value2;
	we.Get(key, value2);
	we.CommitTransaction();

	CLMDBWrapperEx we2;
	we2.Init(db);
	we2.BeginTransaction();
	std::string value3;
	we2.Get(key, value3);
	we2.CommitTransaction();
	//return 0;
	
	DWORD dwStart = 0;
	DWORD dwEnd = 0;

	CLMDBWrapperEx wr1;
	wr1.Init(db);
	dwStart = ::GetTickCount();
	//wr1.GetAllData();
	//void GetAllData()
	{
		int err;
		wr1.BeginTransactionReadOnly();

		wr1.OpenCursor();

		std::string k, value;
		int count = 0;
		while (wr1.GetFromCursor(k, value))
		{
			count++;
			//putchar('.');
			//printf("key: '%s', value: '%s'\n", k.c_str(), value.c_str());
		}
		printf("count=%d LAST key: '%s', value: '%s'\n", count, k.c_str(), value.c_str());

		wr1.CloseCursor();

		wr1.AbortTransaction();
	}

	dwEnd = ::GetTickCount();
	std::cout << "Elasped Time for GetAllData: " << dwEnd - dwStart << " ms" << std::endl;
	return 0;


	dwStart = ::GetTickCount();
	wr1.BeginTransaction();
	for (int i = 0; i < count; i++)
	{
		std::string k = "key_v" + std::to_string(i);
		std::string v = "value_v" + std::to_string(i);
		wr1.Set(k, v);
	}
	wr1.CommitTransaction();
	dwEnd = ::GetTickCount();
	std::cout << "Elasped Time for SET: " << dwEnd - dwStart << " ms" << std::endl;

	dwStart = ::GetTickCount();
	wr1.BeginTransaction();
	for (int i = 0; i < count; i++)
	{
		std::string k = "key_v" + std::to_string(i);
		std::string v;
		wr1.Get(k, v);
	}
	wr1.CommitTransaction();
	dwEnd = ::GetTickCount();
	std::cout << "Elasped Time for GET: " << dwEnd - dwStart << " ms" << std::endl;


	std::string jsonDocument = R"("User": {
		"Account": "%s@agenda.com",
			"Password" : "agenda",
			"AdjustTime" : 1,
			"FiggoURL" : "https://wendelgroup.ilucca.net/",
			"FiggoToken" : "aaaaaaaa-fb8f-41a3-a08e-84b6521ec96a",
			"EnableFiggo" : false
		})";

	dwStart = ::GetTickCount();
	wr1.BeginTransaction();
	for (int i = 0; i < count; i++)
	{
		char sz[1024];
		std::string k = "key_j" + std::to_string(i);
		sprintf_s(sz, jsonDocument.c_str(), std::to_string(i).c_str());
		std::string v = sz;
		wr1.Set(k, v);
	}
	wr1.CommitTransaction();
	dwEnd = ::GetTickCount();
	std::cout << "Elasped Time for SET jSON: " << dwEnd - dwStart << " ms" << std::endl;

	dwStart = ::GetTickCount();
	wr1.BeginTransaction();
	for (int i = 0; i < count; i++)
	{
		std::string k = "key_v" + std::to_string(i);
		std::string v;
		wr1.Get(k, v);
	}
	wr1.CommitTransaction();
	dwEnd = ::GetTickCount();
	std::cout << "Elasped Time for GET jSON: " << dwEnd - dwStart << " ms" << std::endl;

	return 0;

	char szDbName[255];
	strcpy_s(szDbName, "cache_v1000_A2");

	CLMDBWrapper::Init();

	char szKey[255];
	char szValue[255];

	sprintf(szKey, "key_%d", 10);
	sprintf(szValue, "value_%d", 10);

	CLMDBWrapper wrapper1;
	wrapper1.Init(szDbName);
	wrapper1.Set(szKey, szValue);
	wrapper1.Uninit();
	wrapper1.Init(szDbName);
	LPSTR lpszValue1;
	wrapper1.Get(szKey, &lpszValue1);
	printf("GET key:%s value:%s\n", szKey, lpszValue1);
	free(lpszValue1);
	wrapper1.Uninit();

	sprintf(szKey, "key_%d", 12);
	sprintf(szValue, "value_%d", 12);

	CLMDBWrapper wrapper2;
	wrapper2.Init(szDbName);
	wrapper2.Set(szKey, szValue, strlen(szValue));
	wrapper2.Uninit();
	wrapper2.Init(szDbName);
	LPSTR lpszValue2;
	wrapper2.Get(szKey, &lpszValue2);
	printf("GET key:%s value:%s\n", szKey, lpszValue2);
	free(lpszValue2);
	wrapper2.Uninit();
	return 0;

	TCHAR sz[255];
	for (int i = 0; i < count; i++)
	{
		Sleep(25);

		CLMDBWrapper wr;
		wr.Init(szDbName);

		_stprintf_s(sz, _T("key_%ld"), GetTickCount());
		//_stprintf_s(sz, _T("key_104481812"));
		std::wstring key = std::wstring(sz);
		std::string keya(key.begin(), key.end());
		std::wstring value2 = _T("azertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbn");
		std::string valuea2(value2.begin(), value2.end());
		//wr.Set((LPSTR)keya.c_str(), (LPSTR)valuea2.c_str());
		//wr.Set(keya, valuea2);
		wr.Set((LPSTR)keya.c_str(), (LPSTR)valuea2.c_str(), valuea2.length());
		//wr.Set(key, value2);
		valuea2 = "";
		//LPSTR lpszValue;
		//wr.Get((LPSTR)keya.c_str(), &lpszValue);
		//std::cout << "key:" << keya.c_str() << " value:" << lpszValue << std::endl;
		wr.Get(keya, valuea2);
		std::cout << "key:" << keya << " value:" << valuea2 << std::endl;
		std::wstring valuew;
		wr.Get(key, valuew);
		std::wcout << _T("key:") << key << _T(" value:") << valuew << std::endl;
		wr.Uninit();
	}
	return 0;

	CLMDBWrapper wrapper;
	wrapper.Init(szDbName);
	wrapper.Set(szKey, szValue);	
	LPSTR lpszValue;
	wrapper.Get(szKey, &lpszValue);
	printf("GET key:%s value:%s\n", szKey, lpszValue);
	free(lpszValue);

	sprintf(szKey, "key_%d", 12);
	sprintf(szValue, "value_AZERTYUIOPQSDFGHJKLMWXCVBN");
	DWORD len = strlen(szValue);
	wrapper.Set(szKey, szValue, len);
	LPSTR lpszValue3;
	wrapper.Get(szKey, &lpszValue3);
	printf("GET key:%s value:%s\n", szKey, lpszValue3);
	free(lpszValue3);
	wrapper.Uninit();
	return 0;

	printf("Adding %d values\n", count);
	dwStart = GetTickCount();
	for (i = 0; i<count; i++)
	{
		char szKey[255];
		char szValue[255];
		sprintf(szKey, "key_%d", i);
		sprintf(szValue, "value_%d", i);

		if (verbose)
		{
			printf("SET key:%s value:%s\n", szKey, szValue);
		}

		//wrapper.SetData(szKey, szValue);
	}

	char szKey2[255];
	LPSTR szValue2;
	sprintf(szKey2, "key_AA");
	if( wrapper.Get(szKey2, &szValue2) == true )
		free(szValue2);


	DWORD dwStop = GetTickCount();
	sprintf(szDebug, "Elapsed for put: %ld ms\n", dwStop - dwStart);
	printf(szDebug);

	dwStart = GetTickCount();
	for (int i = 0; i < count; i++)
	{
		char szKey[255];
		LPSTR szValue;
		sprintf(szKey, "key_%d", i);
		//memset(szValue, 0, 255);

		if (i > 101)
		{
			int i2 = 101;
		}

		if (wrapper.Get(szKey, &szValue) == true)
		{
			free(szValue);

			if (verbose)
			{
				printf("GET key:%s value:%s\n", szKey, szValue);
			}
		}

	}

	dwStop = GetTickCount();
	sprintf(szDebug, "Elapsed for get: %ld ms\n", dwStop - dwStart);
	printf(szDebug);

	wrapper.Uninit();

	return 0;

	//
	// Binary insertion
	//

	//char szKey[20];
	strcpy_s(szKey, "skyfall.mp3");

	int fd = _open("D:\\Files_Music_More\\BOF - James Bond 007\\skyfall.mp3", _O_RDONLY);
	int s = _filelength(fd);
	char * lpszBuffer = (char *)malloc(s);
	_read(fd, lpszBuffer, s);
	_close(fd);

	char szDb[255];
	strcpy_s(szDb, "cache_v1000");

	CLMDBWrapper w;
	w.Init(szDb);
	w.Set(szKey, (LPSTR) lpszBuffer, s);

	LPSTR lpsz = NULL;
	w.Get(szKey, &lpsz);

	w.Uninit();

	return 0;
}

