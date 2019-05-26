#include "myproxymodel.h"

#include <QBrush>
#include <QInputDialog>
#include <QMessageBox>


MyProxyModel::MyProxyModel(Model* source, QObject *parent) : QSortFilterProxyModel( parent)
{
    setSourceModel(source);
    setSortCaseSensitivity(Qt::CaseInsensitive);
}

MyProxyModel::~MyProxyModel()
{

}

bool MyProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QString leftData = sourceModel()->data(left).toString().replace(",",".");
    QString rightData = sourceModel()->data(right).toString().replace(",",".");

    bool l_ok;
    bool r_ok;
    float left_f = leftData.toFloat(&l_ok);
    float right_f = rightData.toFloat(&r_ok);
    if(l_ok==true && r_ok==true)
        return left_f<right_f;

    QDate left_date  = QDate::fromString(leftData, "dd.MM.yyyy");
    QDate right_date = QDate::fromString(rightData, "dd.MM.yyyy");
    if(left_date.isNull() == false && right_date.isNull() == false)
        return left_date<right_date;

    return QSortFilterProxyModel::lessThan(left, right);
}

void MyProxyModel::rowClicked(const QModelIndex &index)
{
    emit doubleClicked(mapToSource(index));
}
