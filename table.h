#ifndef TABLE_H
#define TABLE_H

#include <QObject>
#include <QTableView>

class Table : public QTableView
{
    Q_OBJECT
public:
    Table(QWidget *parent = 0);
    ~Table();

    void setColumnsWidth(QList<int> columns_width_list);
    QList<int> ColumnsWidth();
};

#endif // TABLE_H


