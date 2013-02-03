#include "database.h"


Database::Database() :
		QObject()
{
	isOpen = false;
}


Database::Database(const Database& other) :
		QObject()
{
	 db = other.db;
}

Database& Database::operator=(const Database& other)
{
	if (this != &other)
	{
		db = other.db;
	}
	 return *this;
}


Database::Database(QString driver,
				   QString host,
				   QString user,
				   QString password,
				   QString databaseName,
				   QObject *parent) :
    QObject(parent)
{
	isOpen = false;
	open(driver,
		 host,
		 user,
		 password,
		 databaseName);

}

Database::~Database()
{
	if (isOpen)
	{
		close();
	}

}


bool Database::open(QString driver,
					QString host,
					QString user,
					QString password,
					QString databaseName)
{
	db = QSqlDatabase::addDatabase(driver);
	db.setHostName(host);
	db.setDatabaseName(databaseName);
	db.setUserName(user);
	db.setPassword(password);

	isOpen = db.open();
#ifdef DEBUG
	qDebug("Database [%s] Opened %s",
				 databaseName.toAscii().data(),
				 isOpen ? "Yes" : "No");
#endif

	return isOpen;
}



bool Database::close()
{
	if (isOpen)
	{
		db.close();
		isOpen = false;

		#ifdef DEBUG
			qDebug("Database Closed");
		#endif
	}
	return true;
}

