#include "sbrowedit.h"

#include <sbqtableview.h>


SbRowEdit::SbRowEdit(QWidget *parent) :
    QWidget(parent)
{
	opCode = REC_DISPLAY;
	this->model = 0;
	this->view = 0;
	commonConstructor();
}

SbRowEdit::SbRowEdit(const int opCode, SbQRelationalTableModel *model, SbQTableView *view, QWidget *parent) :
		QWidget(parent)
{
	this->opCode = opCode;
	this->model = model;
	this->view = view;
	commonConstructor();
}


SbRowEdit::SbRowEdit(const int opCode, SbQTableView *view, QWidget *parent) :
		QWidget(parent)
{
	this->opCode = opCode;
	this->model = (SbQRelationalTableModel *) view->model();
	this->view = view;
	commonConstructor();
}

void SbRowEdit::commonConstructor()
{
	mapper.setModel(model);

	switch (opCode)
	{
		case SbRowEdit::REC_EDIT:
					btnDoAction.setText("&Save");
					btnCancel.setText("&Cancel");
					break;

		case SbRowEdit::REC_INSERT:
					btnDoAction.setText("&Save");
					btnCancel.setText("&Cancel");
					break;

		case SbRowEdit::REC_DELETE:
					btnDoAction.setText("&Delete");
					btnCancel.setText("&Cancel");
					break;

		case SbRowEdit::REC_DISPLAY:
		default:
					btnDoAction.setText("&Display");
					btnDoAction.hide();
					btnCancel.setText("&Close");
					break;

	}

	connect(&btnDoAction,SIGNAL(clicked()), this, SLOT(performTask()));
	connect(&btnCancel,SIGNAL(clicked()), this, SLOT(cancelTask()));

	layout.addWidget(&btnDoAction);
	layout.addWidget(&btnCancel);

	formLayout.addRow(&layout);

	addEditFields();

	formLayout.addRow(&layout);

	setLayout(&formLayout);

	move(view->x() + 10, view->y() + 10);

	setWindowModality(Qt::ApplicationModal);

}




SbRowEdit::~SbRowEdit()
{

}

void SbRowEdit::addEditFields()
{

	for (int i = (model->columnCount() - 1); i >= 0; i--)
	{
		if (view->isColumnHidden(i))
		{
			continue;
		}
		QString columnTitle = model->headerData(i, Qt::Horizontal).toString();
		QSqlRelation relation = model->relation(i);
		if (relation.isValid())
		{
			// look up column - build a combobox
			SbRelationComboBox *cbx = new SbRelationComboBox(model, relation, this);
			QString columnName = model->record(view->currentIndex().row()).fieldName(i);

			cbx->setObjectName(columnName);

			// need to set cbx to proper initial value
			qDebug("cbx set to [%s]",
						 model->record(view->currentIndex().row()).value(i).toString().toAscii().data()
						);

			cbx->setEditText(model->record(view->currentIndex().row()).value(i).toString());
			int index = cbx->findText(model->record(view->currentIndex().row()).value(i).toString());
			cbx->setCurrentIndex(index);
			//mapper.addMapping(cbx, i);
			formLayout.insertRow(0, columnTitle + ";", cbx);
#ifdef DEBUG
			qDebug("object name [%s]", cbx->objectName().toAscii().data());
#endif
		}
		else
		{
			// not a lookup column - build appropriate edit widget
			QWidget *edt = createEditWidget(i);


			formLayout.insertRow(0, columnTitle + ";", edt);
#ifdef DEBUG
			qDebug("object name [%s]", edt->objectName().toAscii().data());
#endif
		}

#ifdef DEBUG
		qDebug("columnName [%s] columnTitle [%s] isRelation [%s]",
					 model->record(view->currentIndex().row()).fieldName(i).toAscii().data(),

					 columnTitle.toAscii().data(),
					 (relation.isValid() ? "Yes" : "No")
					 );
#endif
		// formLayout.insertRow(0, columnTitle + ";", edt);
	}
}


void SbRowEdit::exec()
{
	mapper.setCurrentIndex(view->currentIndex().row());
	show();
}

void SbRowEdit::cancelTask()
{
#ifdef DEBUG
	qDebug("SLOT cancelTask");
#endif
	model->revertAll();
	close();
}

void SbRowEdit::performTask()
{
	qDebug("slot performTask");
	emit rowChange(opCode, this);
	close();
}

void SbRowEdit::displayRecord(QSqlRecord rec)
{
#ifdef DEBUG
	QSqlField fld;

	qDebug("Record Content");
	int i;
	for (i = 0; i < rec.count(); i++)
	{
		fld = rec.field(i);
		qDebug("\tfieldname [%s] \t type [%s] \tvalue [%s]",
					 rec.fieldName(i).toAscii().data(),
					 fld.value().typeName(),
					 rec.value(i).toString().toAscii().data());
	}
#endif
}

QString SbRowEdit::getFieldType(QSqlField fld)
{
#ifdef DEBUG
	/****************
	qDebug("Field Content");

	qDebug("\tfieldname [%s] \t type [%s] \tvalue [%s]",
				 fld.name().toAscii().data(),
				 fld.value().typeName(),
				 fld.value().toString().toAscii().data());
	********************/
#endif
	QString ret = fld.value().typeName();
	return ret.toUpper();
}



QWidget *SbRowEdit::createEditWidget(int columnNumber)
{
	model->record(view->currentIndex().row()).field(columnNumber);
	QString columnName = model->record(view->currentIndex().row()).fieldName(columnNumber);
	QString fldType = getFieldType(model->record(view->currentIndex().row()).field(columnNumber));

#ifdef DEBUG
	qDebug("fieldtype [%s]", fldType.toAscii().data());
#endif

	QWidget *edt = 0;
	QLineEdit *lEdt = 0;
	QDateEdit *lDateEdt = 0;
	QTimeEdit *lTimeEdt = 0;
	QDateTimeEdit *lDateTimeEdt = 0;

	if (fldType == "QSTRING")
	{
		lEdt = new QLineEdit(this);
		if (opCode == SbRowEdit::REC_INSERT)
		{
			lEdt->setText("");
		}
		else
		{
			lEdt->setText(model->record(view->currentIndex().row()).value(columnNumber).toString());
		}
		edt = lEdt;
	}
	else if (fldType == "INT")
	{
		lEdt = new QLineEdit(this);
		if (opCode == SbRowEdit::REC_INSERT)
		{
			lEdt->setText("");
		}
		else
		{
			lEdt->setText(model->record(view->currentIndex().row()).value(columnNumber).toString());
		}
		QValidator *validator = new QIntValidator(this);
		lEdt->setValidator(validator);
		edt = lEdt;
	}
	else if (fldType == "DOUBLE")
	{
		lEdt = new QLineEdit(this);
		if (opCode == SbRowEdit::REC_INSERT)
		{
			lEdt->setText("");
		}
		else
		{
			lEdt->setText(model->record(view->currentIndex().row()).value(columnNumber).toString());
		}
		QValidator *validator = new QDoubleValidator(this);
		lEdt->setValidator(validator);
		edt = lEdt;
	}
	else if (fldType == "QDATE")
	{
		lDateEdt = new QDateEdit(this);
		if (opCode == SbRowEdit::REC_INSERT)
		{
			lDateEdt->setDate(QDate::currentDate());
		}
		else
		{
			lDateEdt->setDate(model->record(view->currentIndex().row()).value(columnNumber).toDate());
		}
		edt = lDateEdt;
	}
	else if (fldType == "QTIME")
	{
		lTimeEdt = new QTimeEdit(this);
		if (opCode == SbRowEdit::REC_INSERT)
		{
			lTimeEdt->setTime(QTime::currentTime());
		}
		else
		{
			lTimeEdt->setTime(model->record(view->currentIndex().row()).value(columnNumber).toTime());
		}
		edt = lTimeEdt;
	}
	else if (fldType == "QDATETIME")
	{
		lDateTimeEdt = new QDateTimeEdit(this);
		if (opCode == SbRowEdit::REC_INSERT)
		{
			lDateTimeEdt->setDateTime(QDateTime::currentDateTime());
		}
		else
		{
			lDateTimeEdt->setDateTime(model->record(view->currentIndex().row()).value(columnNumber).toDateTime());
		}
		edt = lDateTimeEdt;
	}
	else
	{
#ifdef DEBUG
		qDebug("SbRowEdit::createEditWidget(int columnNumber) unsupported fieldtype [%s]",
					 fldType.toAscii().data());
#endif
		lEdt = new QLineEdit(this);
		if (opCode == SbRowEdit::REC_INSERT)
		{
			lEdt->setText("");
		}
		else
		{
			lEdt->setText(model->record(view->currentIndex().row()).value(columnNumber).toString());
		}
		edt = lEdt;
	}

	edt->setObjectName(columnName);

	return edt;
}
