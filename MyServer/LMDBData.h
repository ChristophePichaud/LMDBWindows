#pragma once

class LMDBData
{
public:
	LMDBData();
	virtual ~LMDBData();

public:
	MDB_env * m_env;
	MDB_dbi m_dbi;
	MDB_txn * m_txn;
	MDB_val m_key;
	MDB_val m_data;
	MDB_stat m_mst;
};

