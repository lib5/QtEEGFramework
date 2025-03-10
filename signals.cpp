#include "signals.h"
#include "ui_signals.h"
Signals::Signals(QWidget *parent)
    : QWidget{parent},ui(new Ui::Signals)
{
    ui->setupUi(this);
}
