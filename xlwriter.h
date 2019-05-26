#ifndef XLWRITER_H
#define XLWRITER_H

#include "outputmodel.h"
#include "xlsxdocument.h"
#include "xlsxworksheet.h"
#include "xlsxcellrange.h"
#include "xlsxcellformula.h"

#include <QSettings>
#include <QString>
#include <QPoint>
#include <QList>
#include <QDate>

#include <QDebug>
#include <QObject>



class XlWriter
{
public:
    XlWriter(QString file, QList<ExportProduct> products);
    ~XlWriter();

    void create_sheet(QString name);
private:
    QXlsx::Document* xlsx;

    float str_to_float(QString value);
};

#endif // XLWRITER_H
