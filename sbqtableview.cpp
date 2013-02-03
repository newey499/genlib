#include "sbqtableview.h"


SbQTableView::SbQTableView(QWidget *parent) :
    QTableView(parent)
{
	pView = 0;
	cView = 0;

	//setItemDelegate(new QSqlRelationalDelegate(this));
	connect(this, SIGNAL(rowSelect(SbQTableView *)),
					this, SLOT(slotRowUpdate(SbQTableView *)));
	connect(this, SIGNAL(rowInsert(SbQTableView *)),
					this, SLOT(slotRowInsert(SbQTableView *)));
	connect(this, SIGNAL(rowDelete(SbQTableView *)),
					this, SLOT(slotRowDelete(SbQTableView *)));

	connect(this, SIGNAL(clicked(QModelIndex)),
					this, SLOT(processClickedSignal(QModelIndex)));

	configureView();
}


void SbQTableView::setParentView(SbQTableView *parentView)
{
	pView = parentView;
	pView->cView = this;
}

SbQTableView * SbQTableView::parentView()
{
	return pView;
}

void SbQTableView::setChildView(SbQTableView * childView)
{
	cView = childView;
	cView->pView = this;
}

SbQTableView * SbQTableView::childView()
{
	return cView;
}


void SbQTableView::configureView()
{
	setSelectionBehavior(QAbstractItemView::SelectRows);
	resizeColumnsToContents();
	horizontalHeader()->setStretchLastSection(true);
	selectRow(0);
}



void SbQTableView::keyPressEvent(QKeyEvent *event)
{
	QTableView::keyPressEvent(event);

	if (
			event->key() == Qt::Key_Up ||
			event->key() == Qt::Key_Down ||
			event->key() == Qt::Key_PageUp ||
			event->key() == Qt::Key_PageDown
		 )
	{
		emit rowChanged(this);
#ifdef DEBUG
		qDebug("SIGNAL rowChanged emitted");
#endif
	}

	if ( event->key() == Qt::Key_Return ||
			 event->key() == Qt::Key_Enter )
	{
		emit rowSelect(this);
#ifdef DEBUG
		qDebug("SIGNAL rowSelect emitted");
#endif
	}

	if ( event->key() == Qt::Key_Insert)
	{
		emit rowInsert(this);
#ifdef DEBUG
		qDebug("SIGNAL rowInsert emitted");
#endif
	}


	if ( event->key() == Qt::Key_Delete)
	{
		emit rowDelete(this);
#ifdef DEBUG
		qDebug("SIGNAL rowDelete emitted");
#endif
	}

}


void SbQTableView::processClickedSignal(QModelIndex)
{
	emit rowChanged(this);
#ifdef DEBUG
	qDebug("SIGNAL rowChanged emitted Click");
#endif
}


void SbQTableView::mouseDoubleClickEvent(QMouseEvent * event)
{
	if (event->type() == QEvent::MouseButtonDblClick)
	{
		event->accept();
		emit rowSelect(this);
#ifdef DEBUG
		qDebug("SIGNAL rowSelect emitted DoubleClick");
#endif
	}
	else
	{
		event->ignore();
	}
}

int SbQTableView::currentRow()
{
	QModelIndex index = selectedIndexes().at(0);
	qDebug("selected row [%d]", index.row());
	return index.row();
}


void SbQTableView::slotRowUpdate(SbQTableView *view)
{
#ifdef DEBUG
	qDebug("SLOT rowUpdate");
#endif
	QPointer<SbRowEdit> rowEdit = new SbRowEdit(SbRowEdit::REC_EDIT, view, 0);
	rowEdit->setWindowTitle("Edit");
	rowEdit->btnDoAction.setText("&Save");
	connect(rowEdit, SIGNAL(rowChange(int, SbRowEdit *)),
					this, SLOT(slotRowEditChange(int, SbRowEdit *)));
	rowEdit->exec();
}

void SbQTableView::slotRowInsert(SbQTableView *view)
{
#ifdef DEBUG
	qDebug("SLOT rowInsert");
#endif
	QPointer<SbRowEdit> rowEdit = new SbRowEdit(SbRowEdit::REC_INSERT,
																							view, 0);
	rowEdit->setWindowTitle("Insert");
	rowEdit->btnDoAction.setText("&Save");
	connect(rowEdit, SIGNAL(rowChange(int, SbRowEdit *)),
					this, SLOT(slotRowEditChange(int, SbRowEdit *)));
	rowEdit->exec();
}

void SbQTableView::slotRowDelete(SbQTableView *view)
{
#ifdef DEBUG
	qDebug("SLOT rowDelete");
#endif
	QPointer<SbRowEdit> rowEdit = new SbRowEdit(SbRowEdit::REC_DELETE,
																							view, 0);
	rowEdit->setWindowTitle("Delete");
	rowEdit->btnDoAction.setText("&Delete");
	connect(rowEdit, SIGNAL(rowChange(int, SbRowEdit *)),
					this, SLOT(slotRowEditChange(int, SbRowEdit *)));

	rowEdit->exec();
}


void SbQTableView::slotRowEditChange(int opCode, SbRowEdit *rowEdit)
{

	bool acceptChange = false;	// default to no change

#ifdef DEBUG
	qDebug("QWERTY SbQTableView::rowEditChange(int opCode, SbRowEdit *rowEdit)");
#endif
	int nRow;
	SbQRelationalTableModel * model = (SbQRelationalTableModel *) this->model();

	switch (opCode)
	{
		case SbRowEdit::REC_EDIT:
			qDebug("slot performTask  REC_EDIT");

			nRow = currentRow();
			emit beforeRowChange(opCode, this, rowEdit, &acceptChange);
			saveRowChange(opCode, this, rowEdit, &acceptChange);
			if (acceptChange)
			{
				if (! model->submitAll())
				{
					qDebug("UPDATE Error [%s]", model->lastError().text().toAscii().data());
					QMessageBox::warning(this, "Update Row failed",
															 model->lastError().text(),
															 QMessageBox::Ok, QMessageBox::Ok);
					model->revertAll();
				}
			}
			selectRow(nRow);
			break;

		case SbRowEdit::REC_INSERT:
			qDebug("slot performTask  REC_INSERT");
			emit beforeRowChange(opCode, this, rowEdit, &acceptChange);
			saveRowChange(opCode, this, rowEdit, &acceptChange);
			if (acceptChange)
			{
				if (! model->submitAll())
				{
					qDebug("INSERT Error [%s]", model->lastError().text().toAscii().data());
					QMessageBox::warning(this, "Insert Row failed",
															 model->lastError().text(),
															 QMessageBox::Ok, QMessageBox::Ok);
					model->revertAll();
				}
				else
				{
					// select the last row - this assumes the new row is always
					// inserted as the bottom row
					selectRow(model->rowCount() - 1);
				}
			}
			else
			{
				model->revertAll();
			}
			// TODO Selecting the row after insert probably won't work
			//nRow = currentRow();
			//selectRow(nRow);
			break;

		case SbRowEdit::REC_DELETE:
			qDebug("slot performTask  REC_DELETE");
			// TODO Selecting the row after delete probably won't work
			nRow = currentRow();
			emit beforeRowChange(opCode, this, rowEdit, &acceptChange);
			saveRowChange(opCode, this, rowEdit, &acceptChange);
			if (acceptChange)
			{
				if (! model->submitAll())
				{
					qDebug("DELETE Error [%s]", model->lastError().text().toAscii().data());
					QMessageBox::warning(this, "Delete Row failed",
															 model->lastError().text(),
															 QMessageBox::Ok, QMessageBox::Ok);
					model->revertAll();
				}
				if (nRow == 0)
				{
					selectRow(0);
				}
				else
				{
					if (nRow >= model->rowCount())
					{
						selectRow(nRow - 1);
					}
					else
					{
						selectRow(nRow);
					}
				}
			}
			else
			{
				model->revertAll();
			}
			break;

		case SbRowEdit::REC_DISPLAY:
		default:
			qDebug("slot performTask  REC_DISPLAY");
			nRow = currentRow();
			model->revertAll();
			selectRow(nRow);
			break;

	}

}


void SbQTableView::saveRowChange(int, SbQTableView *, SbRowEdit *, bool *)
{

#ifdef DEBUG
	qDebug("SbQTableView::saveRowChange(int, SbQTableView *, SbRowEdit *, bool *)");
#endif

/*************************************
	// ======================================================================
	// If subclassing use this code as a template when overriding this method
	// ======================================================================
	SbQRelationalTableModel *mdl = (SbQRelationalTableModel*) model();

	QSqlRecord rec;
	QSqlRelation rel;

	int speciesId = mdlSpecies->record(ui->tblSpecies->currentRow()).value("id").toInt();

	// The SbRowEdit class sets the name of each edit object (normally a QLineEdit)
	// to the name of the field whose conetns is being displayed
	QLineEdit * x = rowEdit->findChild<QLineEdit *>("name");

	QString animalName = x->text();

#ifdef DEBUG
	qDebug("species id from primary key of species table [%d]", speciesId);
#endif

	switch (opCode)
	{
	case SbRowEdit::REC_EDIT:
		rec = mdl->record(ui->tblSpecies->currentRow());
		rec.setValue("name", animalName);
		rec.remove(rec.indexOf("id"));
		rec.remove(rec.indexOf("species"));

		displayRecord(rec);
		mdl->setRecord(ui->tblAnimals->currentRow(), rec);
		*acceptChange = true;
		break;

	case SbRowEdit::REC_INSERT:
		rec = mdl->record();
		rec.setValue("species", speciesId);
		rec.setValue("name", animalName);
		if (mdl->insertRecord(-1,rec))
		{
			*acceptChange = true;
		}
		else
		{
			*acceptChange = false;
		}
		break;

	case SbRowEdit::REC_DELETE:
		if (mdl->removeRow(animalView->currentRow()))
		{
			*acceptChange = true;
		}
		else
		{
			*acceptChange = false;
		}
		break;

	case SbRowEdit::REC_DISPLAY:
	default:
		break;

	}
****************************************/
}
