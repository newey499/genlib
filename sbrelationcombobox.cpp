#include "sbrelationcombobox.h"


SbRelationComboBox::SbRelationComboBox(QWidget *parent) :
    QComboBox(parent)
{
}


SbRelationComboBox::SbRelationComboBox(SbQRelationalTableModel *model, QSqlRelation relation, QWidget *parent) :
		QComboBox(parent)
{
	this->relation = relation;
	this->model = model;
	connect(this,SIGNAL(currentIndexChanged(QString)),this,SLOT(currentIndexChanged(QString)));
	QString qryStr = QString("SELECT %1, %2 FROM %3").
			arg(relation.indexColumn()).
			arg(relation.displayColumn()).
			arg(relation.tableName());

	QSqlQuery qry(qryStr);


	while (qry.next())
	{
		keys.append(qry.value(0).toInt());
		this->addItem(qry.value(1).toString());
	}

#ifdef DEBUG
	qDebug("display col [%s] index col [%s] tablename [%s]",
				 relation.displayColumn().toAscii().data(),
				 relation.indexColumn().toAscii().data(),
				 relation.tableName().toAscii().data());
#endif
}


void SbRelationComboBox::currentIndexChanged (const QString & text)
{
#ifdef DEBUG
	qDebug("currentIndexChanged [%s] displaycol [%s] indexcol [%s]",
				 text.toAscii().data(),
				 relation.displayColumn().toAscii().data(),
				 relation.indexColumn().toAscii().data()
				 );
#endif
}

int SbRelationComboBox::key()
{
	return keys.at(currentIndex());
}
