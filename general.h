#ifndef GENERAL_H
#define GENERAL_H
#include"edflib.h"
#include <QWidget>
namespace Ui {
class General;
}

class General : public QWidget
{
    Q_OBJECT
public:
    explicit General(QWidget *parent = nullptr);
    void initHdr(const edf_hdr_struct *hdr);

signals:
private:
    Ui::General *ui;
};

#endif // GENERAL_H
