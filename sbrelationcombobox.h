#ifndef SBRELATIONCOMBOBOX_H
#define SBRELATIONCOMBOBOX_H

#include <QComboBox>
#include <QList>
#include <QModelIndex>
#include <QSqlQuery>
#include <QSqlRelation>
#include <QWidget>

#include "sbqrelationaltablemodel.h"


class SbRelationComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit SbRelationComboBox(QWidget *parent = 0);
		SbRelationComboBox(SbQRelationalTableModel *model, QSqlRelation relation, QWidget *parent = 0);

		virtual int key();

signals:

public slots:

protected slots:

		void currentIndexChanged (const QString & text);

protected:

		QSqlRelation relation;

		SbQRelationalTableModel *model;


private:

		QList<int> keys;	// key values for relation

};


#endif // SBRELATIONCOMBOBOX_H
