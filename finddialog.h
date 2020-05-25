#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QObject>
#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <qlayout.h>

class FindDialog: public QDialog
{
    Q_OBJECT
public:
    FindDialog(QWidget *parent = 0);
private slots:
     void findClicked();
     void enableFindButton(const QString &text);
private:
     QLabel *label;
     QLineEdit *lineEdit;
     QPushButton *findButton;
     QPushButton *closeButton;
signals:
    void find(const QString &str);
};

#endif // FINDDIALOG_H
