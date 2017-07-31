#include <QFtp>
#include <QFile>

#ifndef __PROG_UPDATE_H__
#define __PROG_UPDATE_H__

class ProgUpdate : public QFtp {
    Q_OBJECT
public:
    ProgUpdate() : QFtp() {
	connect_id = -1;
	get_id = -1;
	connect(this, SIGNAL(commandFinished(int,bool)), SLOT(finishedSlot(int,bool)));
	connect(this, SIGNAL(commandStarted(int)), SLOT(startedSlot(int)));
    };
    
    void setHostIP(QString host_ip)
    {
	_host_ip = host_ip;
	return;
    }
    
    void setProgName(QString prog_name)
    {
	_prog_name = prog_name;
	return;
    }
    
    void setPathName(QString path)
    {
	_path = path;
    }
    
    void setLoginName(QString login)
    {
	_login_name = login;
    }
    
    void setPasswd(QString passwd)
    {
	_passwd = passwd;
    }
    
    void update();
    void checkForUpdate();
protected slots:
    void finishedSlot(int id, bool error);
    void startedSlot(int id);
protected:
    int connect_id,get_id,login_id;
    QFile file;
    QString _host_ip, _prog_name, _path, _login_name, _passwd;
};

#endif
