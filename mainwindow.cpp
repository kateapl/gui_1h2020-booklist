#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include "finddialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
 ui->setupUi(this);
 myLayout = new QGridLayout;
 ListCount = new QLabel ("Books = 0");
 ListLabel = new QLabel ("Main list");

 ListView = new QListView;
 findDialog = 0;
 listComboBox = new QComboBox(this);

 QAction *newList = new QAction("&New list", this);
 QAction *chooseList = new QAction("&Choose list", this);
 QAction *deleteList = new QAction("&Delete list", this);
 QAction *historyList = new QAction("&History", this);

 listMenu = menuBar()->addMenu("&Lists");// создаем меню

 connect(newList, SIGNAL(triggered()), this,SLOT(addList_clicked()));
 connect(chooseList, SIGNAL(triggered()), this,SLOT(chooseList_clicked()));
 connect(deleteList, SIGNAL(triggered()), this,SLOT(deleteList_clicked()));
 connect(historyList, SIGNAL(triggered()), this,SLOT(historyList_clicked()));

 listMenu->addAction(newList); // добавляем действие "New"
 listMenu->addAction(chooseList); // добавляем действие "Choose"
 listMenu->addAction(deleteList); // добавляем действие "Delete"
 listMenu->addAction(historyList); // добавляем историю

 Model = new QStringListModel(this);
 Model->setStringList(List);

 nameModel = new QStringListModel(this);
 nameModel->setStringList(nameMyLists);

 ListView->setModel(Model);
 ListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
 ListView->setSelectionMode(QAbstractItemView::ExtendedSelection);

 connect(ListView, SIGNAL(doubleClicked(QModelIndex)),SLOT(editItem()));//определяем обработчик щелчка по списку
 connect(ListView, SIGNAL(doubleClicked(QModelIndex)),SLOT(openList()));//открытие списка по двойному щелчку

 AddButton = new QPushButton ("Add");
 SearchButton = new QPushButton ("Search");
 DeleteButton = new QPushButton ("Delete");
 SortButton = new QPushButton ("Sort");
 BackButton = new QPushButton ("Main list");
 ElementButton= new QPushButton ("Add in list:");
 ReadedButton = new QPushButton ("Readed");
 BackButton->setEnabled(false);

 myLayout->addWidget(ListCount, 0, 0, 1, 4);
 myLayout->addWidget(ListLabel, 1, 0, 1, 4);
 myLayout->addWidget(ListView, 2, 0, 2, 4);
 myLayout->addWidget(AddButton, 4, 0);
 myLayout->addWidget(SearchButton, 4, 1);
 myLayout->addWidget(DeleteButton, 4, 2);
 myLayout->addWidget(ReadedButton, 4, 3);

 myLayout->addWidget(BackButton, 5, 0);
 myLayout->addWidget(ElementButton, 5, 1);
 myLayout->addWidget(listComboBox, 5, 2);
 myLayout->addWidget(SortButton, 5, 3);

 this->centralWidget()->setLayout(myLayout);

 connect(AddButton, SIGNAL(clicked()), this, SLOT(add_clicked()));
 connect(SearchButton, SIGNAL(clicked()), this, SLOT(search_clicked()));
 connect(DeleteButton, SIGNAL(clicked()), this, SLOT(delete_clicked()));
 connect(SortButton, SIGNAL(clicked()), this, SLOT(sort_clicked()));
 connect(BackButton, SIGNAL(clicked()), this, SLOT(back_clicked()));
 connect(ElementButton, SIGNAL(clicked()), this, SLOT(addElement_clicked()));
 connect(ReadedButton, SIGNAL(clicked()), this, SLOT(readed_clicked()));

 connect(listComboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(choosedListIndex(int)));

 state = 0;
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::editItem() {
    if (state == 0){
    int row = ListView->currentIndex().row();
    QModelIndex index = Model->index(row);
    ListView->setCurrentIndex(index);
    ListView->edit(index);
    }
}

void MainWindow::add_clicked() {
    if (state == 0){
        int row = Model->rowCount(); // в числовую переменную заносим общее количество строк
        Model->insertRows(row, 1); // добавляем строки, количеством 1 шт.
        QModelIndex index = Model->index(row); // создаем ссылку на элемент модели
        ListView->setCurrentIndex(index); // передаем этот индекс ListView
        ListView->edit(index); // переводим курсор на указанную позицию для ожидания ввода данных
        ListCount->setText("Books = "+QString::number(Model->rowCount()));
        // пересчитываем количество строк и выводим результат
    }
    if (state == 3){

        int row = nameModel->rowCount(); // в числовую переменную заносим общее количество строк
        nameModel->insertRows(row, 1); // добавляем строки, количеством 1 шт.
        QModelIndex index = nameModel->index(row); // создаем ссылку на элемент модели
        ListView->setCurrentIndex(index); // передаем этот индекс ListView
        ListView->edit(index); // переводим курсор на указанную позицию для ожидания ввода данных

        BackButton->setEnabled(true);
    }
}

void MainWindow::search_clicked() {//вызов окна поиска
    if (!findDialog) {
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(find(const QString &)), this, SLOT(find(const QString &)));
      }

      findDialog->show();
      findDialog->raise();
      //findDialog->setActiveWindow();
}

void MainWindow::delete_clicked() {//удаление элементов

    QModelIndexList selectedIndexes = ListView->selectionModel()->selectedIndexes();
    qSort(selectedIndexes.begin(), selectedIndexes.end(), qGreater<QModelIndex>());

if (state ==0){
    foreach(const QModelIndex &idx, selectedIndexes)
       {
           Model->removeRow(idx.row());
       }
 ListCount->setText("Books = "+QString::number(Model->rowCount()));
}

if (state == 2){//удаление списков
    foreach(const QModelIndex &idx, selectedIndexes)
       {
           int row = idx.row();

           nameModel->removeRow(idx.row());
           myLists.erase(myLists.begin()+row);

           nameMyLists.erase(nameMyLists.begin()+row);

       }

    BackButton->setEnabled(true);
}
if (state == 4){//удаление списков
    foreach(const QModelIndex &idx, selectedIndexes)
       {
           int row = idx.row();

           history.erase(history.begin()+row);

       }

    QStringListModel *historyModel = new QStringListModel(this);
    historyModel->setStringList(history);

    ListView->setModel(historyModel);

    BackButton->setEnabled(true);
}
}

void MainWindow::sort_clicked() {
    Model->sort(0);
}

void MainWindow::find(const QString &str)
{
    int  size = Model->rowCount();
    std::string word = str.toStdString();

    QStringList findList;
    for(int i = 0;i<size;i++){
       QModelIndex index = Model->index(i);//получаем индекс элемента
       QString text = index.data(Qt::DisplayRole).toString();//получаем текст из элемента
       std::string text_str = text.toStdString();

       if(text_str.find(word) != std::string::npos){
           findList.append(text);
       }
    }
    int n = findList.size();
    for(int i = 0;i < n;i++){
        std::cout<<findList.at(i).toStdString()<<std::endl;
    }
    QStringListModel *newModel = new QStringListModel(this);
    newModel->setStringList(findList);

    ListView->setModel(newModel);

    SearchButton->setEnabled(false);
    DeleteButton->setEnabled(false);
    SortButton->setEnabled(false);
    ElementButton->setEnabled(false);
    AddButton->setEnabled(false);
    ReadedButton->setEnabled(false);

    BackButton->setEnabled(true);
}

void MainWindow::back_clicked()
{
    ListView->setModel(Model);
    state = 0;

    addList();

    listComboBox->clear();
    QStringList Items = nameMyLists;
    listComboBox->addItems(Items);//добавление имен в поле выбора

    ListLabel->setText("Main list");

    SearchButton->setEnabled(true);
    DeleteButton->setEnabled(true);
    SortButton->setEnabled(true);
    ElementButton->setEnabled(true);
    AddButton->setEnabled(true);
    ReadedButton->setEnabled(true);

    BackButton->setEnabled(false);
}

void MainWindow::readed_clicked()
{
    QModelIndexList selectedIndexes = ListView->selectionModel()->selectedIndexes();
    qSort(selectedIndexes.begin(), selectedIndexes.end(), qGreater<QModelIndex>());

    foreach(const QModelIndex &idx, selectedIndexes)
       {

           QString text = idx.data(Qt::DisplayRole).toString();//получаем текст из элемента
           history.append(text);
           Model->removeRow(idx.row());
       }
 ListCount->setText("Books = "+QString::number(Model->rowCount()));

}

void MainWindow::addList_clicked()//создание нового списка
{

    ListLabel->setText("Add new list");

    ListView->setModel(nameModel);
    BackButton->setEnabled(true);

    SearchButton->setEnabled(false);
    DeleteButton->setEnabled(false);
    SortButton->setEnabled(false);
    ElementButton->setEnabled(false);
    ReadedButton->setEnabled(false);
    state = 3;
}

void MainWindow::addList()//добавление нового списка в вектор
{    
    int size = nameModel->rowCount();

    for(int i = nameMyLists.size();i<size;i++){
       QModelIndex index = nameModel->index(i);//получаем индекс элемента
       QString text = index.data(Qt::DisplayRole).toString();//получаем текст из элемента
       QStringList newList;
       myLists.push_back(newList);//добавление нового списка
       nameMyLists.append(text);//добавление имени нового списка
    }
}

int choosedList;
void MainWindow::choosedListIndex(int index)
{
    std::cout<<index<<" ";
    choosedList = index;
}

void MainWindow::chooseList_clicked()//выбор списка
{
    addList();
    if(!nameMyLists.isEmpty()){

        ListView->setModel(nameModel);
        BackButton->setEnabled(true);

        ListLabel->setText("Choose list");

        SearchButton->setEnabled(false);
        DeleteButton->setEnabled(false);
        SortButton->setEnabled(false);
        ElementButton->setEnabled(false);
        AddButton->setEnabled(false);
        ReadedButton->setEnabled(false);

        state = 1;
    }
    else{
        QMessageBox msgBox;
         msgBox.setText("No Lists");
         msgBox.exec();
         return;
    }
}

void MainWindow::deleteList_clicked()//удаление списков
{
    addList();
    if(!nameMyLists.isEmpty()){
      //  QStringListModel *deleteModel = new QStringListModel(this);
      //  deleteModel->setStringList(nameMyLists);

        ListView->setModel(nameModel);
        BackButton->setEnabled(true);

        SearchButton->setEnabled(false);
        DeleteButton->setEnabled(true);
        SortButton->setEnabled(false);
        ElementButton->setEnabled(false);
        AddButton->setEnabled(false);
        ReadedButton->setEnabled(false);

        ListLabel->setText("Delete list");

        state = 2;
    }
    else{
        QMessageBox msgBox;
         msgBox.setText("No Lists");
         msgBox.exec();
         return;
    }
}

void MainWindow::historyList_clicked()
{

    if(!history.isEmpty()){
        QStringListModel *historyModel = new QStringListModel(this);
        historyModel->setStringList(history);

        ListView->setModel(historyModel);
        BackButton->setEnabled(true);

        SearchButton->setEnabled(false);
        DeleteButton->setEnabled(true);
        SortButton->setEnabled(false);
        ElementButton->setEnabled(false);
        AddButton->setEnabled(false);
        ReadedButton->setEnabled(false);

        ListLabel->setText("Delete list");

        state = 4;
    }
    else{
        QMessageBox msgBox;
         msgBox.setText("History is empty");
         msgBox.exec();
         return;
    }
}

void MainWindow::openList()
{
    if (state ==1){
    int row = ListView->currentIndex().row();

    ListLabel->setText(nameMyLists.at(row));

    QStringListModel *newModel = new QStringListModel(this);
    newModel->setStringList(myLists.at(row));

    ListView->setModel(newModel);
    BackButton->setEnabled(true);
    }

}

void MainWindow::addElement_clicked()
{
    QModelIndexList selectedIndexes = ListView->selectionModel()->selectedIndexes();
    foreach(const QModelIndex &idx, selectedIndexes)
       {

           QString text = idx.data(Qt::DisplayRole).toString();//получаем текст из элемента
           myLists.at(choosedList).append(text);

       }
}
