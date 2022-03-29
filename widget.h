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
  void format_string(QString &, QString &);

private slots:
  void on_checkbox_enable_formatting_stateChanged(int arg1);

  void on_checkBox_auto_copy_stateChanged(int arg1);

  void timer_update();

private:
  Ui::Widget *ui;
  QTimer *timer;
  QClipboard *clipboard;

  QString origin_str;
  QString formatted_str;
};

#endif // WIDGET_H
