#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QTimer>


#include <vector>
#include <sstream>
#include <string>
#include <iostream>

using std::vector;
using std::string;
using std::endl;
using std::cout;


/**
 * @description: 分割字符串
 * @param {string} &str
 * @param {char} delim
 * @param {vector<string>} &elems
 * @return {*}
 */
vector<string> &split(const string &str, char delim, vector<string> &elems) {
  std::stringstream ss(str);
  string item;
  while (std::getline(ss, item, delim))
    elems.push_back(item);

  return elems;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    clipboard = QGuiApplication::clipboard();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_update()));

    ui->checkbox_enable_formatting->setChecked(true);
    ui->checkBox_auto_copy->setChecked(true);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::timer_update(){
    qDebug("timer update");
    QString ct = clipboard->text();

    // 当剪切板中的内容与原始内容和格式化后的内容都不同时，进行操作
    if(ct != origin_str && ct != formatted_str ){
        // 有新文本
        qDebug("Update str.");
        origin_str = ct;
        ui->orign_text->setText(origin_str);

        // 判断是否需要格式化操作
        if(ui->checkbox_enable_formatting->isChecked()){
            // 格式化文本
            this->format_string(origin_str,formatted_str);
            ui->formatted_text->setText(formatted_str);

            // 判断是否要自动复制
            if(ui->checkBox_auto_copy->isChecked()){
                // 自动复制
                clipboard->setText(formatted_str);
            }
        }
    }



}

void Widget::format_string(QString &ori_str, QString &f_str){
    qDebug("Fomartting string");

    string tmp_str = ori_str.toStdString();
    vector<string> vstr;
    split(tmp_str, '\n', vstr);

    std::stringstream ss;
    for(auto&s:vstr){
        if(s.size() >=1 ){
            if(s.back() == '-'){
                ss<< s;
            }else if(s.back() == ';' || s.back() == '.'){
                ss<< s << "\n";
            }else{
                ss << s<< " ";
            }
        }else{
            ss<<s<<"\n";
        }
    }
    ss<<std::endl;

    std::cout<<"After formatted : "<<ss.str();
    f_str = QString::fromStdString(ss.str());

}

void Widget::on_checkbox_enable_formatting_stateChanged(int arg1)
{
    if (this->ui->checkbox_enable_formatting->isChecked()){
        qDebug("enable formatting checked, Start timer.");
        this->timer->start(2000);
    }else{
        qDebug("enable formatting unchecked, Stop timer.");
        this->timer->stop();
    }

}

void Widget::on_checkBox_auto_copy_stateChanged(int arg1)
{
    if (this->ui->checkBox_auto_copy->isChecked()){
        qDebug("checkBox_auto_copy checked");
    }else{
        qDebug("checkBox_auto_copy unchecked");
    }
}
