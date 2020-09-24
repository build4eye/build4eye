#ifndef WINDOWFORM_H
#define WINDOWFORM_H

#include <QWidget>

namespace Ui {
class windowform;
}

class windowform : public QWidget
{
    Q_OBJECT

public:
    explicit windowform(QWidget *parent = nullptr);
    ~windowform();

    void setImage(QImage img);
private:
    Ui::windowform *ui;
};

#endif // WINDOWFORM_H
