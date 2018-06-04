#include "StdAfx.h"
#include "LMDBData.h"

LMDBData::LMDBData()
{
	m_dbi = 0;
	m_env = nullptr;
	m_txn = nullptr;
}

LMDBData::~LMDBData()
{
}
