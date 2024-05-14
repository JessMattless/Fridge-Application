#ifndef DRIVERLOGINWIDGET_H
#define DRIVERLOGINWIDGET_H

#include <QWidget>

namespace Ui {
class DriverLoginWidget;
}

class DriverLoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DriverLoginWidget(QWidget *parent = nullptr);
    ~DriverLoginWidget();

signals:
    void loginSuccessful();

private slots:
    void on_confirmButton_clicked();

    void on_closeButton_clicked();
    void loginError();
    void on_passcodeField_returnPressed();

private:
    Ui::DriverLoginWidget *ui;


};

#endif // DRIVERLOGINWIDGET_H
