#ifndef WIDGET_H
#define WIDGET_H

#include <QClipboard>
#include <QMimeData>
#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    ~Widget();

private:
    void format_string(QString &, QString &);

private
    slots:
            void text_changed();

    void CreateSystemTrayIcon();

private:
    Ui::Widget *ui;
    QClipboard *clipboard;

    QString origin_str;
    QString formatted_str;
};

#endif // WIDGET_H
