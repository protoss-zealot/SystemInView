#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();
    QString pictureName;
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;

private:
    Ui::AboutDialog *ui;

};

#endif // ABOUTDIALOG_H
