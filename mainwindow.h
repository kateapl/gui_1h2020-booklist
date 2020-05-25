#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMenuBar>
#include<QStatusBar>

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QListView>
#include <QStringListModel>
#include <QPushButton>
#include <QMessageBox>
#include <QAction>
#include <QObject>
#include <QAbstractItemModel>
#include <QStringList>
#include <vector>
#include <QMenuBar>
#include <QComboBox>
#include "finddialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    FindDialog *findDialog;

    QGridLayout *myLayout;
    QComboBox *listComboBox;
    QLabel *ListCount, *ListLabel;
    QStringListModel *Model;
    QStringListModel *nameModel;
    QListView *ListView;
    QPushButton *AddButton, *SearchButton, *DeleteButton, *SortButton, *BackButton, *ElementButton, *ReadedButton;
    QStringList List;
    QStringList nameMyLists;//список названий списков
    QStringList history;//список прочитанных книг

    std::vector <QStringList> myLists;//вектор списков, которые потом добавляются в модель для редактирования
    int state;

    QMenu *listMenu;
public slots:
    void add_clicked();
    void search_clicked();
    void delete_clicked();
    void sort_clicked();
    void back_clicked();
    void readed_clicked();

    void addList_clicked();
    void chooseList_clicked();
    void deleteList_clicked();
    void historyList_clicked();
    void addElement_clicked();

    void openList();

    void editItem();
    void find(const QString &str);
    void addList();
    void choosedListIndex(int index);
};


#endif // MAINWINDOW_H
