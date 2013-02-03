#ifndef SBQRELATIONALTABLEMODEL_H
#define SBQRELATIONALTABLEMODEL_H

#include <QSqlField>
#include <QSqlIndex>
#include <QSqlRecord>
#include <QSqlRelation>
#include <QSqlRelationalTableModel>
#include <QVariant>


class SbQRelationalTableModel : public QSqlRelationalTableModel
{
	Q_OBJECT

public:
	explicit SbQRelationalTableModel(QObject *parent = 0);
    SbQRelationalTableModel(QObject *parent, QString tableName);


	virtual bool setHeaderDataForColumn(QString columnName, QString headerText);
	virtual QVariant headerDataForColumn(QString columnName);

	virtual QSqlRelation relationForColumn(QString columnName);
	virtual bool isRelationForColumn(QString columnName);

	virtual bool isPrimaryKeyField(QString fldName);

signals:


public slots:


protected:


private:

};


#endif // SBQRELATIONALTABLEMODEL_H
