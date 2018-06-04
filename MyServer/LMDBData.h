#pragma once

class LMDBData
{
public:
	LMDBData();
	virtual ~LMDBData();

public:
	bool m_Init = false;
	MDB_env * m_env;
	MDB_dbi m_dbi;
	MDB_txn * m_txn;
};

