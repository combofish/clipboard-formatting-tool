#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QTimer>
#include <QShortcut>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

/**
 * @description: 分割字符串
 * @param {string} &str
 * @param {char} delim
 * @param {vector<string>} &elems
 * @return {*}
 */
vector <string> &split(const string &str, char delim, vector <string> &elems);

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    ui->hide_btn->setText("Hide");
    ui->checkbox_enable_formatting->setChecked(true);
    ui->checkBox_auto_copy->setChecked(true);

    clipboard = QGuiApplication::clipboard();

    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(text_changed()));
    connect(ui->hide_btn, SIGNAL(clicked()), this, SLOT(hide()));

    setWindowFlags(Qt::Tool);
    CreateSystemTrayIcon();
//    this->hide();
}

Widget::~Widget() { delete ui; }


void Widget::CreateSystemTrayIcon() {
    //初始化两个项目
    auto showAction = new QAction(QStringLiteral("显示"));//项1
    auto exitAction = new QAction(QStringLiteral("退出"));//项2
    //项1的点击槽函数
    connect(showAction, SIGNAL(triggered()), this, SLOT(show()));
    //项2的点击槽函数
    connect(exitAction, &QAction::triggered, [&]() {
        QApplication::exit();
    });

    //初始化菜单并添加项
    auto trayMenu = new QMenu(this);  //菜单
    trayMenu->addAction(showAction);
    trayMenu->addSeparator();
    trayMenu->addAction(exitAction);

    //创建一个系统托盘
    auto trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/m-格式化文字.png"));  //设置托盘图标
    trayIcon->setContextMenu(trayMenu);  //设置菜单

    //    设置单击显示主界面
    connect(trayIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason r) {
        if (r == QSystemTrayIcon::ActivationReason::Trigger) emit
        showAction->triggered();
    });

    trayIcon->show();
}

void Widget::text_changed() {
    QString ct;
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasImage()) {
        // 不处理图片
        return;
    }
    if (mimeData->hasText()) {
        ct = mimeData->text();

        // 不处理文件或目录的复制操作
        if (ct.startsWith("file://"))
            return;
    }
    // QString ct = clipboard->text();

    // 当剪切板中的内容与原始内容和格式化后的内容都不同时，进行操作
    if (ct != origin_str && ct != formatted_str && !ct.isEmpty()) {
        // 有新文本
        qDebug("Update str.");
        origin_str = ct;
        ui->orign_text->setText(origin_str);

        // 判断是否需要格式化操作
        if (ui->checkbox_enable_formatting->isChecked()) {
            // 格式化文本
            this->format_string(origin_str, formatted_str);
            ui->formatted_text->setText(formatted_str);

            // 判断是否要自动复制
            if (ui->checkBox_auto_copy->isChecked()) {
                // 自动复制
                clipboard->setText(formatted_str);
            }
        }
    }
}

void Widget::format_string(QString &ori_str, QString &f_str) {
    qDebug("Fomartting string");

    string tmp_str = ori_str.toStdString();
    vector <string> vstr;
    split(tmp_str, '\n', vstr);

    std::stringstream ss;
    if (vstr.size() > 1) {
        // 有多行数据
        for (auto it = vstr.begin(); it != vstr.end() - 1; it++) {
            auto s = *it;

            if (s.size() >= 1) {
                if (s.back() == '-') {
                    ss << s;
                } else if (s.back() == ';' || s.back() == '.') {
                    ss << s << "\n";
                } else {
                    ss << s << " ";
                }
            } else {
                ss << s << "\n";
            }
        }
    }

    // 处理最后一行数据
    ss << vstr.back();

    // std::cout<<"After formatted : "<<ss.str();
    f_str = QString::fromStdString(ss.str());
}

/**
 * @description: 分割字符串
 * @param {string} &str
 * @param {char} delim
 * @param {vector<string>} &elems
 * @return {*}
 */
vector <string> &split(const string &str, char delim, vector <string> &elems) {
    std::stringstream ss(str);
    string item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);

    return elems;
}
