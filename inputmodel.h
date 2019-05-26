#ifndef INPUTMODEL_H
#define INPUTMODEL_H

#include <QObject>
#include <QBrush>
#include <QInputDialog>
#include <QMessageBox>


#include "xlreader.h"
#include "model.h"
//#include "myproxymodel.h"

class InputModel : public Model
{
    Q_OBJECT

public:
    InputModel(Table *parent = 0);
    ~InputModel();

    virtual void appendRowData(QList<QString> dataList);
    virtual void insertRowData(int rowNumber, QList<QString> dataList);
    virtual QList<QString> rowData(int rowNumber);

    void appendRowData(Product product);
    void checkExpire(QString expire, int period=90);


public slots:
    void undoAddRow(QString name, QString farm);
    void rowClicked(const QModelIndex index);

signals:
    void addProduct(QList<QString> product);

private:
    QIcon iconP, iconN;
};

#endif // INPUTMODEL_H
