#include "sbqrelationaltablemodel.h"


SbQRelationalTableModel::SbQRelationalTableModel(QObject *parent) :
    QSqlRelationalTableModel(parent)
{
	setEditStrategy(SbQRelationalTableModel::OnManualSubmit);
#ifdef DEBUG
	qDebug("SbQRelationalTableMode - Edit Strategy set to [OnManualSubmit]");
#endif
}

SbQRelationalTableModel::SbQRelationalTableModel(QObject *parent, QString tableName) :
    QSqlRelationalTableModel(parent)
{
   this->setTable(tableName);
   this->select();

}


bool SbQRelationalTableModel::setHeaderDataForColumn(QString columnName, QString headerText)
{
	return setHeaderData(fieldIndex(columnName), Qt::Horizontal, headerText);
}

QVariant SbQRelationalTableModel::headerDataForColumn(QString columnName)
{
	return headerData(fieldIndex(columnName), Qt::Horizontal);
}


QSqlRelation SbQRelationalTableModel::relationForColumn(QString columnName)
{
	return relation(fieldIndex(columnName));
}


bool SbQRelationalTableModel::isRelationForColumn(QString columnName)
{
	return relation(fieldIndex(columnName)).isValid();
}


bool SbQRelationalTableModel::isPrimaryKeyField(QString fldName)
{
	QSqlIndex pkey = primaryKey();
	QSqlField fld;
	for (int i = 0; i < pkey.count(); i++)
	{
		fld = pkey.field(i);
		if (fldName == pkey.field(i).name())
		{
			return true;
		}
	}

	return false;
}



