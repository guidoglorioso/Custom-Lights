#ifndef AYUDARED_H
#define AYUDARED_H

#include <QDialog>

namespace Ui {
class AyudaRed;
}

class AyudaRed : public QDialog
{
    Q_OBJECT

public:
    explicit AyudaRed(QWidget *parent = nullptr);
    ~AyudaRed();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AyudaRed *ui;
};

#endif // AYUDARED_H
