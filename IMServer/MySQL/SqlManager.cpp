#include "SqlManager.h"

std::unique_ptr<SQLManager,SQLManager::deleter> SQLManager::instance = nullptr;

bool SQLManager::select(char* szSql, int nColumn, list<string>& lstStr) {
  return m_sqlConnection->SelectMySql(szSql,nColumn,lstStr);
}

bool SQLManager::update(char* szSql) { return m_sqlConnection->UpdateMySql(szSql); }

bool SQLManager::getTable(char* szSql, list<string>& lstStr) {
  return m_sqlConnection->GetTables(szSql, lstStr);
}

SQLManager::SQLManager() : m_sqlConnection(new CMySql) {
  m_sqlConnection->ConnectMySql(DB_ADDR, DB_USER, DB_PASS, DB_NAME);
}

void SQLManager::disConnect(){m_sqlConnection -> DisConnect();}

SQLManager& SQLManager::getInstance() {
  if (!instance.get()) {
    std::lock_guard<std::mutex> guard(g_lock);
    if (!instance.get()) instance.reset(new SQLManager());
  }
  return *instance.get();
}

//SQLManager::~SQLManager() {}

void SQLManager::deleter::operator()(SQLManager* ins) {
  ins->disConnect();
  delete ins;
}
