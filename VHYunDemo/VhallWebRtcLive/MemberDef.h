#pragma once
typedef enum VhallInteractionRenderType_e {
    RenderType_Video = 0,//Ĭ�� ��Ƶ����������ͷ�����湲��
    RenderType_Wait,//�ȴ�
    RenderType_Finished,//����
    RenderType_MasterExit,//�������˳�
    RenderType_Screen,//���湲��
    RenderType_Photo,//��Ƭ
}VhallInteractionRenderType;

//�û��б�tabҳ
enum eTabMember
{
    eTabMember_onLine = 0,   //
    eTabMember_raiseHands,      //
    eTabMember_prohibitChat,      //
    eTabMember_kickOut,      //
    eTabMember_Count,      //
};

#define UPPERMIC QString::fromLocal8Bit("��������")
#define DOWNMIC QString::fromLocal8Bit("����")
#define UPPERINVITATION QString::fromLocal8Bit("������")
#define UPPERDOWNMICING QString::fromLocal8Bit("������")

#define CANCELPROHIBITCHAT QString::fromLocal8Bit("ȡ������")
#define PROHIBITCHAT QString::fromLocal8Bit("����")
#define CANCELKICKOUT QString::fromLocal8Bit("ȡ���߳�")
#define KICKOUT QString::fromLocal8Bit("�߳�")
const int PageSpanNum = 5;