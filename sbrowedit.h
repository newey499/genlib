#ifndef SBROWEDIT_H
#define SBROWEDIT_H

#include <QDateEdit>
#include <QDateTimeEdit>
#include <QDataWidgetMapper>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QModelIndex>
#include <QPointer>
#include <QPushButton>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include <QTableWidgetItem>
#include <QTimeEdit>
#include <QVariant>
#include <QWidget>

#include <sbrelationcombobox.h>
//#include <sbqtableview.h>
#include <sbqrelationaltablemodel.h>


class SbQTableView;


class SbRowEdit : public QWidget
{
    Q_OBJECT

public:

	enum recOpCode {REC_DISPLAY, REC_EDIT, REC_INSERT, REC_DELETE};

	explicit SbRowEdit(QWidget *parent = 0);
	SbRowEdit(const int opCode, SbQRelationalTableModel *model,
				SbQTableView *view, QWidget *parent = 0);
	SbRowEdit(const int opCode,
				SbQTableView *view, QWidget *parent = 0);


	~SbRowEdit();

	void exec();
	virtual void addEditFields();

	QDataWidgetMapper mapper;
	QFormLayout formLayout;
	QHBoxLayout layout;

	QPushButton btnDoAction;
	QPushButton btnCancel;

	int opCode;

	SbQRelationalTableModel *model;
	SbQTableView *view;

signals:
	void rowChange(int opCode, SbRowEdit *rowEdit);

public slots:
	void performTask();
	void cancelTask();

protected:

	QWidget * createEditWidget(int columnNumber);
	void displayRecord(QSqlRecord rec);
	QString getFieldType(QSqlField fld);

private:

		void commonConstructor();

};


#endif // SBROWEDIT_H
