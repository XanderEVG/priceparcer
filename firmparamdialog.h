#ifndef FIRMPARAMDIALOG_H
#define FIRMPARAMDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>

class FirmParamDialog : public QDialog
{
    Q_OBJECT

public:
    FirmParamDialog(QWidget *parent = 0);
    ~FirmParamDialog();

    void save_farm(QString farm_name);
    void read_farm_param(QString farm_name="");
    void read_farms();
public slots:
    void add_firm();
    void save();
    void firm_select(QString name);
private:

    QComboBox *firm;
    QPushButton *add_firm_btn;

    QLabel *runame_label;
    QLabel *description_label;
    QLabel *headers_label;
    QLabel *head_name_label;
    QLabel *head_price_label;
    QLabel *head_expire_data_label;
    QLabel *head_manufacturer_label;
    QLabel *discount_label;
    QLabel *caution_label;

    QLineEdit *runame_edit;
    QLineEdit *description_edit;

    QLineEdit *head_name_edit;
    QLineEdit *head_price_edit;
    QLineEdit *head_expire_data_edit;
    QLineEdit *head_manufacturer_edit;
    QLineEdit *discount_edit;

    QPushButton *save_btn;

signals:
    void firm_param_changed();


};

#endif // FIRMPARAMDIALOG_H
