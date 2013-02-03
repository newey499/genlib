#include "sbrowse.h"

Sbrowse::Sbrowse(QWidget *parent) :
    QTableView(parent)
{
	commonConstructor();
}

Sbrowse::Sbrowse(QString queryStr, QWidget *parent) :
	QTableView(parent)
{
	commonConstructor();
	setQuery(queryStr);
}


void Sbrowse::commonConstructor()
{
	setModel(&model);
	configureView();
}

void Sbrowse::setQuery(QString queryStr)
{
	model.setQuery(queryStr);
	configureView();
}

QSqlQuery Sbrowse::getQuery()
{
	return model.query();
}

void Sbrowse::configureView()
{
	setSelectionBehavior(QAbstractItemView::SelectRows);
	resizeColumnsToContents();
	horizontalHeader()->setStretchLastSection(true);
	selectRow(0);
}

int Sbrowse::currentRow()
{
	QModelIndex index = selectedIndexes().at(0);
	return index.row();
}



void Sbrowse::keyPressEvent(QKeyEvent *event)
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
        qDebug("SIGNAL rowChanged emitted %s %d", __FILE__, __LINE__);
#endif
	}

	if ( event->key() == Qt::Key_Return ||
			 event->key() == Qt::Key_Enter )
	{
        emit rowEdit(this);
#ifdef DEBUG
        qDebug("SIGNAL rowEdit emitted %s %d", __FILE__, __LINE__);
#endif
	}

	if ( event->key() == Qt::Key_Insert)
	{
		emit rowInsert(this);
#ifdef DEBUG
        qDebug("SIGNAL rowInsert emitted %s %d", __FILE__, __LINE__);
#endif
	}


	if ( event->key() == Qt::Key_Delete)
	{
		emit rowDelete(this);
#ifdef DEBUG
        qDebug("SIGNAL rowDelete emitted %s %d", __FILE__, __LINE__);
#endif
	}

}


void Sbrowse::mousePressEvent(QMouseEvent * event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		event->accept();

		int row = this->rowAt(event->y());
		if (row != -1)
		{
			selectRow(row);
		}

		emit rowChanged(this);

		int id = this->getQuery().record().field("id").value().toInt();
#ifdef DEBUG
        qDebug("SIGNAL rowChanged emitted Click (QEvent::MouseButtonPress) row [%d] id [%d] %s %d",
               row, id, __FILE__, __LINE__);
#endif

	}
	else
	{
		event->ignore();
	}
}

void Sbrowse::mouseDoubleClickEvent(QMouseEvent * event)
{
	if (event->type() == QEvent::MouseButtonDblClick)
	{
		event->accept();
        emit rowEdit(this);
#ifdef DEBUG
        qDebug("SIGNAL rowEdit emitted DoubleClick %s %d", __FILE__, __LINE__);
#endif
	}
	else
	{
		event->ignore();
	}
}
