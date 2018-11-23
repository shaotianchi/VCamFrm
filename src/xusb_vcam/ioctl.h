/// By fanxiushu 2016-04-06 

#pragma once


// {ED3FB623-0C23-48B8-880A-9D1A07263FFD}
DEFINE_GUID(GUID_XUSB_VIRT_INTERFACE,
	0xed3fb623, 0xc23, 0x48b8, 0x88, 0xa, 0x9d, 0x1a, 0x7, 0x26, 0x3f, 0xfd);


#define IOCTL_PDO_ADD \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_PDO_REMOVE \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)


#define IOCTL_BEGIN_TRANS_USB_DATA \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x808, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_END_TRANS_USB_DATA \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x809, METHOD_NEITHER, FILE_ANY_ACCESS)

/////
#define HW_IDS_COUNT    260

struct ioctl_pdo_create_t
{
    //���ڻ�ȡURB ֪ͨ, ���� ULONGLONG ,����32��64λ����
	ULONGLONG     hSemaphore;
	ULONGLONG     hRemoveEvent; ///�豸�����Ƴ����������ֵ
	LONG          BusAddress; ///��ֻ�Ǹ�����ֵ��������������ֵ������û��ʹ�õ�ֵ,�����0�������

	/// SZ
	WCHAR device_id[HW_IDS_COUNT];
	///MULTI SZ ����
	WCHAR hardware_ids[HW_IDS_COUNT];
	///MULTI SZ
	WCHAR compatible_ids[HW_IDS_COUNT];
    	
};

#pragma pack(1)

///ISO����ʱ�򸽼ӵİ���Ϣ
struct iso_packet_t
{
	unsigned int offset;
	unsigned int length;
	unsigned int status; 
};
struct iso_packet_hdr_t
{
	unsigned int start_frame;  ///��ʼ֡
	unsigned int flags;       //��Ҫ�����ж��Ƿ����� START_ISO_TRANSFER_ASAP
	unsigned int error_count;  ///���������
	unsigned int number_packets; // iSO ������
	////
	iso_packet_t packets[1]; ////
};
#define ISO_PACKET_HDR_SIZE    (4*sizeof(unsigned int))

struct ioctl_usbtx_header_t
{
	ULONGLONG        inter_handle;        //  �ǵȴ�������ļ�IRP ָ��
	LONG             inter_seqno;         //  ÿ��IRP�����кţ���������������inter_handleһ��������֤�������Ψһ����֤  
	LONG             data_length;         //  ���ݵĳ���; ����Ƕ��豸�����ȡ���ֽ����� �����д���ݵ��豸��д��ǰ����Ҫд����ֽ�����д��ɹ���ʵ��д����ֽ���,ISO �������� iso_packet_hdr_t�ṹ��С
	LONG             result;              //  �����Ƿ�ɹ�
	LONG             reserved1;           //  ����
	////


	int        type;  // 1 ��ȡ������, 2 vendor or class �� 3 ��������,  4 ����, 5 ��ȡ״̬, 6 ����feature
	int        reserved[ 3 ]; //����
	/////
	union{
		///
		struct{
			int          type;     // 1 ��ȡ�������豸�������� 2 ��������������, 3 ��ȡ�����ýӿ��������� 4 ��ȡ�����ö˿�������, 5 ��ȡ΢����ϢURB_FUNCTION_GET_MS_FEATURE_DESCRIPTOR
			int          subtype;  // (type=1,3,4) 1 ��ȡ�豸�������� 2 ��ȡ������������ 3 ��ȡ�ַ���;;;;; (type=2) 1����config(index=-1 & value=-1 unconfigure)�� 2 ���� interface
			int          is_read;  // (type=1,3,4) is_readΪTRUE��ȡ��������FALSE ����������
			int          index;    // ���, (type=5 LOBYTE(index)=interfaceNumber, HIBYTE(index)=Recipient)
			int          value;    // ֵ, ��ȡstringʱ�����language_id, (type=5 Ϊ MS_FeatureDescriptorIndex)

		}descriptor;
		////////
		struct{
			int          type;    //1 CLASS���� 2 VENDOR����
			int          subtype; //1 device; 2 interface ; 3 endpoint; 4 other
			int          is_read; //�Ǵ��豸��������д���豸
			int          request;
			int          index;
			int          value;
		}vendor;
		////
		struct {
			int           type; // 1 ���ƴ���,  2 �жϻ��������䣬 3 ͬ������
			int           ep_address; //�˿�λ��   ��� (ep_address &0x80) ���Ƕ�������д;  ���ƴ���ʱ�����Ϊ0��ʾʹ��Ĭ�϶˿�
			int           is_read;    //�Ǵ��豸��������д���豸
			union{
				struct { //�жϣ�������ͬ������
					int           number_packets; //ͬ������ʱ�򣬰�����,���Ϊ0������ϵ�һ���䣬>0����ͷ�����iso_packet_hdr_t�ṹ����СΪ ISO_PACKET_HDR_SIZE + number_packets*sizeof(iso_packet_t)
					int           reserved0;      ////
					char          is_split;      ///�ж��������䣬��ͬ�������Ƿ��ֳɶ�飬 
					char          reserved[3];   ///
				};
				struct{ //���ƴ���
					unsigned char setup_packet[8]; /////���ƴ���ʱ�򣬷��͵�8���ֽڵĿ�����
					unsigned int  timeout;         /////URB_FUNCTION_CONTROL_TRANSFER_EX ��Ӧ�ĳ�ʱֵ����λ����,Ϊ0 ��ʾ��ʹ�ó�ʱ����ͬ��URB_FUNCTION_CONTROL_TRANSFER
				};
			};
			
		}transfer;
		////////
		struct {
			int           type; /// 1 IOCTL_INTERNAL_USB_RESET_PORT�����豸�� 2 IOCTL_INTERNAL_USB_CYCLE_PORT �����豸�� 3 ���ö˿�URB_FUNCTION_RESET_PIPE; 4 �ж϶˿� URB_FUNCTION_ABORT_PIPE
			int           ep_address;
		}reset;
		/////
		struct {
			int           type; /// 1 device; 2 interface ; 3 endpoint; 4 other status; 5 ��ȡ��ǰ������������6 ����interface��ȡ��ǰ�ӿڵ�alterantesetting; 7 ��ȡcurrent frame number
			int           index; ///
		}status;
		//////
		struct {
			int           type;     //// 1 SET���� 2 CLEAR����
			int           subtype;  ///  1 device; 2 interface ; 3 endpoint; 4 other
			int           index;    ///
			int           value;    ///
		}feature;
		////////
	};
	////////////

};

#pragma pack()


