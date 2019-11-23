/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     liurui <liurui_cm@deepin.com>
 *
 * Maintainer: liurui <liurui_cm@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DPRINTERSSHOWWINDOW_H
#define DPRINTERSSHOWWINDOW_H
#include "dprintermanager.h"
#include "printersearchwindow.h"
#include "renameprinterwindow.h"

#include <DMainWindow>
#include <DListView>
#include <DTitlebar>
#include <DWidgetUtil>
#include <DFontSizeManager>
#include <DFrame>

#include <QCheckBox>
#include <QVBoxLayout>
#include <QEventLoop>
DWIDGET_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE
class DImageButton;
class DSettingsDialog;
class DDialog;
class DFloatingButton;
DWIDGET_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QLabel;
class QStandardItemModel;
class QMenu;
class QCheckBox;
QT_END_NAMESPACE

class ServerSettingsWindow : public DMainWindow
{
    Q_OBJECT
public:
    explicit ServerSettingsWindow(QWidget *parent = nullptr)
        : DMainWindow(parent)
    {
        initUI();
        initConnections();
    }
    int exec()
    {
        show();
        QEventLoop loop;
        connect(this, &ServerSettingsWindow::finished, &loop, &QEventLoop::quit);
        loop.exec();
        return 1;
    }
private:
    void initUI()
    {
        titlebar()->setMenuVisible(false);
        titlebar()->setTitle("");
        titlebar()->setIcon(QIcon(":/images/dde-printer.svg"));
        setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
        setWindowModality(Qt::ApplicationModal);
        setFixedWidth(480);

        QFont font;
        font.setBold(true);

        QLabel *pBaseSettings = new QLabel(tr("Basic Server Settings"));
        pBaseSettings->setFont(font);
        DFontSizeManager::instance()->bind(pBaseSettings, DFontSizeManager::T5);

        m_pCheckShared = new QCheckBox(tr("Publish shared printers connected to this system"));
        m_pCheckIPP = new QCheckBox(tr("Allow printing from the Internet"));
        m_pCheckIPP->setEnabled(false);
        m_pCheckRemote = new QCheckBox(tr("Allow remote administration"));
//        m_pCheckCancelJobs = new QCheckBox(tr("Allow users to cancel all tasks (not just their own)"));
        m_pCheckSaveDebugInfo = new QCheckBox(tr("Save debugging information for troubleshooting"));
        QVBoxLayout *pSettingsVLayout = new QVBoxLayout();
        pSettingsVLayout->setSpacing(1);
        pSettingsVLayout->addWidget(pBaseSettings);

        QVBoxLayout *pSettingsVLayout1 = new QVBoxLayout();
        pSettingsVLayout1->addWidget(m_pCheckShared);
        pSettingsVLayout1->addSpacing(3);
        QHBoxLayout *pSettingsHLayout = new QHBoxLayout();
        pSettingsHLayout->addSpacing(12);
        pSettingsHLayout->addWidget(m_pCheckIPP);
        pSettingsVLayout1->addLayout(pSettingsHLayout);
        DFrame *pFrame1 = new DFrame();
        pFrame1->setFixedHeight(64);
        pFrame1->setBackgroundRole(this->backgroundRole());
        pFrame1->setLayout(pSettingsVLayout1);

        pSettingsVLayout->addWidget(pFrame1);

        QVBoxLayout *pSettingsVLayout2 = new QVBoxLayout();
        pSettingsVLayout2->addWidget(m_pCheckRemote);
        DFrame *pFrame2 = new DFrame();
        pFrame2->setLayout(pSettingsVLayout2);
        pFrame2->setFixedHeight(36);
        pFrame2->setBackgroundRole(this->backgroundRole());
        pSettingsVLayout->addWidget(pFrame2);
//        pSettingsVLayout->addWidget(m_pCheckCancelJobs);

        QVBoxLayout *pSettingsVLayout3 = new QVBoxLayout();
        pSettingsVLayout3->addWidget(m_pCheckSaveDebugInfo);
        DFrame *pFrame3 = new DFrame();
        pFrame3->setFixedHeight(36);
        pFrame3->setBackgroundRole(this->backgroundRole());
        pFrame3->setLayout(pSettingsVLayout3);
        pSettingsVLayout->addWidget(pFrame3);

        DFrame *pSettingWidget = new DFrame();
        pSettingWidget->setLayout(pSettingsVLayout);
        pSettingsVLayout->setContentsMargins(10, 10, 10, 10);
        takeCentralWidget();
        setCentralWidget(pSettingWidget);
        moveToCenter(this);

    }
    void initConnections()
    {
        connect(m_pCheckShared, &QCheckBox::clicked, this, [this](bool checked) {
            if (checked) {
                m_pCheckIPP->setEnabled(true);
            } else {
                m_pCheckIPP->setEnabled(false);
                m_pCheckIPP->setChecked(false);
            }
        });
    }
protected:
    void closeEvent(QCloseEvent *event) override
    {
        emit finished();
        event->accept();
    }

public:
    QCheckBox *m_pCheckShared;
    QCheckBox *m_pCheckIPP;
    QCheckBox *m_pCheckRemote;
//    QCheckBox *m_pCheckCancelJobs;
    QCheckBox *m_pCheckSaveDebugInfo;
signals:
    void finished();
};

class DPrintersShowWindow : public DMainWindow
{
    Q_OBJECT
public:
    explicit DPrintersShowWindow(QWidget *parent = nullptr);
    virtual ~DPrintersShowWindow() override;


private:
    // 初始化UI
    void initUI();
    // 初始化信号槽连接
    void initConnections();
    /**
    * @projectName   Printer
    * @brief         过滤QListWidget默认代理的右键事件
    * @author        liurui
    * @date          2019-11-08
    */
//    bool eventFilter(QObject *watched, QEvent *event) override;

    void showEvent(QShowEvent *event) override;

    /**
    * @projectName   Printer
    * @brief         选中指定名称的打印机列表项
    * @author        liurui
    * @date          2019-11-07
    */
    void selectPrinterByName(const QString &printerName);
    /**
    * @projectName   Printer
    * @brief         更新打印机列表的默认选项图标
    * @author        liurui
    * @date          2019-11-08
    */
    void updateDefaultPrinterIcon();
private slots:
    // 添加打印机
    void addPrinterClickSlot();
    // 删除打印机
    void deletePrinterClickSlot();
    /**
    * @projectName   Printer
    * @brief         重命名打印机，策略是先复制打印机，然后删除当前打印机，最后启用新打印机
    * @author        liurui
    * @date          2019-11-07
    */
    void renamePrinterSlot(QStandardItem *pItem);

    void printSettingClickSlot();
    void printQueueClickSlot();
    void printTestClickSlot();
    void printFalutClickSlot();
    void printDriveInstall();

    void printerListWidgetItemChangedSlot(const QModelIndex &previous);
    // 响应列表的右键菜单
    void contextMenuRequested(const QPoint &point);
    // 响应菜单栏的action
    void listWidgetMenuActionSlot(bool checked);

    // 初始化左侧打印机列表
    void refreshPrinterListView(const QString &newPrinterName);
    // 服务器设置
    void serverSettingsSlot();

    bool eventFilter(QObject *watched, QEvent *event) override;

    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:

    // UI成员变量
    DIconButton *m_pBtnAddPrinter;
    DIconButton *m_pBtnDeletePrinter;
    QLabel *m_pLeftTipLabel;

    QLabel *m_pLabelPrinterName;
    QLabel *m_pLabelLocationShow;
    QLabel *m_pLabelTypeShow;
    QLabel *m_pLabelStatusShow;

    DFloatingButton *m_pTBtnSetting;
    DFloatingButton *m_pTBtnPrintQueue;
    DFloatingButton *m_pTBtnPrintTest;
    DFloatingButton *m_pTBtnFault;

    DListView *m_pPrinterListView;
    QStandardItemModel *m_pPrinterModel;
    QMenu *m_pListViewMenu;
    QAction *m_pShareAction;
    QAction *m_pEnableAction;
    QAction *m_pRejectAction;
    QAction *m_pDefaultAction;


    QWidget *m_pPrinterInfoWidget;
    QLabel *m_pPRightTipLabel1;
    QLabel *m_pPRightTipLabel2;

    PrinterSearchWindow *m_pSearchWindow;
    ServerSettingsWindow *m_pSettingsDialog;
    QAction *m_pSettings;

private:
    // 数据成员变量
    DPrinterManager *m_pPrinterManager;
    // 当前选中的打印机名称
    QString m_CurPrinterName;
};

#endif // DPRINTERSSHOWWINDOW_H
