#include "widget.h"
#include <QShortcut>
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Widget w;
  w.setWindowTitle("ClipboardFormator");
  w.show();

  // 设置退出的快捷键
  QShortcut *shortcut = new QShortcut(QKeySequence(QLatin1String("Ctrl+Q")), &w);
  a.connect(shortcut, SIGNAL(activated()), &a, SLOT(quit()));

  return a.exec();
}
