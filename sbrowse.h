#ifndef SBROWSE_H
#define SBROWSE_H

#include <QKeyEvent>
#include <QTableView>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QHeaderView>

//#include "sbqrelationaltablemodel.h"

#include "sbrowedit.h"
class SbRowEdit;

class Sbrowse : public QTableView
{
	Q_OBJECT

public:

	enum sbOpCode {SB_BROWSE, SB_INSERT, SB_UPDATE, SB_DELETE};

    QSqlQueryModel model;
    //SbQRelationalTableModel model;

	Sbrowse(QWidget *parent = 0);
	Sbrowse(QString queryStr, QWidget *parent = 0);

	virtual int currentRow();
	virtual void setQuery(QString queryStr);
	virtual QSqlQuery getQuery();

signals:

	/******
	Connect to this signal to perform row changes
	OR subclass this class and reimplement the public slot saveRowChange
	*********************/
	void beforeRowChange(int, Sbrowse *, SbRowEdit *, bool *);

	void rowChanged(Sbrowse *);
    void rowEdit(Sbrowse *);
	void rowInsert(Sbrowse *);
	void rowDelete(Sbrowse *);

public slots:

protected:

	virtual void keyPressEvent(QKeyEvent *event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseDoubleClickEvent(QMouseEvent * event);

	virtual void commonConstructor();
	virtual void configureView();

protected slots:


private:



};

#endif // SBROWSE_H
