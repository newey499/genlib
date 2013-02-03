#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

#include <QtSql>

class Database : public QObject
{
    Q_OBJECT
public:

	Database();


	explicit Database(	QString driver,
						QString host,
						QString user,
						QString password,
						QString databaseName,
						QObject *parent = 0);

	Database(const Database&);
	Database& operator=(const Database&);

	~Database();


	bool open(QString driver,
			  QString host,
			  QString user,
			  QString password,
			  QString databaseName);

	bool close();

	bool isOpen;

signals:

public slots:

protected:
	QSqlDatabase db;
	QString host;
	QString user;
	QString password;
	QString databaseName;

private:

};

#endif // DATABASE_H
