/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
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
    QAction *actionwithanimagebackground;
    QAction *actionmycase;
    QAction *actionmycase1;
    QAction *actionNormalBackgroud_all_others_are_alhpa;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QMenu *menuactions;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        action_testmemqt = new QAction(MainWindow);
        action_testmemqt->setObjectName(QString::fromUtf8("action_testmemqt"));
        actionPlot_using_pngqt = new QAction(MainWindow);
        actionPlot_using_pngqt->setObjectName(QString::fromUtf8("actionPlot_using_pngqt"));
        actionwithanimagebackground = new QAction(MainWindow);
        actionwithanimagebackground->setObjectName(QString::fromUtf8("actionwithanimagebackground"));
        actionmycase = new QAction(MainWindow);
        actionmycase->setObjectName(QString::fromUtf8("actionmycase"));
        actionmycase1 = new QAction(MainWindow);
        actionmycase1->setObjectName(QString::fromUtf8("actionmycase1"));
        actionNormalBackgroud_all_others_are_alhpa = new QAction(MainWindow);
        actionNormalBackgroud_all_others_are_alhpa->setObjectName(QString::fromUtf8("actionNormalBackgroud_all_others_are_alhpa"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menuactions = new QMenu(menubar);
        menuactions->setObjectName(QString::fromUtf8("menuactions"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuactions->menuAction());
        menuactions->addAction(actionNormalBackgroud_all_others_are_alhpa);
        menuactions->addAction(action_testmemqt);
        menuactions->addAction(actionPlot_using_pngqt);
        menuactions->addAction(actionwithanimagebackground);
        menuactions->addAction(actionmycase);
        menuactions->addAction(actionmycase1);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action_testmemqt->setText(QCoreApplication::translate("MainWindow", "Testmemqt", nullptr));
#if QT_CONFIG(tooltip)
        action_testmemqt->setToolTip(QCoreApplication::translate("MainWindow", "plot using memqt driver", nullptr));
#endif // QT_CONFIG(tooltip)
        actionPlot_using_pngqt->setText(QCoreApplication::translate("MainWindow", "Plot using pngqt", nullptr));
#if QT_CONFIG(tooltip)
        actionPlot_using_pngqt->setToolTip(QCoreApplication::translate("MainWindow", "Plot using pngqt", nullptr));
#endif // QT_CONFIG(tooltip)
        actionwithanimagebackground->setText(QCoreApplication::translate("MainWindow", "withanimagebackground", nullptr));
        actionmycase->setText(QCoreApplication::translate("MainWindow", "mycase", nullptr));
        actionmycase1->setText(QCoreApplication::translate("MainWindow", "mycase1", nullptr));
        actionNormalBackgroud_all_others_are_alhpa->setText(QCoreApplication::translate("MainWindow", "NormalBackgroud (all others are alhpa)", nullptr));
        menuactions->setTitle(QCoreApplication::translate("MainWindow", "actions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
