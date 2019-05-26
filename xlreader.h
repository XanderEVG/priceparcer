#ifndef XLSREADER_H
#define XLSREADER_H

//#include <freexl.h>
#include "freexl_lib/freexl.h"
#include "xlsxdocument.h"
#include "xlsxworksheet.h"
#include "xlsxcellrange.h"


#include <QSettings>
#include <QString>
#include <QPoint>
#include <QList>
#include <QDate>

#include <QDebug>
#include <QObject>

struct Firm
{
   QString name;
   QString runame;
   QString description;
   QList<QString> headers;
   int start_row_index;
   int head_id_name;
   int head_id_cost;
   int head_id_madeby;
   int head_id_expire;
   double discount;
};

struct Product
{
   QString name;
   float price;
   float discount_price;
   QDate expire_data;
   QString manufacturer;
   QString supplier;
};


class XlsReader
{

public:
    XlsReader();
    virtual ~XlsReader();

    virtual bool openXL(QString file);
    virtual qint16 currentSheet();
    virtual bool selectSheet(quint16 sheet);
    virtual QString sheetName(quint16 sheet);
    virtual qint32 rowCount();
    virtual qint32 colCount();
    virtual QVariant readCell(quint32 row, quint32 col); //QString?
    virtual void closeFile();

    QPoint  findCell( quint16 sheet, QString text);
    QPoint  findCell( QString text);

    QPoint  findText( QString text);
    QPoint  findText(int after_row, QString text);
    QPoint  findTextInRow( int row, QString text);

    void readSettings();
    void saveSettings();
    QList<Product> getPriceList();
    QString getFirmName();

protected:
    const void *handle;
    int ret;
    bool isOk;
    Firm firm;
    QList<QString> farmnames;
    QXlsx::Document* xlsx;
};


class XlsXReader: public XlsReader
{

public:
    XlsXReader();
    virtual ~XlsXReader();

    virtual bool openXL(QString file);
    virtual qint16 currentSheet();
    virtual bool selectSheet(quint16 sheet);
    virtual QString sheetName(quint16 sheet);
    virtual qint32 rowCount();
    virtual qint32 colCount();
    virtual QVariant readCell(quint32 row, quint32 col); //QString?
    virtual void closeFile();

};

class XlReader : public QObject
{
    Q_OBJECT

public:
    XlReader(QString file);
    virtual ~XlReader();
    QList<Product> getPriceList();
    QString getFirmName();

    QString current_file;
    QList<Product> price_list;
    XlsReader* xl_reader;

 public slots:
     void process();

 signals:
     void finished();
     void error(QString err);
};







#endif // XLSREADER_H
