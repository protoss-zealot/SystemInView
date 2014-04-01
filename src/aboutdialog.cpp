#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->pictureName = "about.jpg";
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,80,80);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    QPixmap pixmap(this->pictureName);
    pixmap = pixmap.scaledToWidth(80);
    item = scene->addPixmap(pixmap);

    /*view*/
    ui->view->setScene(scene);
    ui->view->setRenderHint(QPainter::Antialiasing);
    ui->view->setCacheMode(QGraphicsView::CacheBackground);
    ui->view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->view->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->view->resize(100,100);

}

AboutDialog::~AboutDialog()
{
    delete ui;
}
