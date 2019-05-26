#include "model.h"
#include <QBrush>
#include <QInputDialog>
#include <QMessageBox>

Model::Model(Table *parent) : QStandardItemModel(parent)
{
    defaultColumnNumber = 5;
    setColumnCount(defaultColumnNumber);

}

Model::~Model()
{

}

void Model::setDefaultHeaderName(QList<QString> headNameList)
{
    //headItems=headNameList;
    setHorizontalHeaderLabels(headNameList);
}


void Model::clearData()
{
    removeRows(0, rowCount());
}

void Model::appendRowData(QList<QString> dataList)
{
    QList<QStandardItem *> rowData;
    foreach (QString data, dataList)
    {
        rowData << new QStandardItem(QString(data));
        rowData.last()->setEditable(false);
    }
    appendRow(rowData);
}

void Model::insertRowData(int rowNumber, QList<QString> dataList)
{
    QList<QStandardItem *> rowData;
    foreach (QString data, dataList)
    {
        rowData << new QStandardItem(QString(data));
        rowData.last()->setEditable(false);
    }
    insertRow(rowNumber, rowData);
}

void Model::editRowData(int rowNumber, QList<QString> dataList)
{
    /*self.removeRows(rowNumber, 1)
    self.insertRowData(rowNumber, dataList)
    ##TODO: Check IT*/

    removeRows(rowNumber, 1);
    insertRowData(rowNumber, dataList);
}

QList<QString> Model::rowData(int rowNumber)
{
    QList<QString> data_list;
    //QList<QStandardItem *> row_list = takeRow(rowNumber);


    for(int col = 0; col < columnCount(); col++)
    {
        data_list<<item(rowNumber, col)->text().replace(",","."); //TODO удалить этот реплейс и др т к уже добавлен в цене в xlreader
    }
    return data_list;
}

void Model::setColor(int row, int col, QColor color)
{
    QBrush brush(color);
    item(row, col)->setBackground(brush);
}







