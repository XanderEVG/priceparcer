#ifndef MYPROXYMODEL_H
#define MYPROXYMODEL_H

#include <QObject>
#include <QBrush>
#include <QInputDialog>
#include <QMessageBox>

#include "xlreader.h"
#include "model.h"
#include "inputmodel.h"
#include "table.h"


class MyProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MyProxyModel(Model* source, QObject* parent = 0);
    ~MyProxyModel();

protected:
    virtual bool lessThan(const QModelIndex & left, const QModelIndex & right) const;

public slots:
   void rowClicked(const QModelIndex &index); //const QModelIndex &index);

signals:
   void doubleClicked(const QModelIndex proxyIndex);

};


#endif // MYPROXYMODEL_H
