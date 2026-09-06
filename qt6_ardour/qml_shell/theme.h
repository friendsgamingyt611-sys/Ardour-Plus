#ifndef ARDOUR_THEME_H
#define ARDOUR_THEME_H

#include <QObject>
#include <QColor>
#include <QFont>

class ArdourTheme : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor bgWindow READ bgWindow CONSTANT)
    Q_PROPERTY(QColor bgPanel READ bgPanel CONSTANT)
    Q_PROPERTY(QColor bgCard READ bgCard CONSTANT)
    Q_PROPERTY(QColor bgHeader READ bgHeader CONSTANT)
    Q_PROPERTY(QColor bgCanvas READ bgCanvas CONSTANT)
    Q_PROPERTY(QColor bgInput READ bgInput CONSTANT)
    Q_PROPERTY(QColor bgTrackHeader READ bgTrackHeader CONSTANT)
    Q_PROPERTY(QColor bgTrackSelected READ bgTrackSelected CONSTANT)
    Q_PROPERTY(QColor borderDark READ borderDark CONSTANT)
    Q_PROPERTY(QColor borderLight READ borderLight CONSTANT)
    Q_PROPERTY(QColor borderFocus READ borderFocus CONSTANT)
    Q_PROPERTY(QColor textMain READ textMain CONSTANT)
    Q_PROPERTY(QColor textMuted READ textMuted CONSTANT)
    Q_PROPERTY(QColor textSubtle READ textSubtle CONSTANT)
    Q_PROPERTY(QColor clockBlue READ clockBlue CONSTANT)
    Q_PROPERTY(QColor clockGreen READ clockGreen CONSTANT)
    Q_PROPERTY(QColor clockYellow READ clockYellow CONSTANT)
    Q_PROPERTY(QColor btnNormal READ btnNormal CONSTANT)
    Q_PROPERTY(QColor btnHover READ btnHover CONSTANT)
    Q_PROPERTY(QColor btnActive READ btnActive CONSTANT)
    Q_PROPERTY(QColor soloOrange READ soloOrange CONSTANT)
    Q_PROPERTY(QColor muteRed READ muteRed CONSTANT)
    Q_PROPERTY(QColor recRed READ recRed CONSTANT)
    Q_PROPERTY(QColor loopBlue READ loopBlue CONSTANT)
    Q_PROPERTY(QColor punchRose READ punchRose CONSTANT)
    Q_PROPERTY(QFont defaultFont READ defaultFont CONSTANT)
    Q_PROPERTY(QFont clockFont READ clockFont CONSTANT)
    Q_PROPERTY(QFont miniFont READ miniFont CONSTANT)

public:
    explicit ArdourTheme(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~ArdourTheme() = default;

    QColor bgWindow() const { return QColor("#1e1e1e"); }
    QColor bgPanel() const { return QColor("#242424"); }
    QColor bgCard() const { return QColor("#313131"); }
    QColor bgHeader() const { return QColor("#161616"); }
    QColor bgCanvas() const { return QColor("#242424"); }
    QColor bgInput() const { return QColor("#1e1e1e"); }
    QColor bgTrackHeader() const { return QColor("#2a2a2a"); }
    QColor bgTrackSelected() const { return QColor("#3584e4"); }
    QColor borderDark() const { return QColor("#161616"); }
    QColor borderLight() const { return QColor("#393939"); }
    QColor borderFocus() const { return QColor("#3584e4"); }
    QColor textMain() const { return QColor("#f1f3f5"); }
    QColor textMuted() const { return QColor("#868e96"); }
    QColor textSubtle() const { return QColor("#6c757d"); }
    QColor clockBlue() const { return QColor("#66a3d8"); }
    QColor clockGreen() const { return QColor("#2ec27e"); }
    QColor clockYellow() const { return QColor("#f5c211"); }
    QColor btnNormal() const { return QColor("#393939"); }
    QColor btnHover() const { return QColor("#454545"); }
    QColor btnActive() const { return QColor("#3584e4"); }
    QColor soloOrange() const { return QColor("#fd7e14"); }
    QColor muteRed() const { return QColor("#c01c28"); }
    QColor recRed() const { return QColor("#c01c28"); }
    QColor loopBlue() const { return QColor("#3584e4"); }
    QColor punchRose() const { return QColor("#fa5252"); }

    QFont defaultFont() const { QFont f("Sans-Serif"); f.setPixelSize(11); return f; }
    QFont clockFont() const { QFont f("Monospace"); f.setPixelSize(13); f.setBold(true); return f; }
    QFont miniFont() const { QFont f("Sans-Serif"); f.setPixelSize(9); return f; }
};

#endif // ARDOUR_THEME_H
