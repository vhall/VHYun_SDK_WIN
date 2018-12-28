#pragma once
#include "LiveEventInterface.h"
#include "paas_common.h"

//���յ��ķ����ڹ㲥����Ϣ֪ͨ
class VHRoomRecvInavMsgCallback {
public:
    /*
    *   �յ�����������Ϣ ,��ϢΪ�㲥��Ϣ���յ���Ϣ��ͨ�����뻥������ʱ��ȡ��Ȩ���б��ж��û��Ƿ������Ȩ��
    */
    virtual void OnRecvApplyInavPublishMsg(std::wstring& third_party_user_id) = 0;
    /*
    *   �յ����������Ϣ ,��ϢΪ�㲥��Ϣ���յ�ͬ�������ִ���������
    */
    virtual void OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish) = 0;
    /*
    *   ����������Ϣ  ��ϢΪ�㲥��Ϣ���յ���Ϣ����ʾ������Ϣ
    */
    virtual void OnRecvAskforInavPublishMsg(const std::wstring& third_party_user_id) = 0;
    /*
    *   �߳�����Ϣ  ��ϢΪ�㲥��Ϣ���յ���Ϣ��ִ���߳���
    */
    virtual void OnRecvKickInavStreamMsg(const std::wstring& third_party_user_id) = 0;
    /*
    *   �߳��������� , ��ϢΪ�㲥��Ϣ���յ���Ϣ���ж��ǵ�ǰ�û���ִ���߳��������
    */
    virtual void OnRecvKickInavMsg(const std::wstring& third_party_user_id) = 0;
    /*
    *   ��/��/�ܾ�������Ϣ ��ϢΪ�㲥��Ϣ
    */
    virtual void OnUserPublishCallback(const std::wstring& third_party_user_id, const std::string& stream_id, PushStreamEvent event) = 0;
    /*
    *   ��������ر���Ϣ  ���ܵ�����Ϣ�������ڻ����������Ա��ȫ�������˳�����
    */
    virtual void OnRecvInavCloseMsg() = 0;
    /*
    *   �û�������֪ͨ
    *   online�� true �û�����/ false �û�����
    *   user_id�� �û�id
    */
    virtual void OnUserOnLineState(bool online, const std::string user_id) = 0;
};

/*
**===================================
**
**   ����SDK�¼������ص��ӿڣ����нӿڻص�����SDKҵ���̡߳�
**   ����ص�����appҵ���¼��������ڻص��н��С�
**
**===================================
*/
class VHRoomMonitor :public VHRoomRecvInavMsgCallback, public WebRtcSDKEventInterface {
public:
    /*
    *   ��������������HTTPҵ��API���óɹ��¼�
    *   code: RoomEvent �¼�����  
    *   userData: ������json����
    */
    virtual void OnRoomSuccessedEvent(RoomEvent code, std::string userData = std::string()) = 0;
    /*
    *   ��������������HTTPҵ��API����ʧ���¼�
    *   respCode: ������
    *   msg��������Ϣ
    *   userData: ������json����
    */
    virtual void OnRoomFailedEvent(RoomEvent code, int respCode, const std::string& msg, std::string userData = std::string()) = 0;
    /*
    *   ��ȡ���������Ա�б�ص�
    **/
    virtual void OnGetVHRoomMembers(const std::string& third_party_user_id, std::list<VHRoomMember>&) = 0;
    /*
    *   ��ȡ���߳������������б�ص�
    **/
    virtual void OnGetVHRoomKickOutMembers(std::list<VHRoomMember>&) = 0;

};

