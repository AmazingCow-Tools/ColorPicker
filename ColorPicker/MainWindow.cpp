// Header
#include "MainWindow.h"
// UI
#include "ui_MainWindow.h"
// Qt
#include <QtCore>
#include <QtGui>
#include <QPixmap>
#include <QDesktopWidget>
// AmazingCow Libs
#include "CoreString/CoreString.h"


//----------------------------------------------------------------------------//
// CTOR / DTOR                                                                //
//----------------------------------------------------------------------------//
MainWindow::MainWindow(QWidget *pParent /* = nullptr */)
     : QMainWindow(pParent)
     , m_ui(new Ui::MainWindow)
{
    //--------------------------------------------------------------------------
    // Setup the UI.
    m_ui->setupUi(this);

    m_pStatusBarMouse = new QLabel(this);
    m_pStatusBarHex   = new QLabel(this);

    m_ui->statusBar->addWidget(m_pStatusBarMouse, 0);
    m_ui->statusBar->addPermanentWidget(m_pStatusBarHex, 0);


    //--------------------------------------------------------------------------
    // Setup the Pool Timer.
    m_pPollMouseTimer = new QTimer(this);
    m_pPollMouseTimer->setInterval(50);

    connect(m_pPollMouseTimer, SIGNAL(timeout()), this, SLOT(OnTimerTimeout()));
    m_pPollMouseTimer->start();

    //--------------------------------------------------------------------------
    // Setup connections.
    connect(
        m_ui->combo_ColorMode,
        SIGNAL(currentIndexChanged(int)),
        this,
        SLOT(OnComboColorModeChanged(int))
    );

    //--------------------------------------------------------------------------
    //
    m_colorCompLabelsRefs.push_back(m_ui->label_ColorComp1);
    m_colorCompLabelsRefs.push_back(m_ui->label_ColorComp2);
    m_colorCompLabelsRefs.push_back(m_ui->label_ColorComp3);
    m_colorCompLabelsRefs.push_back(m_ui->label_ColorComp4);

    ChangeColorMode(ColorMode::RGBA);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}


//----------------------------------------------------------------------------//
// Slots                                                                      //
//----------------------------------------------------------------------------//
void MainWindow::OnTimerTimeout()
{
    //--------------------------------------------------------------------------
    // Update Position.
    m_mousePosition = QCursor::pos();

    //--------------------------------------------------------------------------
    // Update Color.
    m_screenPixamp    = QPixmap::grabWindow(QApplication::desktop()->winId());
    auto screen_image = m_screenPixamp.toImage();

    m_currentRGBColor = screen_image.pixel(m_mousePosition);

    m_color.setRGBA(
        qRed  (m_currentRGBColor) / 255.0f,
        qGreen(m_currentRGBColor) / 255.0f,
        qBlue (m_currentRGBColor) / 255.0f
    );

    switch(m_currentColorMode)
    {
        case ColorMode::RGBA : m_color.toRGBA_InPlace(); break;
        case ColorMode::HSL  : m_color.toHSL_InPlace (); break;
        case ColorMode::HSV  : m_color.toHSV_InPlace (); break;
        case ColorMode::CMYK : m_color.toCMYK_InPlace(); break;
    }

    //--------------------------------------------------------------------------
    // Update the displays.
    UpdateScreenDisplay();
    UpdateColorDisplay ();
    UpdateColorLabels  ();
    UpdateStatusBar    ();

}

void MainWindow::OnComboColorModeChanged(int index)
{
    ChangeColorMode(static_cast<ColorMode>(index));
}

//----------------------------------------------------------------------------//
// Color Mode Methods                                                         //
//----------------------------------------------------------------------------//
void MainWindow::ChangeColorMode(ColorMode mode)
{
    m_currentColorMode = mode;

    switch(mode)
    {
        case ColorMode::RGBA : ChangeColorMode_RGBA(); break;
        case ColorMode::HSL  : ChangeColorMode_HSL (); break;
        case ColorMode::HSV  : ChangeColorMode_HSV (); break;
        case ColorMode::CMYK : ChangeColorMode_CMYK(); break;
    }
}

void MainWindow::ChangeColorMode_RGBA()
{
    m_colorCompLabelsPrefixes.clear();
    m_colorCompLabelsPrefixes.push_back("R: %.2f");
    m_colorCompLabelsPrefixes.push_back("G: %.2f");
    m_colorCompLabelsPrefixes.push_back("B: %.2f");
}

void MainWindow::ChangeColorMode_HSL()
{
    m_colorCompLabelsPrefixes.clear();
    m_colorCompLabelsPrefixes.push_back("H: %.2f");
    m_colorCompLabelsPrefixes.push_back("S: %.2f");
    m_colorCompLabelsPrefixes.push_back("L: %.2f");
}

void MainWindow::ChangeColorMode_HSV()
{
    m_colorCompLabelsPrefixes.clear();
    m_colorCompLabelsPrefixes.push_back("H: %.2f");
    m_colorCompLabelsPrefixes.push_back("S: %.2f");
    m_colorCompLabelsPrefixes.push_back("V: %.2f");
}

void MainWindow::ChangeColorMode_CMYK()
{
    m_colorCompLabelsPrefixes.clear();
    m_colorCompLabelsPrefixes.push_back("C: %.2f");
    m_colorCompLabelsPrefixes.push_back("M: %.2f");
    m_colorCompLabelsPrefixes.push_back("Y: %.2f");
    m_colorCompLabelsPrefixes.push_back("K: %.2f");
}



//----------------------------------------------------------------------------//
// "Display" Methods                                                          //
//----------------------------------------------------------------------------//
void MainWindow::UpdateScreenDisplay()
{
    auto &p_screen_label = m_ui->label_ScreenDisplay;
    auto width  = p_screen_label->width();
    auto height = p_screen_label->height();


    auto pixmap = m_screenPixamp.copy(
        m_mousePosition.x() - (width / 2),
        m_mousePosition.y() - (height / 2),
        width,
        height
    );

    p_screen_label->setPixmap(pixmap);
}

void MainWindow::UpdateColorDisplay()
{
    auto style_sheet = QString::fromStdString(
        CoreString::Format(
            "QLabel { background-color: rgba(%d, %d, %d, 255); }",
            qRed  (m_currentRGBColor),
            qGreen(m_currentRGBColor),
            qBlue (m_currentRGBColor)
        )
    );

    m_ui->label_ColorDisplay->setStyleSheet(style_sheet);
}


//----------------------------------------------------------------------------//
// Color Labels Methods                                                       //
//----------------------------------------------------------------------------//
void MainWindow::UpdateColorLabels()
{
    for(int i = 0; i < m_colorCompLabelsRefs.size(); ++i)
    {
        auto &p_label = m_colorCompLabelsRefs[i];
        if(i >= m_colorCompLabelsPrefixes.size())
        {
            p_label->setVisible(false);
            continue;
        }

        p_label->setVisible(true);
        auto &prefix = m_colorCompLabelsPrefixes[i];

        auto str = CoreString::Format(prefix, m_color.m_data[i]);
        p_label->setText(QString::fromStdString(str));
    }
}


//----------------------------------------------------------------------------//
// Status Bar Methods                                                         //
//----------------------------------------------------------------------------//
void MainWindow::UpdateStatusBar()
{
    //--------------------------------------------------------------------------
    // Mouse.
    auto mouse_msg = CoreString::Format(
        "Mouse: (%d, %d)",
        m_mousePosition.x(),
        m_mousePosition.y()
    );

    m_pStatusBarMouse->setText(QString::fromStdString(mouse_msg));

    //--------------------------------------------------------------------------
    // Hex.
    auto hex_str = CoreString::ToUpper(m_color.toHexRGB("#"));
    auto hex_msg = CoreString::Format("Hex: (%s)", hex_str);
    m_pStatusBarHex->setText(QString::fromStdString(hex_msg));

}
