#include "stdafx.h"
#include "VedioPlayUI.h"
#include "vhallprocessslider.h"
#include "push_button.h"
#include "ToolButton.h"
#include <QLabel>
#include <QToolButton>
#include <QBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QPixmap>
#include <QToolTip>
#include <QBitmap>
#include "vhallmenuwidgetitem.h"
#include "vhallmenuwidget.h"
#include "VHDialog.h"
#include "../../Unility/Unility.h"

extern ToolManager *globalToolManager;

VedioPlayUI::VedioPlayUI(QWidget *parent)
: m_pProgressSlider(NULL)
, m_pPlayBtn(NULL)
, m_pTimeLbl(NULL)
, m_pVolumeBtn(NULL)
, m_pAddFileBtn(NULL)
, m_pVolumeSlider(NULL)
, m_pFileListBtn(NULL)
, m_pLoopSettingBtn(NULL)
, m_pExitBtn(NULL)
, m_pMenuCircleControl(NULL)
, m_pMenuPlayList(NULL)
, m_pControlLayout(NULL)
, m_pVolumeLayout(NULL)
, m_pMainLayout(NULL)
, m_iVolume(100)
, m_bIsMute(false)
, QWidget(parent)

{
   //无标题栏窗体
   setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
   setFixedHeight(55);
   setFixedWidth(896);
   setAttribute(Qt::WA_TranslucentBackground);
   setAutoFillBackground(false);
}

VedioPlayUI::~VedioPlayUI() {

}

bool VedioPlayUI::Create() {
   //进度条
   m_pProgressSlider = new VHallProcessSlider(Qt::Horizontal, this);
   connect(m_pProgressSlider, SIGNAL(sigValueChange(int)), this, SLOT(OnProgressChanged(int)));
   m_pProgressSlider->setObjectName("MediaPlayerSlider");
   m_pProgressSlider->setMinimumHeight(5);
   m_pProgressSlider->setMaximumHeight(5);
   m_pProgressSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
   m_pProgressSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

   //播放按钮
   m_pPlayBtn = new PushButton(this);
   connect(m_pPlayBtn, SIGNAL(clicked()), this, SLOT(OnPlayClicked()));
   m_pPlayBtn->loadPixmap(":/sysButton/mediaPlayStart");
   m_pPlayBtn->setToolTip(QStringLiteral("播放"));

   //时间显示
   m_pTimeLbl = new QLabel(this);
   m_pTimeLbl->setText(QString("00:00:00/00:00:00"));
   m_pTimeLbl->setStyleSheet("color:#B0B0B0;");

   //添加文件按钮   
   m_pAddFileBtn = new ToolButton(":/mediaReader/InsertVideo",this);
   connect(m_pAddFileBtn, SIGNAL(sigClicked()), this, SLOT(OnAddFiles()));
   m_pAddFileBtn->setToolTip(QStringLiteral("添加视频"));

   //静音按钮
   m_pVolumeBtn = new PushButton(this);
   connect(m_pVolumeBtn, SIGNAL(sigClicked()), this, SLOT(OnMuteClicked()));
   m_pVolumeBtn->loadPixmap(":/sysButton/mediaPlayVolumeOn");
   m_pVolumeBtn->setToolTip(QStringLiteral("静音"));

   //音量调节   
   m_pVolumeSlider = new QSlider(Qt::Horizontal, this);
   m_pVolumeSlider->setFixedWidth(150);
   m_pVolumeSlider->setMaximum(100);
   m_pVolumeSlider->setValue(100);
   m_pVolumeSlider->setStyleSheet(
      "QSlider{"
      "border-radius: 2px;"
      "margin:0px 0px;"
      "}"
      "QSlider::groove:horizontal {"
      "border: 1px solid rgb(209,209,209);"
      "height: 2px;"
      "margin: 0px 0px;"
      "left: 5px;"
      "right: 5px;"
      "}"
      "QSlider::handle:horizontal {"
      "background: rgb(209,209,209);"
      "border-radius: 5px;"
      "border: 1px solid rgb(209,209,209);"
      "width: 10px;"
      "height: 12px;"
      "margin: -5px -5px;"
      "}"
      "QSlider::sub-page:horizontal{"
      "background: rgb(209,209,209);"
      "}"
      "QSlider::add-page:horizontal{"
      "background: rgb(80,80,80);"
      "}"
   );

   
   connect(m_pVolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(OnVolumnChanged(int)));


   //文件列表按钮
   m_pFileListBtn = new PushButton( this);
   m_pFileListBtn->setObjectName("mMediaFileListBtn");
   m_pFileListBtn->loadPixmap(":/sysButton/mediaPlayFileList");
   m_pFileListBtn->setToolTip(QStringLiteral("视频列表"));

   connect(m_pFileListBtn, SIGNAL(sigClicked()), this, SLOT(OnPlayListClick()));
   connect(m_pFileListBtn,SIGNAL(sigEnterIn()),this,SLOT(OnEnterFileList()));

   //循环按钮
   m_pLoopSettingBtn = new ToolButton(":/mediaReader/SeqLoop",this);
   m_pLoopSettingBtn->setObjectName("mMediaLoopSettingBtn");

   connect(m_pLoopSettingBtn, SIGNAL(sigClicked()), this, SLOT(OnPlayModeMenuClick()));
   connect(m_pLoopSettingBtn, SIGNAL(sigEnter()), this, SLOT(OnEnterCircleList()));

   //退出按钮
   m_pExitBtn = new ToolButton(":/mediaReader/StopPlay",this);
   connect(m_pExitBtn, SIGNAL(sigClicked()), this, SLOT(OnStopPlay()));

   //控制菜单布局
   m_pControlLayout = new QHBoxLayout();
   m_pControlLayout->addWidget(m_pPlayBtn);
   m_pControlLayout->addWidget(m_pTimeLbl);

   //音量滚动条布局
   m_pVolumeLayout = new QHBoxLayout();
   m_pVolumeLayout->addWidget(m_pVolumeBtn);
   m_pVolumeLayout->addWidget(m_pVolumeSlider);
   m_pVolumeLayout->setSpacing(5);

   //控制布局添加窗体
   m_pControlLayout->addLayout(m_pVolumeLayout);
   m_pControlLayout->addWidget(m_pAddFileBtn);
   m_pControlLayout->addWidget(m_pFileListBtn);
   m_pControlLayout->addWidget(m_pLoopSettingBtn);
   m_pControlLayout->addWidget(m_pExitBtn);
   m_pControlLayout->setSpacing(20);
   m_pControlLayout->setContentsMargins(16, 0, 16, 0);

   //主菜单布局
   m_pMainLayout = new QVBoxLayout(this);
   m_pMainLayout->setContentsMargins(0, 0, 0, 0);
   m_pMainLayout->setSpacing(0);
   m_pMainLayout->addWidget(m_pProgressSlider);

   //控制部分窗体
   m_pControlWidget = new QWidget(this);
   m_pControlWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
   m_pControlWidget->setLayout(m_pControlLayout);

   //主布局中添加控制窗体
   m_pMainLayout->addWidget(m_pControlWidget);
   //设置主布局
   setLayout(m_pMainLayout);

   //初始化循环控制菜单
   InitCycleControl();

   //初始化播放列表
   InitPlayList();
 
   
   //动画
   m_propertyAdmin = new QPropertyAnimation(this, "geometry");
   connect(m_propertyAdmin,SIGNAL(finished()),this,SLOT(SlotAdminFinished()));
   connect(&m_LeaveTimer,SIGNAL(timeout()),this,SLOT(LeaveTimerTimeout()));   

   FlushCircleButton(false);

   installEventFilter(this);
   m_pVolumeSlider->installEventFilter(this);

   m_pPadding = new QWidget(this);
   m_pPadding->setStyleSheet("background-color:rgba(0,0,0,180);");
   m_pPadding->resize(this->width(),this->height());

   QPixmap m_mainMask=QPixmap(this->size());
   
   m_mainMask = QPixmap (this->size());
   m_mainMask.fill(Qt::black);
   QPainter p(&m_mainMask);
   
   QPixmap r=QPixmap(m_pExitBtn->size());
   r.fill(Qt::white);
  
   p.drawPixmap(this->width()-m_pExitBtn->width()-16
      ,15
      ,m_pExitBtn->width(),m_pExitBtn->height(),r);
   
   m_pPadding->setMask(m_mainMask);

   
   return true;
}

void VedioPlayUI::Destroy() {
   UnInitPlayList();
   UnInitCycleControl();
   
   if(m_propertyAdmin != NULL) {
      delete m_propertyAdmin;
      m_propertyAdmin=NULL;
   }

   if (NULL != m_pControlLayout) {
      delete m_pControlLayout;
      m_pControlLayout = NULL;
   }

   if (NULL != m_pControlWidget) {
      delete m_pControlWidget;
      m_pControlWidget = NULL;
   }

   if (NULL != m_pMenuPlayList) {
      delete m_pMenuPlayList;
      m_pMenuPlayList = NULL;
   }

   if (NULL != m_pMenuCircleControl) {
      delete m_pMenuCircleControl;
      m_pMenuCircleControl = NULL;
   }

   if (NULL != m_pProgressSlider) {
      delete m_pProgressSlider;
      m_pProgressSlider = NULL;
   }
}

void VedioPlayUI::InitPlayList() {
   m_pMenuPlayList = new VHallMenuWidget(NULL);
   if (NULL == m_pMenuPlayList) {
      //ASSERT(FALSE);
      return;
   }
   m_pMenuPlayList->LoadPixmap(":/mediaReader/PlayListBackground");

   m_pMenuPlayList->setIsFileCheck(true);
   m_pMenuPlayList->setLoadEnable(true);
   connect(m_pMenuPlayList, 
      SIGNAL(vHallMenuWidgetChecked(VHallMenuWidgetItem *)), 
      this, 
      SLOT(OnPlayListChecked(VHallMenuWidgetItem *)));
   
   connect(m_pMenuPlayList, 
      SIGNAL(SigClose()), 
      this, 
      SLOT(SlotPlayListClose()));
   m_pMenuPlayList->setNoneTip(QStringLiteral("视频插播列表为空"));
}

int VedioPlayUI::PlayListNum()
{
   int num=0;
   if(m_pMenuPlayList) {
      num=m_pMenuPlayList->count();
   }
   return num;
}

QStringList VedioPlayUI::GetPlayList(int &currentIndex) {
   QStringList playList;
   if(m_pMenuPlayList) {
      for(int i=0;i<m_pMenuPlayList->count();i++) {
         QString filename=m_pMenuPlayList->itemString(i);
         playList.append(filename);

         VHallMenuWidgetItem *item=m_pMenuPlayList->item(i);
         if(item) {
            if(item==m_pMenuPlayList->currentItem()){
               currentIndex=i;
            }
         }
      }
   }
   return playList;
}

void VedioPlayUI::UnInitPlayList() {
   if (NULL != m_pMenuPlayList) {
      delete m_pMenuPlayList;
      m_pMenuPlayList = NULL;
   }
}

void VedioPlayUI::InitCycleControl() {
   //循环列表
   m_pMenuCircleControl = new VHallMenuWidget(NULL);
   connect(m_pMenuCircleControl, SIGNAL(vHallMenuWidgetChecked(VHallMenuWidgetItem *)), this, SLOT(OnCycleControl(VHallMenuWidgetItem *)));

   VHallMenuWidgetItem *item = NULL;

   item = new VHallMenuWidgetItem(m_pMenuCircleControl);
   item->setText(QStringLiteral("顺序播放"));
   item->setData(0, QVariant((int)CycleTyle_None));
   item->setSelect(true);
   item->setLabelCenter();
   m_pMenuCircleControl->append(item);

   item = new VHallMenuWidgetItem(m_pMenuCircleControl);
   item->setText(QStringLiteral("列表循环"));
   item->setData(0, QVariant((int)CycleTyle_List));
   item->setLabelCenter();
   m_pMenuCircleControl->append(item);

   item = new VHallMenuWidgetItem(m_pMenuCircleControl);
   item->setText(QStringLiteral("单视频循环"));
   item->setData(0, QVariant((int)CycleTyle_Single));
   item->setLabelCenter();
   m_pMenuCircleControl->append(item);

   m_pMenuCircleControl->setMaxItemCountShow((int)CycleTyle_END);
   m_CycleType = CycleTyle_None;
}

void VedioPlayUI::UnInitCycleControl() {
   if (NULL != m_pMenuCircleControl) {
      m_pMenuCircleControl->clear();
      delete m_pMenuCircleControl;
      m_pMenuCircleControl = NULL;
   }
}

bool VedioPlayUI::GetCurFile(QString& selectFile) {
   if (NULL != m_pMenuPlayList) {
      VHallMenuWidgetItem *item = m_pMenuPlayList->currentItem();
      if (item) {
         QString qStrFileName = item->getData(0).toString();
         selectFile = qStrFileName.replace("/", "\\");
         return true;
      }
   }
   return false;
}

void VedioPlayUI::AddPlayList(char* fileName) {
   if (NULL != m_pMenuPlayList) {
      m_pMenuPlayList->addItem(fileName);
   }
}
void VedioPlayUI::AppendPlayList(QStringList &playList,int currentIndex) {
   if (NULL != m_pMenuPlayList) {
      if(m_pMenuPlayList->setItems(playList,currentIndex)) {
      }
   }
}

void VedioPlayUI::SwitchToNext() {
   VHallMenuWidgetItem* pCurItem = m_pMenuPlayList->nextItem();
   if (pCurItem) {
      m_pMenuPlayList->setSelected(pCurItem);
   }
}

int VedioPlayUI::GetCurVolume() {
   return m_bIsMute?0:m_iVolume;
}

CycleType VedioPlayUI::GetCurPlayMode() {
   return m_CycleType;
}
void VedioPlayUI::SetCurPlayMode(CycleType type) {
   if(m_pMenuCircleControl) {
      for(int i=0;i<m_pMenuCircleControl->count();i++) {
         VHallMenuWidgetItem *item=m_pMenuCircleControl->item(i);
         if(item) {
            QVariant mode= item->getData(0);
            if(mode.toInt()==(int)type) {
               m_pMenuCircleControl->setSelected(item);
               m_CycleType=type;
               break;
            }
         }
      }
   }
   FlushCircleButton();
}
void VedioPlayUI::FlushCircleButton(bool bShow) {
   if(!m_pLoopSettingBtn) {
      return ;
   }
   
   switch(m_CycleType) {
      case CycleTyle_None:
         m_pLoopSettingBtn->changeImage(":/mediaReader/SeqLoop");
         m_pLoopSettingBtn->showTips(this->cursor().pos(), QStringLiteral("顺序播放"),bShow);
         break;
      case CycleTyle_List:
         m_pLoopSettingBtn->changeImage(":/mediaReader/ListLoop");
         m_pLoopSettingBtn->showTips(this->cursor().pos(), QStringLiteral("列表循环"),bShow);
         break;
      case CycleTyle_Single: 
         m_pLoopSettingBtn->changeImage(":/mediaReader/SingleLoop");
         m_pLoopSettingBtn->showTips(this->cursor().pos(), QStringLiteral("单视频循环"),bShow);
         break;
      default:
         break;
   }
   m_pLoopSettingBtn->repaint();
}
void VedioPlayUI::UpdatePlayTime(QString qPlayTime) {
   if (m_pTimeLbl) {
      m_pTimeLbl->setText(qPlayTime);
   }
}

void VedioPlayUI::UpdatePlayStates(bool bPlay) {
   if (bPlay) {
      m_pPlayBtn->loadPixmap(":/sysButton/mediaPlayPause");
      m_pPlayBtn->setToolTip(QStringLiteral("暂停"));
   } else {
      m_pPlayBtn->loadPixmap(":/sysButton/mediaPlayStart");
      m_pPlayBtn->setToolTip(QStringLiteral("播放"));
   }
}

bool VedioPlayUI::IsArriveTheEnd() {
   VHallMenuWidgetItem *currentItem = m_pMenuPlayList->currentItem();
   int index = m_pMenuPlayList->ItemIndex(currentItem);

   if (m_pMenuPlayList->count() == index + 1) {
      return true;
   }
   return false;
}

void VedioPlayUI::SetProgressValue(int nCurValue, int nMaxValue) {
   if (m_pProgressSlider) {
      m_pProgressSlider->setValue(nCurValue);
      m_pProgressSlider->setMaxValue(nMaxValue);
   }
}

void VedioPlayUI::OnProgressChanged(int nValue) {

		//VH::CComPtr<IMainUILogic> pMainUILogic;
		//DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_IMainUI, IID_IMainUILogic, pMainUILogic, return, ASSERT(FALSE));
		//pMainUILogic->SetPlayMediaFilePos(nValue, eLiveType_VhallActive);

		//if (mMediaStream) {
		//	mMediaStream->FileSeek(n64Pos);
		//}

   globalToolManager->GetPaasSDK()->MediaFileSeek(nValue);
   if (m_pProgressSlider) {
      m_pProgressSlider->reActive();
   }
}

void VedioPlayUI::OnVolumnChanged(int nValue) {
   QToolTip::showText(QCursor::pos(), QString("%1").arg(nValue));
   if (nValue != 0) {
      m_pVolumeBtn->loadPixmap(":/sysButton/mediaPlayVolumeOn");
      m_pVolumeBtn->setToolTip(QStringLiteral("静音"));
      m_bIsMute = false;
   } else {
      m_pVolumeBtn->loadPixmap(":/sysButton/mediaPlayVolumeOff");
      m_pVolumeBtn->setToolTip(QStringLiteral("恢复"));
      m_bIsMute = true;
   }

   m_iVolume = nValue;
   SetDeviceSystemVolume(nValue);
}

void VedioPlayUI::OnPlayClicked() {
   /*STRU_MAINUI_PLAY_CLICK loPlay;
   SingletonMainUIIns::Instance().PostCRMessage(MSG_MAINUI_VEDIOPLAY_PLAY, &loPlay, sizeof(STRU_MAINUI_PLAY_CLICK));*/
	emit sig_PlayClicked();
}

void VedioPlayUI::OnAddFiles() {
   SlotSetForceActice(false);
   emit sig_AddFiles();
   //SingletonMainUIIns::Instance().PostCRMessage(MSG_MAINUI_VEDIOPLAY_ADDFILE, NULL, 0);
}
void VedioPlayUI::SlotSetForceActice(bool bActive) {
   m_bEnterEventActive=m_bForceEventActive = bActive;
}

bool VedioPlayUI::IsShow() {
   return !(this->isHidden());
}
void VedioPlayUI::Enabled(bool bEnable){
   m_pPadding->setVisible(!bEnable);
}

void VedioPlayUI::SlotFlushPlayList(QStringList fileList,int current){   
	//if (m_pMenuPlayList) {
	//	VH::CComPtr<IOBSControlLogic> pOBSControlLogic;
	//	DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_IOBSControl, IID_IOBSControlLogic, pOBSControlLogic, return, ASSERT(FALSE));

	//	m_pMenuPlayList->Flush(fileList);
	//	if (current >= 0 && current<fileList.count()) {
	//		QString currentFileName = fileList[current];
	//		if (currentFileName != m_currentFile) {
	//			m_currentFile = currentFileName;
	//			SingletonMainUIIns::Instance().PostCRMessage(MSG_MAINUI_MAINUI_PLAYLISTCHG, NULL, 0);
	//		}
	//	}
	//}
}

void VedioPlayUI::SlotPlayListClose() {
   SlotSetForceActice(true);
   TryHideToolBar();
}

void VedioPlayUI::OnStopPlay() {
	emit sig_stoplay();
   //SingletonMainUIIns::Instance().PostCRMessage(MSG_MAINUI_VEDIOPLAY_STOPPLAY, NULL, 0);
}

void VedioPlayUI::OnShowPlayList() {
   if (!m_pFileListBtn || !m_pMenuPlayList) {
      //ASSERT(FALSE);
      return;
   }
   
   SlotSetForceActice(false);
   QPoint pos = mapToGlobal(QPoint(m_pFileListBtn->x() + m_pFileListBtn->width() / 2 - this->m_pMenuPlayList->width() / 2, 0));
   m_pMenuPlayList->leftDownMoveShow(pos.x()+2, pos.y());
}

void VedioPlayUI::OnHidePlayList() {
   if (!m_pMenuPlayList) {
      //ASSERT(FALSE);
      return;
   }
}
void VedioPlayUI::OnEnterFileList() {
   if(!m_pMenuPlayList) {
      return ;
   }
   m_bIsFileListShow=!m_pMenuPlayList->isHidden();
}

void VedioPlayUI::OnEnterCircleList() {
   FlushCircleButton();
}

void VedioPlayUI::OnPlayListClick() {
   if(!m_pMenuPlayList) {
      return ;
   }

   if(!m_bIsFileListShow){
      OnShowPlayList();
   }
   else {
      OnHidePlayList();
   }
   m_bIsFileListShow=!m_bIsFileListShow;

   //VH::CComPtr<ICommonData> pCommonData;
   //DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
   //pCommonData->ReportEvent("crsp06cbt03");
}

void VedioPlayUI::OnShowPlayModeMenu() {
   if (!m_pLoopSettingBtn || !m_pMenuCircleControl) {
      //ASSERT(FALSE);
      return;
   }
   QPoint pos = mapToGlobal(QPoint(m_pLoopSettingBtn->x() + m_pLoopSettingBtn->width() / 2 - m_pMenuCircleControl->width() / 2, 0));
   m_pMenuCircleControl->leftDownMoveShow(pos.x(), pos.y());
}

void VedioPlayUI::OnHidePlayModeMenu() {
   if (!m_pMenuCircleControl) {
      //ASSERT(FALSE);
      return;
   }
}

void VedioPlayUI::OnPlayModeMenuClick() {
   switch(m_CycleType) {
      case CycleTyle_None:
         m_CycleType=CycleTyle_List;
         break;
      case CycleTyle_List:
         m_CycleType=CycleTyle_Single;
         break;
      case CycleTyle_Single: 
         m_CycleType=CycleTyle_None;
         break;
      default:
         break;
   }
   FlushCircleButton();
   //VH::CComPtr<ICommonData> pCommonData;
   //DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
   //pCommonData->ReportEvent("crsp06cbt04");
}

void VedioPlayUI::OnCycleControl(VHallMenuWidgetItem *item) {
   m_CycleType = (enum CycleType)item->getData(0).toInt();
}
void VedioPlayUI::OnPlayListChecked(VHallMenuWidgetItem * item) {
   qDebug()<<"####VedioPlayUI::OnPlayListChecked####";
   if (item) {
      m_pMenuPlayList->setSelected(item);
   }
   emit sig_PlayListChecked();
   //if(0==m_pMenuPlayList->count()){
   //   SingletonMainUIIns::Instance().PostCRMessage(MSG_MAINUI_VEDIOPLAY_STOPPLAY, NULL, 0);
   //}
   //else {
   //   SingletonMainUIIns::Instance().PostCRMessage(MSG_MAINUI_MAINUI_PLAYLISTCHG, NULL, 0);
   //}
}

void VedioPlayUI::paintEvent(QPaintEvent *) {
   QPainter painter(this);
   painter.setRenderHint(QPainter::Antialiasing, true);
   painter.fillRect(this->rect(), QColor(0, 0, 0, 180));
}

void VedioPlayUI::hideEvent(QHideEvent *e)
{
   m_pMenuCircleControl->hide();
   m_pMenuPlayList->hide();
   QWidget::hideEvent(e);
}

void VedioPlayUI::resizeEvent(QResizeEvent *e){
   QWidget::resizeEvent(e);
}

void VedioPlayUI::TryShowToolBar() {
   if(m_bForceHideAdmin) {
      return ;
   }
   if(!m_bEnterEventActive||!m_bForceEventActive) {      
      return ;
   }
   setFocus();
   if(m_bMiniMode) {
      SetMiniMode(m_bMiniMode=false);
   }
   else {
      m_LeaveTimer.stop();
   }
}

void VedioPlayUI::TryHideToolBar(int pauseMsec) {
   if(m_bForceHideAdmin) {
      return ;
   }
   
   if(!m_bEnterEventActive||!m_bForceEventActive) {
      return ;
   }
   
   if(!m_bMiniMode) {      
      m_LeaveTimer.start(pauseMsec);
   }
   else {
   }
}


void VedioPlayUI::enterEvent(QEvent *e) { 
   if(m_bForceHideAdmin) {
      return ;
   }

   if (VHDialog::VHDialogShowCount()==0) {
      TryShowToolBar();
   }

   QWidget::enterEvent(e);
}
void VedioPlayUI::LeaveTimerTimeout() {
   m_LeaveTimer.stop();
   if(m_bForceHideAdmin) {
      return ;
   }
   SetMiniMode(m_bMiniMode=true);
}

void VedioPlayUI::leaveEvent(QEvent *e) {
   if(m_bForceHideAdmin) {
      return ;
   }
   
   TryHideToolBar();
   QWidget::leaveEvent(e);
}

void VedioPlayUI::OnMuteClicked() {
   if (!m_pVolumeSlider || !m_pVolumeBtn) {
      //ASSERT(FALSE);
      return;
   }

   m_bIsMute = !m_bIsMute;
   disconnect(m_pVolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(OnVolumnChanged(int)));
   if (m_bIsMute) {
      m_pVolumeBtn->loadPixmap(":/sysButton/mediaPlayVolumeOff");
      m_pVolumeBtn->setToolTip(QStringLiteral("恢复"));
      m_pVolumeSlider->setValue(0);
   } else {
      m_pVolumeBtn->loadPixmap(":/sysButton/mediaPlayVolumeOn");
      m_pVolumeBtn->setToolTip(QStringLiteral("静音"));
      if (m_iVolume==0) {
         m_iVolume=100;
      }
      m_pVolumeSlider->setValue(m_iVolume);
   }
   connect(m_pVolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(OnVolumnChanged(int)));
   SetDeviceSystemVolume(m_bIsMute ? 0 : m_iVolume);
}

void VedioPlayUI::SlotAdminFinished() {  
   qDebug()<<"VedioPlayUI::SlotAdminFinished";
   if(m_bForceHideAdmin) {
      return ;
   }
   if(m_bMiniMode){     
      this->setFixedHeight(5);
   }
   else {
      this->setFixedHeight(55);
   }

   emit SigForceHide(false);
   emit this->SigRepos(true);
   m_bEnterEventActive = true;
   setFocus();
}

void VedioPlayUI::StopAdmin() {
   if(!this->m_propertyAdmin) {
      return ;
   }
   m_bMiniMode = false;
   this->m_propertyAdmin->stop();
   m_LeaveTimer.stop();
}

void VedioPlayUI::StopAdminIMMediately(bool bForceHideAdmin) {
   m_bForceHideAdmin = bForceHideAdmin;
   if(!this->m_propertyAdmin) {
      return ;
   }
   if(m_LeaveTimer.isActive()) {
      m_LeaveTimer.stop();
      return ;
   }
}

void VedioPlayUI::FlushStatus() {
   m_bMiniMode = false;
   //m_LeaveTimer.stop();
   //if (m_propertyAdmin) {
   //   m_propertyAdmin->stop();
   //}
   //SlotAdminFinished();
}

void VedioPlayUI::SetMiniMode(bool bMini,int dulation) {
   if(m_bForceHideAdmin) {
      return ;
   }
   if(!m_pControlWidget) {
      return ;
   }
   
   m_bEnterEventActive=false;
   emit this->SigRepos(true);
   
   this->m_propertyAdmin->setDuration(dulation);
   QRect rect=this->rect();  
   QPoint pos=this->mapToGlobal(QPoint(rect.x(),rect.y()));

   if(bMini){
      this->setFixedHeight(55);
   }
   emit SigForceHide(true); 
   if(bMini){
		m_pPlayBtn->hide();
		m_pTimeLbl->hide();
		m_pAddFileBtn->hide();
		m_pVolumeBtn->hide();
		m_pVolumeSlider->hide();
		m_pFileListBtn->hide();
		m_pLoopSettingBtn->hide();
		m_pExitBtn->hide();
		this->setFixedHeight(5);
      m_propertyAdmin->setStartValue(QRect(pos.x(),pos.y(),rect.width(),rect.height()));
      m_propertyAdmin->setEndValue(QRect(pos.x(), pos.y()+50, rect.width(), rect.height()-50));
		qDebug() << "111111111111111111111111111:" << pos;
   }
   else {
		m_pPlayBtn->show();
		m_pTimeLbl->show();
		m_pAddFileBtn->show();
		//if (mLiveType != eLiveType_TcActive) {
		//	m_pVolumeSlider->show();
		//	m_pVolumeBtn->show();
		//}
		m_pFileListBtn->show();
		m_pLoopSettingBtn->show();
		m_pExitBtn->show();
		m_propertyAdmin->setStartValue(QRect(pos.x(), pos.y(), rect.width(), rect.height()));
		m_propertyAdmin->setEndValue(QRect(pos.x(), pos.y() - 50, rect.width(), rect.height() + 50));
		qDebug() << "2222222222222222222222222" << pos;
   }
   m_propertyAdmin->start();
}

bool VedioPlayUI::eventFilter(QObject *obj, QEvent *ev) {
   if (ev) {
      if (ev->type() == QEvent::MouseButtonPress) {
         //麦克风音量slider, 点击即到
         if (obj == m_pVolumeSlider  && m_pVolumeSlider->isEnabled()){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(ev);
            if (mouseEvent->button() == Qt::LeftButton)
            {
               int dur = m_pVolumeSlider->maximum() - m_pVolumeSlider->minimum();
               int pos = m_pVolumeSlider->minimum() + dur * ((double)(mouseEvent->x() - 5) / (m_pVolumeSlider->width() - 10));
               if (pos != m_pVolumeSlider->sliderPosition())
               {
                  m_pVolumeSlider->setValue(pos);
               }
            }
         }
      }
   }
   return false;
}

void VedioPlayUI::mousePressEvent(QMouseEvent * event)
{
	this->parentWidget()->raise();
	QWidget::mousePressEvent(event);
}

void VedioPlayUI::SetCaptureType(int type) {
   mLiveType = type;
   //if (mLiveType == eLiveType_TcActive) {
   //   m_pVolumeSlider->hide();
   //   m_pVolumeBtn->hide();
   //}
}

void VedioPlayUI::SetDeviceSystemVolume(int volume) {
   //if (mLiveType == eLiveType_Live) {
   //   STRU_MAINUI_VOLUME_CHANGE loVolChg;
   //   loVolChg.m_eType = change_VedioPlay;
   //   loVolChg.m_nVolume = volume;
   //   SingletonMainUIIns::Instance().PostCRMessage(MSG_MAINUI_VOLUME_CHANGE, &loVolChg, sizeof(STRU_MAINUI_VOLUME_CHANGE));
   //} else if (mLiveType == eLiveType_VhallActive) {
   //   VH::CComPtr<IMainUILogic> pMainUILogic;
   //   DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_IMainUI, IID_IMainUILogic, pMainUILogic, return, ASSERT(FALSE));
   //   pMainUILogic->SetPlayMediaFileVolume(volume, eLiveType_VhallActive);
   //}
}
