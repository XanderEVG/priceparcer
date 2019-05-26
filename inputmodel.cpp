#include "inputmodel.h"
#include <QBrush>
#include <QInputDialog>
#include <QMessageBox>



InputModel::InputModel(Table *parent) : Model(parent)
{
    this->parent=parent;

    iconP.addPixmap(QPixmap(":/images/images/ok.png"),QIcon::Normal, QIcon::Off);
    iconN.addPixmap(QPixmap(":/images/images/bad.png"),QIcon::Normal, QIcon::Off);
    //#connect(this, SIGNAL(itemChanged(QStandardItem * item)),this, SLOT(undoAddRow()));
}

InputModel::~InputModel()
{

}

void InputModel::appendRowData(QList<QString> dataList)
{
    QStandardItem* item = new QStandardItem(iconN, "");
    item->setEditable(false);

    QList<QStandardItem *> rowData;
    rowData<<item;
    foreach (QString data, dataList)
    {
        rowData << new QStandardItem(QString(data));
        rowData.last()->setEditable(false);
    }
    appendRow(rowData);
    checkExpire(rowData.at(4)->text());
}

void InputModel::appendRowData(Product product)
{
    QStandardItem* item = new QStandardItem(iconN, "");
    item->setEditable(false);

    QList<QStandardItem *> rowData;
    rowData<<item;

    rowData << new QStandardItem(product.name);
    rowData.last()->setEditable(false);

    rowData << new QStandardItem(QString::number(product.price));
    rowData.last()->setEditable(false);

    rowData << new QStandardItem(QString::number(product.discount_price));
    rowData.last()->setEditable(false);

    rowData << new QStandardItem(product.expire_data.toString("dd.MM.yyyy"));
    rowData.last()->setEditable(false);

    rowData << new QStandardItem(product.manufacturer);
    rowData.last()->setEditable(false);

    rowData << new QStandardItem(product.supplier);
    rowData.last()->setEditable(false);

    appendRow(rowData);
    checkExpire(rowData.at(4)->text());
}


void InputModel::insertRowData(int rowNumber, QList<QString> dataList)
{
    QStandardItem* item = new QStandardItem(iconN, "");
    item->setEditable(false);

    QList<QStandardItem *> rowData;
    rowData<<item;

    foreach (QString data, dataList)
    {
        rowData << new QStandardItem(QString(data));
        rowData.last()->setEditable(false);
    }
    insertRow(rowNumber, rowData);
    checkExpire(rowData.at(4)->text());
}

QList<QString> InputModel::rowData(int rowNumber)
{
    QList<QString> data_list;
    data_list = Model::rowData(rowNumber);
    data_list.removeFirst();
    return data_list;
}

void InputModel::undoAddRow(QString name, QString farm)
{
    for(int i = 0; i < rowCount(); i++)
    {
        if(item(i,1)->text().replace(",",".")==name)
            if(item(i,6)->text()==farm)
            {
                setColor(i, 1, QColor(255,255,255,0));
                setColor(i, 2, QColor(255,255,255,0));
                item(i,0)->setIcon(iconN);
            }
    }


}

void InputModel::checkExpire(QString expire, int period)
{


      QDate date = QDate::currentDate();
      QDate expire_date = QDate::fromString(expire, "dd.MM.yyyy"); //expire;
      qint64 days = date.daysTo(expire_date);


      if(expire_date.isNull())
      {
          setColor(rowCount()-1, 4, QColor(130,0,0,100));
          return;
      }

      if(days < 3)
            setColor(rowCount()-1, 4, QColor(130,0,0,255));
      else if(days < period/2)
            setColor(rowCount()-1, 4, QColor(255,120,0,255));
      else if(days < period)
            setColor(rowCount()-1, 4, QColor(255,198,0,255));
      else
            setColor(rowCount()-1, 4, QColor(255,255,255,0));

}



void InputModel::rowClicked(const QModelIndex index)
{
    if(item(index.row(), 1)->background().color().value() == 100)
    {
        //QtGui.QMessageBox.warning(self.parent,"Warning!","Строка уже добавлена!", QtGui.QMessageBox.Ok);
        parent->clearSelection();
        return;
    }

    QList<QString> rez_list;
    rez_list = rowData(index.row());
    rez_list.removeAt(1);
    bool ok;
    int count = QInputDialog::getInt(NULL, tr("Введите количество"), tr("Количество:"), 1, 0, 10000, 1, &ok);
    if (ok)
    {
        rez_list.insert(2,QString::number(count));
    }
    else
    {
        //QMessageBox::warning(NULL,tr("Warning!"),tr("Значение поля \"Количество\" не корректно!"), QMessageBox::Ignore);
        //rez_list.insert(2,"0");
        return;
    }


    rez_list.insert(3,QString::number(rez_list.at(1).toFloat()*rez_list.at(2).toInt()));

    setColor(index.row(), 1);
    setColor(index.row(), 2);


    item(index.row(), 0)->setIcon(iconP);
    parent->clearSelection();
    emit addProduct(rez_list);

}



