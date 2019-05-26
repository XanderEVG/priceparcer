#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QString>
#include <QIcon>

#include "xlreader.h"
#include "table.h"
//#include "inputmodel.h"
//#include "myproxymodel.h"


class Model : public QStandardItemModel
{
public:
    Model(Table *parent = 0);
    ~Model();


    //QList<QString> headItems;
    int defaultColumnNumber;


    void setDefaultHeaderName(QList<QString> headNameList);
    void clearData();
    void setColor(int row, int col, QColor color = QColor(100,100,100,100));

    virtual void appendRowData(QList<QString> dataList);
    virtual void insertRowData(int rowNumber, QList<QString> dataList);
    virtual void editRowData(int rowNumber, QList<QString> dataList);
    virtual QList<QString> rowData(int rowNumber);

protected:
    Table *parent;

};

#endif // MODEL_H














