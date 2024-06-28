#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QTextStream>
#include <QSettings>
#include <QUrl>
#include <QMultiMap>
#include <QUrlQuery>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->addWidget(new QLabel("1.0.0 xyl dsdbe@qq.com", ui->statusbar));
    loadConfig();
    on_autoButton_clicked();
}

MainWindow::~MainWindow()
{
    saveConfig();
    delete ui;
}

void MainWindow::showSRUrl(QString path)
{
    int ret = getSRUrl(path);
    if(0 == ret)
    {
        ui->plainTextEdit->setPlainText(srUrl);
        ui->infoTextEdit->insertPlainText("获取成功，请在下方复制抽卡链接\n");
//        QUrl url(srUrl);
//        if (url.isValid())
//        {
//            // 获取URL的查询部分
//            QUrlQuery query(url.query());
//            QString param1Value = query.queryItemValue("timestamp");
//            ui->infoTextEdit->insertPlainText("当前抽卡链接时间戳为:" + param1Value + "("+ QDateTime::fromTime_t(param1Value.toUInt()).toString("yyyy/MM/dd") + ")请注意抽卡链接有效期\n");
//        }
    }
    else if(1 == ret)
    {
        ui->infoTextEdit->insertPlainText("无法获取抽卡链接，请先查阅跃迁记录后重试\n");
    }
    else if(2 == ret)
    {
        ui->infoTextEdit->insertPlainText("无法打开缓存文件，请关闭游戏后重试，或检查安装路径后手动打开\n");
    }
}

int MainWindow::getSRUrl(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        QString str = in.readAll();
        file.close();
        QString h = "https://api-takumi.mihoyo.com/common/gacha_record";
        QString e = "end_id";
        QString b = "begin_id";
        if(str.contains(h))
        {
            int start = str.lastIndexOf(h);
            if(-1 != start)
            {
                int end = str.indexOf(e,start);
                if(-1 != end)
                {
                    srUrl = str.mid(start, end - start - 1);
                    return 0;
                }
                else
                {
                    end = str.indexOf(b,start);
                    if(-1 != end)
                    {
                        srUrl = str.mid(start, end - start - 1);
                        return 0;
                    }
                }
            }
        }
        return 1;
        //return "无法获取抽卡链接，请先查阅跃迁记录后重试";
    }
    else
    {
        return 2;
        //return "无法打开缓存文件，请关闭游戏后重试";
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->infoTextEdit->clear();
    ui->plainTextEdit->clear();
    QString fileName = QFileDialog::getOpenFileName(nullptr,QObject::tr("Open File"), path.isEmpty() ? "C:\\Program Files\\Star Rail\\Game\\StarRail_Data\\webCaches\\2.25.0.0\\Cache\\Cache_Data" : path, QObject::tr("All Files (*)"));
    if (!fileName.isEmpty())
    {
        path = fileName;
        ui->infoTextEdit->insertPlainText("尝试从\n" + fileName + "\n获取抽卡链接\n");
        showSRUrl(fileName);
    }
}

void MainWindow::on_autoButton_clicked()
{
    ui->infoTextEdit->clear();
    ui->plainTextEdit->clear();
    QString path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\崩坏：星穹铁道";
    QSettings settings(path, QSettings::Registry64Format);
    QString installString = settings.value("InstallPath").toString();
    ui->infoTextEdit->insertPlainText("正在自动获取您的崩坏：星穹铁道安装路径\n");
    if(installString.isEmpty())
    {
        ui->infoTextEdit->insertPlainText("无法获取崩铁安装路径，请尝试手动打开以下路径的文件\n");
        ui->infoTextEdit->insertPlainText("游戏目录\\Game\\StarRail_Data\\webCaches\\2.25.0.0\\Cache\\Cache_Data\\data_2\n");
        return;
    }
    ui->infoTextEdit->insertPlainText("您的崩铁路径为：\n" + installString + "\n");
    QString p = installString + "\\Game\\StarRail_Data\\webCaches\\2.25.0.0\\Cache\\Cache_Data\\data_2";
    ui->infoTextEdit->insertPlainText("正在尝试打开缓存文件\n" + p + "\n");
    showSRUrl(p);
}


void MainWindow::saveConfig()
{
    QJsonObject jsonObj;
    if(path.isEmpty())
    {
        path = QDir::homePath();
    }
    jsonObj["path"] = path;
    QJsonDocument jsonDoc(jsonObj);
    QFile configFile("config.json");
    if (!configFile.open(QIODevice::WriteOnly))\
    {
        return;
    }
    configFile.write(jsonDoc.toJson());
    configFile.close();
}

void MainWindow::loadConfig()
{
    QFile configFile("config.json");
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QByteArray fileContent = configFile.readAll();
    configFile.close();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent, &jsonError);
    if (jsonError.error != QJsonParseError::NoError || !jsonDoc.isObject())
    {
        return;
    }
    QJsonObject jsonObject = jsonDoc.object();
    if (jsonObject.contains("path"))
    {
        path = jsonObject["path"].toString();
    }
}
