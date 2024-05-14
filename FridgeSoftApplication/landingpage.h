#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QMainWindow>

#include "loginwidget.h"

namespace Ui {
class landingPage;
}

class landingPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit landingPage(QWidget *parent = nullptr);
    ~landingPage();

private slots:

    void on_user_Login_clicked();

    void on_delivery_Login_clicked();

    void onLoginSuccessful();

private:
    Ui::landingPage *ui;

};

#endif // LANDINGPAGE_H
