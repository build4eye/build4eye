#include "windowform.h"
#include "ui_windowform.h"

windowform::windowform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::windowform)
{
    ui->setupUi(this);
}

windowform::~windowform()
{
    delete ui;
}

void windowform::setImage(QImage img)
{
 ui->scrollArea->setImage(img);
}
