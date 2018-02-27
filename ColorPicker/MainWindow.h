#pragma once

// std
#include <vector>
#include <string>
// Qt
#include <QMainWindow>
#include <QDebug>
#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QApplication>
#include <QTimer>
#include <QLabel>
#include <QPixmap>
// AmazingCow Libs
#include "CoreColor/CoreColor.h"


namespace Ui {
    class MainWindow;
}

class MainWindow
    : public QMainWindow
{
    Q_OBJECT

    //------------------------------------------------------------------------//
    // Emums / Constants / Typedefs                                           //
    //------------------------------------------------------------------------//
private:
    //--------------------------------------------------------------------------
    // COWNOTE(n2omatt): Important!!!
    //   The order of the enum must match the order of the combo box on the
    //   UI, since is used to make the conversions correctly...
    enum class ColorMode { RGBA, HSL, HSV, CMYK };


    //------------------------------------------------------------------------//
    // CTOR / DTOR                                                            //
    //------------------------------------------------------------------------//
public:
    MainWindow(QWidget *pParent = nullptr);
    ~MainWindow();


    //------------------------------------------------------------------------//
    // Slots                                                                  //
    //------------------------------------------------------------------------//
private slots:
    void OnTimerTimeout();
    void OnComboColorModeChanged(int index);


    //------------------------------------------------------------------------//
    // Color Mode Methods                                                     //
    //------------------------------------------------------------------------//
private:
    void ChangeColorMode(ColorMode mode);

    void ChangeColorMode_RGBA();
    void ChangeColorMode_HSL ();
    void ChangeColorMode_HSV ();
    void ChangeColorMode_CMYK();


    //------------------------------------------------------------------------//
    // "Display" Methods                                                      //
    //------------------------------------------------------------------------//
private:
    void UpdateScreenDisplay();
    void UpdateColorDisplay();


    //------------------------------------------------------------------------//
    // Color Labels Methods                                                   //
    //------------------------------------------------------------------------//
private:
    void UpdateColorLabels();


    //------------------------------------------------------------------------//
    // Status Bar Methods                                                     //
    //------------------------------------------------------------------------//
private:
    void UpdateStatusBar();


    //------------------------------------------------------------------------//
    // iVars                                                                  //
    //------------------------------------------------------------------------//
private:
    QTimer *m_pPollMouseTimer;
    QPoint  m_mousePosition;
    QRgb    m_currentRGBColor;

    ColorMode        m_currentColorMode;
    CoreColor::Color m_color;

    std::vector<std::string> m_colorCompLabelsPrefixes;

    //--------------------------------------------------------------------------
    // UI
    Ui::MainWindow        *m_ui;
    std::vector<QLabel *>  m_colorCompLabelsRefs;
    QPixmap                m_screenPixamp;

    QLabel *m_pStatusBarMouse;
    QLabel *m_pStatusBarHex;

}; // class MainWindow
