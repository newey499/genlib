#ifndef SBQTABLEVIEW_H
#define SBQTABLEVIEW_H

#include <QHeaderView>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPointer>
#include <QSqlError>
#include <QSqlRelationalDelegate>
#include <QTableView>
#include <QWidget>

#include <sbqrelationaltablemodel.h>
#include <sbrowedit.h>


class SbQTableView : public QTableView
{
	Q_OBJECT

public:
	explicit SbQTableView(QWidget *parent = 0);

	void configureView();

	void setParentView(SbQTableView *parentView);
	SbQTableView * parentView();
	void setChildView(SbQTableView * childView);
	SbQTableView * childView();

	int currentRow();

signals:

	/******
	Connect to this signal to perform row changes
	OR subclass this class and reimplement the public slot saveRowChange
	*********************/
	void beforeRowChange(int, SbQTableView *, SbRowEdit *, bool *);

	void rowChanged(SbQTableView *);
	void rowSelect(SbQTableView *);
	void rowInsert(SbQTableView *);
	void rowDelete(SbQTableView *);

public slots:


	/******
	Reimplement the public slot saveRowChange in a subclass to
	do table specific row changes
	OR connect to the beforeRowChange signal above to perform row changes
	*********************/
	virtual void saveRowChange(int, SbQTableView *, SbRowEdit *, bool *);



protected:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent * event);


protected slots:
	virtual void processClickedSignal(QModelIndex);

	virtual void slotRowUpdate(SbQTableView *);
	virtual void slotRowInsert(SbQTableView *);
	virtual void slotRowDelete(SbQTableView *);

	virtual void slotRowEditChange(int opCode, SbRowEdit *rowEdit);

private:
	QPointer<SbQTableView> pView;
	QPointer<SbQTableView> cView;


};



#endif // SBQTABLEVIEW_H
