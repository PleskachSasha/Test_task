#include "useradded.h"
#include "ui_useradded.h"

UserAdded::UserAdded(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserAdded)
{
    ui->setupUi(this);
    QPixmap pix (":/resource/img/success-svgrepo-com.svg");
    int w = ui->succesfully_added_label->width();
    int h = ui->succesfully_added_label->height();

    ui->succesfully_added_label->setPixmap(pix.scaled(w,h, Qt::KeepAspectRatio));
}

UserAdded::~UserAdded()
{
    delete ui;
}

void UserAdded::on_Users_list_UP_clicked()
{
    emit UA_move_UsersList_1();
}

void UserAdded::on_Users_list_down_clicked()
{
    emit UA_move_UsersList_2();
}

void UserAdded::on_Add_One_More_clicked()
{
    emit UA_move_Registration();
}




