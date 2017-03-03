#ifndef OKFAILMESSAGEWINDOW_H
#define OKFAILMESSAGEWINDOW_H

#include <QWidget>

namespace Ui {
class OkFailMessageWindow;
}

class OkFailMessageWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OkFailMessageWindow(QWidget *parent = 0);
    ~OkFailMessageWindow();

private slots:
    void on_okButton_2_clicked();

private:
    Ui::OkFailMessageWindow *ui;
};

#endif // OKFAILMESSAGEWINDOW_H
