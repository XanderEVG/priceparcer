#ifndef DRUGDILERSDIALOG_H
#define DRUGDILERSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>


class DrugDilersDialog : public QDialog
{
    Q_OBJECT

public:
    DrugDilersDialog(QWidget *parent = 0);
    ~DrugDilersDialog();

    void read_drugdilers();

public slots:
    void save();
    void add_drugdiler();
    void del_drugdiler();

private:
    QLabel *drugdilers_label;
    QListWidget *drugdilers;
    QPushButton *add_btn;
    QPushButton *del_btn;
    QPushButton *save_btn;
};

#endif // DRUGDILERSDIALOG_H
