#include "finddialog.h"
#include "iostream"
FindDialog::FindDialog(QWidget *parent)
   : QDialog(parent)
{
    label = new QLabel(tr("Find &what:"), this);
    lineEdit = new QLineEdit(this);
    label->setBuddy(lineEdit);

    findButton = new QPushButton(tr("&Find"), this);
    findButton->setDefault(true);
    findButton->setEnabled(false);
    closeButton = new QPushButton(tr("Close"), this);

    connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableFindButton(const QString &)));
    connect(findButton, SIGNAL(clicked()), this, SLOT(findClicked()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(label);
    topLayout->addWidget(lineEdit);

    QHBoxLayout *buttomLayout = new QHBoxLayout;
    buttomLayout->addWidget(findButton);
    buttomLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttomLayout);
}
void FindDialog::findClicked()
{
     QString text = lineEdit->text();

     emit find(text);

}

void FindDialog::enableFindButton(const QString &text)
{
    findButton->setEnabled(!text.isEmpty());
}
