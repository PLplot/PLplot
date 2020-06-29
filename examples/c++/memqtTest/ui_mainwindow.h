/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_testmemqt;
    QAction *actionPlot_using_pngqt;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QMenu *menuactions;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        action_testmemqt = new QAction(MainWindow);
        action_testmemqt->setObjectName(QStringLiteral("action_testmemqt"));
        actionPlot_using_pngqt = new QAction(MainWindow);
        actionPlot_using_pngqt->setObjectName(QStringLiteral("actionPlot_using_pngqt"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        menuactions = new QMenu(menubar);
        menuactions->setObjectName(QStringLiteral("menuactions"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuactions->menuAction());
        menuactions->addAction(action_testmemqt);
        menuactions->addAction(actionPlot_using_pngqt);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action_testmemqt->setText(QApplication::translate("MainWindow", "Testmemqt", nullptr));
#ifndef QT_NO_TOOLTIP
        action_testmemqt->setToolTip(QApplication::translate("MainWindow", "plot using memqt driver", nullptr));
#endif // QT_NO_TOOLTIP
        actionPlot_using_pngqt->setText(QApplication::translate("MainWindow", "Plot using pngqt", nullptr));
#ifndef QT_NO_TOOLTIP
        actionPlot_using_pngqt->setToolTip(QApplication::translate("MainWindow", "Plot using pngqt", nullptr));
#endif // QT_NO_TOOLTIP
        menuactions->setTitle(QApplication::translate("MainWindow", "actions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
