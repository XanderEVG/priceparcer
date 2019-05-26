#ifndef OUTPUTMODEL_H
#define OUTPUTMODEL_H

#include <QObject>
#include "xlreader.h"
#include "model.h"

struct ExportProduct : public Product
{
   QString count;
   QString cost;
   QString expire_data;
   QString discount_price;

};




class OutputModel : public Model
{
    Q_OBJECT

public:
    OutputModel(Table *parent);
    ~OutputModel();

    virtual void appendRowData(QList<QString> dataList);
    void deleteRow(const QModelIndex &index);
    void setEditable(int column);
    QList<ExportProduct> export_data();

public slots:
    void rowClicked(const QModelIndex &index);
    void addProduct(QList<QString> dataList);

signals:
    void unclickRow(QString name, QString farm);

private slots:
    void itemChangedSlot(QStandardItem *current_item);
private:
    int editable_col;
    int changed_row;
    int changed_col;

    void itemChangedCount(int row, int col);
};

#endif // OUTPUTMODEL_H


/*



class OutputModel(Model):
  def __init__(self, parent=0):
    super(OutputModel, self).__init__(parent)

  def export(self):
    tableList=[]
    #TODO: do it in pythonway? self.items()???
    for row in range(self.rowCount()):
      rowList=[]
      for column in range(self.columnCount()):
        rowList.append(self.item(row, column).data(QtCore.Qt.DisplayRole).toPyObject())
      tableList.append(rowList)
    return tableList


  def deleteRow(self, index):
    self.removeRows(index.row(), 1)

  def setEditable(self, nameColumn):
    pass
    column = -1
    try:
      column=self.headItems.index(nameColumn)
    except ValueError:
      print("Warning!", er)
      QtGui.QMessageBox.warning(self,"Warning!","Column \"" + nameColumn + "\" not found!", QtGui.QMessageBox.Ignore)
    if column>=0:
      for row in range(self.rowCount()):
        self.item(row, column).setEditable(True)

  def rowClicked(self, index):
    self.removeRows(index.row(), 1)


*/
