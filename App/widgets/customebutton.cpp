#include "customebutton.h"

CustomeButton::CustomeButton(QWidget *parent) : QPushButton(parent)
{
    initUI();
}

void CustomeButton::initUI()
{
    this->setStyleSheet(QStringLiteral(
                            "QPushButton{\n"
                            "   color: #3d2600;\n"
                            "   background: #ffdca3;\n"
                            "   border: 2px solid #3d2600;\n"
                            "   border-radius: 5px;\n"
                            "}\n"
                            "QPushButton:hover{\n"
                            "   background: #DFC18F;\n"
                            "}\n"
    ));

    QFont font;
    font.setPixelSize(24);
    font.setFamily("Bahnschrift SemiBold");

    this->setFont(font);

    this->setCursor(QCursor(Qt::PointingHandCursor));
}
