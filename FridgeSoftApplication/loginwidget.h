#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QDialog>

// How to style qt: https://www.pythontutorial.net/pyqt/qt-style-sheets/.

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

signals:
    void loginSuccessful();


private slots:
    void onConfirmClicked();
    void onCloseClicked();

    void loginError();


    void on_closeButton_2_clicked();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
