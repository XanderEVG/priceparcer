#include "xlwriter.h"
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include "xlsxcellformula.h"


XlWriter::XlWriter(QString file, QList<ExportProduct> products)
{
    QFile xlsx_file(file);
    if(xlsx_file.exists())
    {
        if(QMessageBox::question(NULL, QObject::tr("Файл существует"), QObject::tr("Перезаписать ") + file + " ?" ) == QMessageBox::Yes)
        {
            xlsx_file.remove();
        }
    }

    xlsx = new QXlsx::Document(file);

    QXlsx::Format format_body;
    format_body.setFontSize(12);
    format_body.setHorizontalAlignment(QXlsx::Format::AlignLeft);
    format_body.setBorderStyle(QXlsx::Format::BorderThin);
    format_body.setNumberFormatIndex(1);

    QXlsx::Format format_body_2_digits;
    format_body_2_digits.setFontSize(12);
    format_body_2_digits.setHorizontalAlignment(QXlsx::Format::AlignLeft);
    format_body_2_digits.setBorderStyle(QXlsx::Format::BorderThin);
    format_body_2_digits.setNumberFormatIndex(2);



    float total = 0;

    foreach (ExportProduct product, products)
    {

       if(!xlsx->selectSheet(product.supplier))
            create_sheet(product.supplier);
       int row = xlsx->currentWorksheet()->dimension().rowCount()+1;
       xlsx->write(row, 1, product.name, format_body);
       xlsx->write(row, 2, str_to_float(product.discount_price), format_body_2_digits);
       xlsx->write(row, 3, str_to_float(product.count), format_body);
       xlsx->write(row, 4, str_to_float(product.cost), format_body_2_digits);
       xlsx->write(row, 5, product.expire_data, format_body);
       xlsx->write(row, 6, product.manufacturer, format_body);

       total += str_to_float(product.cost);

       /*
        формулы:
        QXlsx::Worksheet *sheet = xlsx->currentWorksheet();
        sheet->writeFormula("D3", QXlsx::CellFormula("=R1C1+R2C2"));
       */
    }

    int row = xlsx->currentWorksheet()->dimension().rowCount()+2;
    xlsx->write(row, 3, "Итого", format_body);
    xlsx->write(row, 4, total, format_body_2_digits);

    xlsx->save();
}

XlWriter::~XlWriter()
{
    delete xlsx;
}

void XlWriter::create_sheet(QString name)
{
    QXlsx::Format format_head;
    //format_head.setFontColor(QColor(Qt::red));
    format_head.setFontSize(13);
    format_head.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format_head.setBorderStyle(QXlsx::Format::BorderMedium);
    format_head.setPatternBackgroundColor(QColor(Qt::gray));

    xlsx->addSheet(name);
    xlsx->selectSheet(name);
    xlsx->write(1,1, QObject::tr("Наименование"), format_head);
    xlsx->write(1,2, QObject::tr("Цена") ,format_head);
    xlsx->write(1,3, QObject::tr("Кол-во") ,format_head);
    xlsx->write(1,4, QObject::tr("Стоимость") ,format_head);
    xlsx->write(1,5, QObject::tr("Срок годности") ,format_head);
    xlsx->write(1,6, QObject::tr("Производитель") ,format_head);


    /*
    Некорректно работают формулы
    xlsx->write(1,8, QObject::tr("Цена с наценкой") ,format_head);
    xlsx->write(1,9, QObject::tr("Наценка, %") ,format_head);
*/

    xlsx->setColumnWidth(1,1, 100);
    xlsx->setColumnWidth(2,2, 15);
    xlsx->setColumnWidth(3,3, 15);
    xlsx->setColumnWidth(4,4, 20);
    xlsx->setColumnWidth(5,5, 25);
    xlsx->setColumnWidth(6,6, 40);
    //xlsx->setColumnWidth(8,8, 40);
    //xlsx->setColumnWidth(9,9, 40);

}

float XlWriter::str_to_float(QString value)
{
    QTextStream floatTextStream(&value);
    float float_value;
    floatTextStream >> float_value;
    return float_value;
}




