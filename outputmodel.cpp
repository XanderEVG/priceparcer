#include "outputmodel.h"
#define EDITABLE_COLUMN 2
OutputModel::OutputModel(Table *parent) : Model(parent)
{
    editable_col=EDITABLE_COLUMN;
    changed_row = -1;
    changed_col = -1;
    connect(this, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChangedSlot(QStandardItem*)));
}

OutputModel::~OutputModel()
{

}

void OutputModel::appendRowData(QList<QString> dataList)
{
    Model::appendRowData(dataList);
    item(rowCount()-1, editable_col)->setEditable(true);
}


void OutputModel::setEditable(int column)
{
    if(editable_col >= columnCount()) return;

    editable_col=column;
    for(int i = 0; i< rowCount(); i++)
        item(i, editable_col)->setEditable(true);
}

void OutputModel::rowClicked(const QModelIndex &index)
{
    if(index.column()==EDITABLE_COLUMN)
    {
        itemChangedCount(index.row(),index.column());
        return;
    }
    QString name = item(index.row(), 0)->text();
    QString farm = item(index.row(), 6)->text();

    emit unclickRow(name, farm);
    removeRows(index.row(), 1);

}

QList<ExportProduct> OutputModel::export_data()
{
    QList<ExportProduct> rez_list;

    for(int i = 0; i < rowCount(); i++) //"Name","dis price","Count","Cost","Expire","Made by","Supplier"
    {
        ExportProduct extended_product;;
        extended_product.name = item(i, 0)->text();
        extended_product.discount_price = item(i, 1)->text();
        extended_product.count = item(i, 2)->text();
        extended_product.cost = item(i, 3)->text();
        extended_product.expire_data = item(i, 4)->text();
        extended_product.manufacturer = item(i, 5)->text();
        extended_product.supplier = item(i, 6)->text();

        rez_list<<extended_product;
    }
    return rez_list;
}

void OutputModel::addProduct(QList<QString> dataList)
{
    appendRowData(dataList);
}

void OutputModel::itemChangedCount(int row, int col)
{

    changed_row = row;
    changed_col = col;

}

void OutputModel::itemChangedSlot(QStandardItem* current_item)
{
    if(changed_row<0 || changed_col<0) return;
    if(current_item->row()!=changed_row && current_item->column()!=changed_col) return;

    int row = changed_row;
    int col_price = changed_col-1;
    int col_count = changed_col;
    int col_cost = changed_col+1;

    if(item(row, col_count)->text().isEmpty())
        item(row, col_count)->setText("0");

    bool ok;
    int count = item(row, col_count)->text().toInt(&ok);
    if(ok == false || count < 0)
    {
        item(row, col_count)->setText("0");
        count = 0;
    }

    float price = item(row, col_price)->text().toFloat(&ok);
    if(ok == false || price < 0)
    {
        price = 0;
    }

    float cost = 1.0*price*count;
    item(row, col_cost)->setText(QString::number(cost));

}








