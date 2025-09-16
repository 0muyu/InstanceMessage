#ifndef SQLMANAGER_H

#define SQLMANAGER_H
#include "../config.h"
#include "CMySql.h"

#define SQLselect(sql,column,list) SQLManager::getInstance().select(sql,(column),list)
#define SQLupdate(sql) SQLManager::getInstance().update(sql)
#define SQLgetTab(sql,list) SQLManager::getInstance().getTable(sql,list)

class SQLManager {
 public:
  static SQLManager &getInstance();

  /// <summary>
  /// 查询数据库
  /// </summary>
  /// <param name="szSql">要执行的 SQL 语句</param>
  /// <param name="nColumn">返回列数</param>
  /// <param name="lstStr">结果列表</param>
  /// <returns>是否执行成功</returns>
  bool select(char *szSql, int nColumn, list<string> &lstStr);
  bool update(char *szSql);
  bool getTable(char *szSql, list<string> &lstStr);

 private:
  SQLManager();
  SQLManager(const SQLManager &) = delete;
  SQLManager &operator=(const SQLManager &) = delete;
  //~SQLManager();

  //^定义删除器(仿函数)
  struct deleter {
    void operator()(SQLManager *ins);
  };

  static std::unique_ptr<SQLManager, deleter> instance;

  void disConnect();

 private:
  std::unique_ptr<CMySql> m_sqlConnection;
};

#endif  // !SQLMANAGER_H
