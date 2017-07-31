#include "prog_update.h"
#include <iostream>
#include <QTextStream>

void ProgUpdate::checkForUpdate()
{
    connect_id=connectToHost(_host_ip);
    return;
}

void ProgUpdate::finishedSlot(int id, bool error)
{
    if (id==connect_id) {
	std::cout << "Connection is " << error << std::endl;
	login_id = login(_login_name, _passwd);
    }
    else if (id==login_id) {
	std::cout << "Login is " << error << std::endl;
	std::cout << "ProgName is " << _prog_name.toAscii().data() << std::endl;
	file.setFileName(QString("/tmp/")+_prog_name);
	if (file.open(QIODevice::WriteOnly)) {
	    get_id=get(_path+"/"+_prog_name,&file);
	}
    }
    else if (id==get_id) {
	std::cout << "Get file is " << error << std::endl;
	file.close();
	QString str;
	str = QString("md5sum -c %1.md5sum > /tmp/%1.res").arg(_prog_name);
	system(str.toAscii().data());
	update();
	
    }
    return;
}

void ProgUpdate::update()
{
    QString text;
    bool isrestart=false;
    QFile file(QString("/tmp/")+_prog_name + ".res");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	QTextStream ts(&file);
	ts >> text; // Пропускаем контр. сумму
	ts >> text;
	std::cout << text.toAscii().data() << std::endl;
	if (text.count()>7) {
	    QString cmd;
	    cmd = QString("cd ../;tar xzf /tmp/%1").arg(_prog_name);
	    system(cmd.toAscii().data());
	    cmd = QString("md5sum /tmp/%1 > %1.md5sum").arg(_prog_name);
	    system(cmd.toAscii().data());
	    isrestart = true;
	}
	file.close();
    }
    if (isrestart) {
	std::cout << "Программа была обновлена требуется перезапуск" << std::endl;
	system("cat ./change.log");
	//system("at -f ./colorlines -m now");
	exit(0);
    }
    return;
}

void ProgUpdate::startedSlot(int id)
{
    if (id==get_id) {
	std::cout << "Get started" << std::endl;
    }
    return;
}
