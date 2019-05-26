#include "table.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

Table::Table(QWidget *parent) : QTableView(parent)
{
    connect(horizontalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(sortByColumn(int)));

}

Table::~Table()
{

}


void Table::setColumnsWidth(QList<int> columns_width_list)
{
    int count = 0;


    if (model())
      for (int i = 0; i <  columns_width_list.size(); i++)
        if (count < model()->columnCount())
        {
            setColumnWidth(count, columns_width_list[i]);
            count++;
        }

}

QList<int> Table::ColumnsWidth()
{
    QList<int> listWidths;
    if (model())
        for(int i = 0; i< model()->columnCount(); i++)
        listWidths.append(columnWidth(i));

    return listWidths;
}

