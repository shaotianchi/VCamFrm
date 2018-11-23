/// By Fanxiushu 2016-04-27

#pragma once

#pragma pack(1)

///ISO����ʱ�򸽼ӵİ���Ϣ,��ʵ���Ǵ�IOCTL.H���Ƶ�iso_packet_t�Ƚṹ
struct iso_packet
{
	unsigned int offset;
	unsigned int length;
	unsigned int status;
};
struct iso_packet_hdr
{
	unsigned int start_frame;  ///��ʼ֡
	unsigned int flags;       //��Ҫ�����ж��Ƿ����� START_ISO_TRANSFER_ASAP
	unsigned int error_count;  ///���������
	unsigned int number_packets; // iSO ������
								 ////
	iso_packet packets[1]; ////
};
#define ISO_PACKET_HDR_SIZE    (4*sizeof(unsigned int))

///////////
struct usbtx_header_t
{
	int        type;  // 1 ��ȡ������, 2 vendor or class �� 3 ��������,  4 ����, 5 ��ȡ״̬, 6 ����feature
	int        result;
	int        data_length; ///��Ӧд��USB�豸�ĵĲ����������ɹ������ֵ����ʵ��д�����
	int        reserved; //����
	/////
	union{
		///
		struct{
			int          type;     // 1 ��ȡ�������豸�������� 2 ��������������, 3 ��ȡ�����ýӿ��������� 4 ��ȡ�����ö˿�������
			int          subtype;  // (type=1,3,4) 1 ��ȡ�豸�������� 2 ��ȡ������������ 3 ��ȡ�ַ���;;;;; (type=2) 1����config(index=-1 & value=-1 unconfigure)�� 2 ���� interface
			int          is_read;  // (type=1,3,4) is_readΪTRUE��ȡ��������FALSE ����������
			int          index;    // ���
			int          value;    // ֵ, ��ȡstringʱ�����language_id

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
			union {
				int               number_packets; //ͬ������ʱ�򣬰�����,���Ϊ0������ϵ�һ���䣬>0����ͷ�����iso_packet_hdr_t�ṹ����СΪ ISO_PACKET_HDR_SIZE + number_packets*sizeof(iso_packet_t)
				struct {
					unsigned char setup_packet[8]; /////���ƴ���ʱ�򣬷��͵�8���ֽڵĿ�����
				};
			};
			char          is_split; ///�ж��������䣬��ͬ�������Ƿ��ֳɶ�飬 
			char          reserved[3]; ///
		}transfer;
		////////
		struct {
			int           type; /// 1 IOCTL_INTERNAL_USB_RESET_PORT�����豸�� 2 IOCTL_INTERNAL_USB_CYCLE_PORT �����豸�� 3 ���ö˿�URB_FUNCTION_RESET_PIPE; 4 �ж϶˿� URB_FUNCTION_ABORT_PIPE
			int           ep_address;
		}reset;
		/////
		struct {
			int           type; /// 1 device; 2 interface ; 3 endpoint; 4 other status; 5 ��ȡ��ǰ������������6 ����interface��ȡ��ǰ�ӿڵ�alterantesetting
			int           index; ///
		}status;
		//////
		struct {
			int           type;     //// 1 SET���� 2 CLEAR����
			int           subtype;  /// 1 device; 2 interface ; 3 endpoint; 4 other
			int           index;    ///
			int           value;    ///
		}feature;
		////////
	};
	////////////

	/////
	char  data[0]; ////ռλ��
};

#pragma pack()

//// function
void* virt_usb_open();
void virt_usb_close(void* handle);

//������Ƴ�USB�豸
int virt_usb_plugin(void* handle, const char* hw_ids, const char* comp_ids);
int virt_usb_unplug(void* handle);

//��ȡUSB�豸����
usbtx_header_t* virt_usb_begin(void* handle);
int virt_usb_end(void* handle, usbtx_header_t* header);

