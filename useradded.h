#ifndef USERADDED_H
#define USERADDED_H

#include <QWidget>

namespace Ui {
class UserAdded;
}

class UserAdded : public QWidget
{
    Q_OBJECT

public:
    explicit UserAdded(QWidget *parent = nullptr);
    ~UserAdded();

private:
    Ui::UserAdded *ui;

signals:
    void UA_move_Registration();
    void UA_move_UsersList_1();
    void UA_move_UsersList_2();
private slots:
    void on_Users_list_UP_clicked();
    void on_Add_One_More_clicked();
    void on_Users_list_down_clicked();
};

#endif // USERADDED_H
