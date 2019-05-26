#include "xlreader.h"

#include <QRegularExpression>
#include <QMessageBox>
#include <QObject>
#include <math.h>

#include <QCoreApplication>




#define IMAX_FIND 4000
#define JMAX_FIND 30


//// XlsReader ///////////////////////////////////////////////////////////////////////////////////
XlsReader::XlsReader()
{
    isOk=false;

   /*
        isOk = openXL(file);
        if(!isOk)
        {

            return;
        }
        readSettings();
    */
}

XlsReader::~XlsReader()
{
}

bool XlsReader::openXL(QString file)
{
    ret = freexl_open (file.toLocal8Bit().data(), &handle);
    if (ret != FREEXL_OK)
    {
          QMessageBox::information(NULL, QObject::tr("xlReader file error"), QObject::tr("Не удалось открыть ") + file);
          return false;
    }
    isOk = true;
    selectSheet(0);
    readSettings();
    return true;
}

qint16 XlsReader::currentSheet()
{
    unsigned short sheet_index;
    ret = freexl_get_active_worksheet(handle, &sheet_index);
    if (ret != FREEXL_OK)
        return -1;
    return sheet_index;
}

bool XlsReader::selectSheet(quint16 sheet)
{
    unsigned short sheet_index = (unsigned short)sheet;
    ret = freexl_select_active_worksheet(handle, sheet_index);
    if (ret != FREEXL_OK)
        return false;
    return true;
}

QString XlsReader::sheetName(quint16 sheet)
{
    const char* name;
    unsigned short sheet_index = (unsigned short)sheet;
    ret = freexl_get_worksheet_name(handle, sheet_index, &name);
    if (ret != FREEXL_OK)
        return "";
    return QString(name);
}

qint32 XlsReader::rowCount()
{
    unsigned int rows;
    unsigned short columns;
    ret = freexl_worksheet_dimensions (handle, &rows, &columns);
    if (ret != FREEXL_OK)
        return -1;
    return rows ;
}

qint32 XlsReader::colCount()
{
    unsigned int rows;
    unsigned short columns;
    ret = freexl_worksheet_dimensions (handle, &rows, &columns);
    if (ret != FREEXL_OK)
        return -1;
    return columns   ;
}

QVariant XlsReader::readCell(quint32 row, quint32 col) //QString?  //засечь время выполнения
{
    unsigned int row_index = (unsigned int)row;
    unsigned short column_index = (unsigned int)col;
    FreeXL_CellValue val;
    ret = freexl_get_cell_value(handle, row_index, column_index, &val);
        if (ret != FREEXL_OK)
           return QString("");

    switch (val.type)
    {
        case FREEXL_CELL_INT:
            return val.value.int_value;
            break;

        case FREEXL_CELL_DOUBLE:
            return val.value.double_value;
            break;

        case FREEXL_CELL_TEXT:
        case FREEXL_CELL_SST_TEXT:
            return QString(val.value.text_value);
            break;

        case FREEXL_CELL_DATE:
        case FREEXL_CELL_DATETIME:
        case FREEXL_CELL_TIME:
            return QString(val.value.text_value);
            break;

        case FREEXL_CELL_NULL:
            //qDebug()<<"FREEXL_CELL_NULL"<<row<<col;
            return QString("");
            break;

        default:
            qDebug()<<"Invalid data-type\n";
            return QString("");
            break;
    }

    return QString("return");
}



void XlsReader::closeFile()
{
    ret = freexl_close(handle);
}



//------------------------------------------------------------------
QPoint XlsReader::findCell(QString text)
{
    return findCell(0, text);
}

QPoint XlsReader::findCell(quint16 sheet, QString text)
{
    if(!selectSheet(sheet)) return QPoint(-1,-1);

    int imax, jmax;

    imax = IMAX_FIND; if(imax>rowCount())imax = rowCount();
    jmax = JMAX_FIND; if(jmax>colCount())jmax = colCount();

    for(int i = 0; i<imax; i++)
    for(int j = 0; j<jmax; j++)
    {
        if(readCell(i, j).toString()==text)
            return QPoint(i, j);

    }

    return QPoint(-1,-1);
}

QPoint XlsReader::findText(QString text)
{
    int sheet = 0;
    if(!selectSheet(sheet)) return QPoint(-1,-1);

    int imax, jmax;

    imax = IMAX_FIND; if(imax>rowCount())imax = rowCount();
    jmax = JMAX_FIND; if(jmax>colCount())jmax = colCount();

    for(int i = 0; i<imax; i++)
    for(int j = 0; j<jmax; j++)
    {
        if(readCell(i, j).toString().contains(text, Qt::CaseInsensitive))
        {
            return QPoint(i, j);
        }
    }

    return QPoint(-1,-1);
}

QPoint XlsReader::findText(int start_row, QString text)
{
    int sheet = 0;
    if(!selectSheet(sheet)) return QPoint(-1,-1);

    int imax, jmax;

    imax = IMAX_FIND; if(imax>rowCount())imax = rowCount();
    jmax = JMAX_FIND; if(jmax>colCount())jmax = colCount();


    if (start_row<imax)
    {
        for(int i = start_row; i<imax; i++)
        {
            for(int j = 0; j<jmax; j++)
            {
                if(readCell(i, j).toString().contains(text, Qt::CaseInsensitive))
                {
                    return QPoint(i, j);
                }
            }
        }
    }
    qDebug()<<"Не найдено: " << text;
    return QPoint(-1,-1);
}


QPoint XlsReader::findTextInRow( int row, QString text)
{
    if(!selectSheet(0)) return QPoint(-1,-1);

    int imax, jmax;

    imax = IMAX_FIND; if(imax>rowCount())imax = rowCount();
    jmax = JMAX_FIND; if(jmax>colCount())jmax = colCount();

    for(int j = 0; j<jmax; j++)
    {
        if(readCell(row, j).toString().contains(text, Qt::CaseInsensitive))
           return QPoint(row, j);
    }

    return QPoint(-1,-1);
}

void XlsReader::readSettings()
{
    if(!isOk) return;

    QSettings* settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");



    farmnames = settings->value("main/farmnames", "").toString().split(";");

    foreach(QString name, farmnames)
    {

        QString temp_firm_runame = settings->value(name+"/runame", "unknown").toString();

        if(name.isEmpty() || temp_firm_runame == "unknown")
        {
            farmnames.removeOne(name);
            continue;
        }

        firm.name       = name;
        firm.runame     = settings->value(name+"/runame", "unknown").toString();
        firm.description = settings->value(name+"/description", "unknown").toString();

        QPoint find_rez = findText(firm.description);

        if(find_rez.x()<0 || find_rez.y()< 0 )
        {
            firm.name = "";
            firm.runame = "";
            firm.description ="";
            continue;
        }

        firm.headers = settings->value(name+"/headers", "unknown").toString().split(";");
        foreach(QString head, firm.headers)
        {
            if(head.isEmpty())
            {
                firm.headers.removeOne(head);
                continue;
            }
        }
        if(firm.headers.size()<4)
        {
            isOk = false; // Значит в файле касяк, его читать не будем
            QMessageBox::information(NULL, QObject::tr("xlReader file error"), QObject::tr("Не удалось распарсить файл "));
            continue;
        }

        firm.start_row_index = settings->value(name+"/start_row_index","-1").toInt();
        firm.head_id_name    = settings->value(name+"/head_id_name",   "-1").toInt();
        firm.head_id_cost    = settings->value(name+"/head_id_cost",   "-1").toInt();
        firm.head_id_madeby  = settings->value(name+"/head_id_madeby", "-1").toInt();
        firm.head_id_expire  = settings->value(name+"/head_id_expire", "-1").toInt();
        firm.discount        = settings->value(name+"/discount",        "0").toDouble();      

        if( readCell(firm.start_row_index, firm.head_id_name ) == firm.headers[0] &&
            readCell(firm.start_row_index, firm.head_id_cost ) == firm.headers[1]    &&
              readCell(firm.start_row_index, firm.head_id_madeby ) == firm.headers[3]  &&
                readCell(firm.start_row_index, firm.head_id_expire ) == firm.headers[2]   )
        {
            //Найдено, сохранены значения из файла
            qDebug()<<"Найдено, сохранены значения из файла";

            saveSettings();
        }
        else
        {
            //Не надено, ищем вручную
            qDebug()<<"Не найдено, ищем вручную";

            int start_row = 0;
            int max_rows_index = 5000;
            bool founded = false;
            do {

                qDebug()<<"Ищем:"<<firm.headers[0] <<", c позиции: "<<start_row;
                firm.start_row_index = findText(start_row, firm.headers[0]).x();

                firm.head_id_name    = findTextInRow(firm.start_row_index, firm.headers[0]).y();
                firm.head_id_cost = findTextInRow( firm.start_row_index, firm.headers[1]).y();
                firm.head_id_expire = findTextInRow( firm.start_row_index, firm.headers[2]).y();
                firm.head_id_madeby = findTextInRow( firm.start_row_index, firm.headers[3]).y();

                if(firm.start_row_index<0 || firm.head_id_name<0   ||
                   firm.head_id_cost<0    || firm.head_id_madeby<0 || firm.head_id_expire<0)
                {
                    isOk=false;
                    founded = false;
                }
                else
                {
                    isOk=true;
                    founded = true;
                    saveSettings();
                    break;
                }

                if(firm.start_row_index > 0)
                {
                    start_row += firm.start_row_index + 1;
                }
                else
                {
                    start_row = max_rows_index;
                }


            } while (start_row < max_rows_index && founded == false );

            if(founded)
            {
                qDebug() << "Файл распознан";
            }
            else
            {
                qDebug() << "Файл не распознан";
            }
        }
    }

    delete settings;
}

void XlsReader::saveSettings()
{
    QSettings* settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    settings->setValue(firm.name+"/start_row_index", firm.start_row_index );
    settings->setValue(firm.name+"/head_id_name",    firm.head_id_name );
    settings->setValue(firm.name+"/head_id_cost",    firm.head_id_cost );
    settings->setValue(firm.name+"/head_id_madeby",  firm.head_id_madeby );
    settings->setValue(firm.name+"/head_id_expire",  firm.head_id_expire );
    settings->setValue(firm.name+"/discount",        QString::number(firm.discount) );
    settings->sync();
    delete settings;
}


QList<Product> XlsReader::getPriceList()
{

    QList<Product> products_list;
    if(!isOk) return products_list;

    //TODO обработка ошибок чтения, data

    for(int i=firm.start_row_index+1; i<rowCount(); i++)
    {
        Product product;
        QString title = readCell(i, firm.head_id_name).toString();//.replace(",",".");
        if(title.isEmpty()) continue;
        product.name = title;
        bool convertOk=false;
        QString price_str = readCell(i, firm.head_id_cost).toString();//.replace(",", ".");

        if(price_str.isEmpty())
            continue;

        float price = price_str.toFloat(&convertOk);
        if(convertOk)
        {
            product.price = price; //floor(cost*(100.0-firm.discount)/100 * pow(10., decimal_places) + .5) / pow(10., decimal_places);
        }
        else
        {
            product.price=0;
        }

        quint8 decimal_places = 2;
        product.discount_price = floor(product.price*(100.0-firm.discount)/100 * pow(10., decimal_places) + .5) / pow(10., decimal_places);

        QList<QString> date_str;
        date_str = readCell(i, firm.head_id_expire).toString().split("/");
        //if(date_str.size()>1) //TODO обработка второй даты
        //{
        //}

        if(date_str[0].size()>8)
        {
            product.expire_data = QDate::fromString(date_str[0], "dd.MM.yyyy");
        }
        else
        {
            product.expire_data = QDate::fromString(date_str[0], "dd.MM.yy");
            product.expire_data = product.expire_data.addYears(100);
        }



        product.manufacturer = readCell(i, firm.head_id_madeby).toString();
        //qDebug()<<"firm.runame:"<<firm.runame;
        product.supplier = firm.runame;

        products_list.append(product);

    }
    return products_list;
}

QString XlsReader::getFirmName()
{
    return firm.runame;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//// XlsXReader ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

XlsXReader::XlsXReader(): XlsReader()
{
   /*
    isOk = openXL(file);
    if(!isOk)
    {
        QMessageBox::information(NULL, QObject::tr("xlReader file error"), QObject::tr("Не удалось открыть ") + file);
        return;
    }

    readSettings();
*/
}

XlsXReader::~XlsXReader()
{
}

bool XlsXReader::openXL(QString file)
{
    xlsx = 0;
    xlsx = new QXlsx::Document(file);

    xlsx->selectSheet(xlsx->sheetNames()[0]);
    if (!xlsx->currentWorksheet()->dimension().rowCount())
    {
        QMessageBox::information(NULL, QObject::tr("xlReader file error"), QObject::tr("Не удалось открыть ") + file);
        isOk = false;
        return false;
    }


    isOk = true;
    readSettings();
    return true;
}

qint16 XlsXReader::currentSheet()
{
    QString current_name = xlsx->currentWorksheet()->sheetName();
    QList<QString> sheetNames_list = xlsx->sheetNames();
    for(unsigned short i = 0; i < sheetNames_list.size(); i++)
    {
       if(sheetNames_list.at(i) == current_name)
       return i;
    }
    return -1;

}

bool XlsXReader::selectSheet(quint16 sheet)
{
    return xlsx->selectSheet(xlsx->sheetNames()[sheet]);
}

QString XlsXReader::sheetName(quint16 sheet)
{ 
    return xlsx->currentWorksheet()->sheetName();
}

qint32 XlsXReader::rowCount()
{
    int row_count;
    row_count = xlsx->currentWorksheet()->dimension().rowCount();
    return row_count;
}

qint32 XlsXReader::colCount()
{
    int col_count;
    col_count = xlsx->currentWorksheet()->dimension().columnCount();
    return col_count;
}

QVariant XlsXReader::readCell(quint32 row, quint32 col) //QString?  //засечь время выполнения
{

    QVariant value;
    if(QXlsx::Cell *cell=xlsx->cellAt(row+1, col+1))
    {
        value = cell->value();
        return value;
    }
    else
    {
        return QString("");
    }

    if(value.isValid()==false || value.isNull()==true)
        return QString("");

    return value;
}



void XlsXReader::closeFile()
{
}









XlReader::XlReader(QString file): QObject()
{
    current_file = file;
    if(current_file.contains(QRegularExpression("\\.xls$",QRegularExpression::CaseInsensitiveOption)))
    {
        xl_reader = new XlsReader;
    }
    else if(current_file.contains(QRegularExpression("\\.xlsx$",QRegularExpression::CaseInsensitiveOption)))
    {
        xl_reader = new XlsXReader;
    }
    else
    {
       xl_reader=0;
       //emit finished();
    }



}

XlReader::~XlReader()
{
    delete xl_reader;
}

void XlReader::process()
{
    if(xl_reader==0) return;

    xl_reader->openXL(current_file);
    price_list = xl_reader->getPriceList();
   // qDebug()<<xl_reader->readCell(36,5);

  /*  qDebug()<<0<<xl_reader->readCell(350,0);
    qDebug()<<1<<xl_reader->readCell(350,1);
    qDebug()<<2<<xl_reader->readCell(350,2);
    qDebug()<<3<<xl_reader->readCell(350,3);
    qDebug()<<4<<xl_reader->readCell(350,4);
    qDebug()<<5<<xl_reader->readCell(350,5);*/

    //emit finished();
}

QList<Product> XlReader::getPriceList()
{
    process();
    QList<Product> rez = price_list;
    xl_reader->closeFile();
    return rez;
}

QString XlReader::getFirmName()
{
    return xl_reader->getFirmName();
}


