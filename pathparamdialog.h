#ifndef PATHPARAMDIALOG_H
#define PATHPARAMDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

class PathParamDialog : public QDialog
{
    Q_OBJECT

public:
    PathParamDialog(QWidget *parent = 0);
    ~PathParamDialog();

    void read_paths();

public slots:
    void save();

private:
    QLabel *open_folder_label;
    QLabel *save_folder_label;
    QLineEdit *open_folder_edit;
    QLineEdit *save_folder_edit;
    QPushButton *save_btn;
};

#endif // PATHPARAMDIALOG_H



