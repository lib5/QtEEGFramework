#ifndef SIGNALS_H
#define SIGNALS_H

#include <QWidget>
namespace Ui {
class Signals;
}
class Signals : public QWidget
{
    Q_OBJECT
public:
    explicit Signals(QWidget *parent = nullptr);

signals:
private:
    Ui::Signals * ui;
};

#endif // SIGNALS_H
