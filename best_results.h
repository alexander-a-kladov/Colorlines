#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QWidget>
#include <QDir>
#include <QTextStream>
#include <iostream>

#ifndef __BEST_RESULTS_H__
#define __BEST_RESULTS_H__

class BestResults {
public:
    BestResults() {
	db = QSqlDatabase::addDatabase("QPSQL");
	user_name = "colorlines";
    }
    
    void setParams(const char* hostname, const char* dbname, const char* user, const char* passw) {
	db.setHostName(hostname);
	db.setDatabaseName(dbname);
	db.setUserName(user);
	db.setPassword(passw);
	return;
    }
    
    void setUserName(const char* name)
    {
	user_name = name;
	return;
    }
    
    void getResults(QStringList &sl) {
	QString results;
	if (db.open()) {
	    QSqlQuery sql = QSqlQuery("select username,max(result),avg(result) from colorlines group by username order by max(result) desc;");
	    while (sql.next()) {
		results = sql.value(0).toString() + ":" + sql.value(1).toString()+":"+QString::number(sql.value(2).toUInt());
		sl.append(results);
		//std::cout << results.toAscii().data() << std::endl;
	    }
	    db.close();
	} else {
	    std::cout << "DB error = " << db.lastError().databaseText().toAscii().data() << std::endl;
	}
	return;
    }
    
    void writeResultToFile(int score_value)
    {
	QFile file("local_results.txt");
	if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
	    QTextStream ts(&file);
	    ts << QString("%1,%2\n").arg(user_name.toAscii().data()).arg(score_value);
	    file.close();
	}
	return;
    }
    
    void updateResultsFromFile()
    {
	QString result;
	bool bd_access=true;
	QFile file("local_results.txt");
	QDir dir;
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	    QTextStream ts(&file);
	    do {
		result = ts.readLine();
		if (!result.isNull()) {
		    if (!writeResult(result.section(",",0,0),
				     result.section(",",1,1).toUInt())) {bd_access=false;break;}
		}
	    } while (!result.isNull());
	    file.close();
	    if (bd_access) {
		std::cout << "bd_access ok!" << std::endl;
		dir.remove("local_results.txt");
	    } else {
		std::cout << "bd_access bad" << std::endl;
	    }
	}
	return;
    }
    
    bool writeResult(QString user_name, int score_value) {
	bool success=false;
	//std::cout << user_name.toAscii().data() << " " << score_value << std::endl;
	if (db.open()) {
	    QSqlQuery sql;
	    sql.prepare("insert into colorlines (username, result)"
			"values (:username, :result);");
	    sql.bindValue(":username", user_name);
	    sql.bindValue(":result", score_value);
	    success = sql.exec();
	    db.close();
	} else {
	    std::cout << "DB error = " << db.lastError().databaseText().toAscii().data() << std::endl;
	}
	return success;
    }
    QString user_name;
protected:
    QSqlDatabase db;
};

#endif
