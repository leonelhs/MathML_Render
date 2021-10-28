#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>

#include "MmlDocument.h"

int main(int argc, char *argv[])
{
    QScreen* screen = QGuiApplication::primaryScreen();

    QApplication a(argc, argv);

    QWidget MainWindow;

    QLabel labelformula(&MainWindow);
    QVBoxLayout layout(&MainWindow);
    layout.addWidget(&labelformula);

    QPainter painter;
    QPixmap pixmap(300, 150);
    painter.begin(&pixmap);
    painter.fillRect( QRect(0,0, 300, 150), Qt::white );

    QString formula = "<math><mfrac><mrow><mn>1</mn></mrow><mrow><mn>2</mn></mrow></mfrac><mo>&times;</mo><mi>ln</mi><mrow><mo>(</mo><mo>&VerticalBar;</mo><mi>tan</mi><mrow><mo>(</mo><mfrac><mrow><mi>x</mi></mrow><mrow><mn>2</mn></mrow></mfrac><mo>)</mo></mrow><mo>&VerticalBar;</mo><mo>)</mo></mrow></math>";

    MmlDocument doc;
    doc.clear();
    doc.setContent( formula );
    doc.setBaseFontPointSize( 32 );
    doc.layout();

    doc.paint( &painter, QPointF(50,50) );

    painter.end();

    //optionally pixmap can be saved to a PNG file
    pixmap.save("./mathml_formula.png");

    labelformula.setPixmap(pixmap);

    MainWindow.show();

    return a.exec();
}


