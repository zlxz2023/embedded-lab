//单片机头文件
#include "system.h"

//网络设备
#include "esp8266.h"
#include "cJSON.h"
//协议文件
#include "onenet.h"
#include "mqttkit.h"

//硬件驱动
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "dht11.h"
#include "step_motor.h"
#include "HC_SR.h"
//C库
#include <string.h>
#include <stdio.h>


#define PROID		"77247"

#define AUTH_INFO	"test"

#define DEVID		"5616839"


extern unsigned char esp8266_buf[128];
int stop_flag = 0;
unsigned char *dataInf = NULL;

//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	UsartPrintf(USART_DEBUG, "OneNet_DevLink\r\n"
							"PROID: %s,	AUIF: %s,	DEVID:%s\r\n"
                        , PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//上传平台
		dataPtr = ESP8266_GetIPD(250);									//等待平台响应
        if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:UsartPrintf(USART_DEBUG, "Tips:	连接成功\r\n");status = 0;break;
					
					case 1:UsartPrintf(USART_DEBUG, "WARN:	连接失败：协议错误\r\n");break;
					case 2:UsartPrintf(USART_DEBUG, "WARN:	连接失败：非法的clientid\r\n");break;
					case 3:UsartPrintf(USART_DEBUG, "WARN:	连接失败：服务器失败\r\n");break;
					case 4:UsartPrintf(USART_DEBUG, "WARN:	连接失败：用户名或密码错误\r\n");break;
					case 5:UsartPrintf(USART_DEBUG, "WARN:	连接失败：非法链接(比如token非法)\r\n");break;
					
					default:UsartPrintf(USART_DEBUG, "ERR:	连接失败：未知错误\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//删包
	}
	else
		UsartPrintf(USART_DEBUG, "WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}

//==========================================================
//	函数名称：	OneNet_Subscribe
//
//	函数功能：	订阅
//
//	入口参数：	topics：订阅的topic
//				topic_cnt：topic个数
//
//	返回参数：	SEND_TYPE_OK-成功	SEND_TYPE_SUBSCRIBE-需要重发
//
//	说明：		
//==========================================================
void OneNet_Subscribe(const char *topics[], unsigned char topic_cnt)
{
	
	unsigned char i = 0;
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//协议包
	
	for(; i < topic_cnt; i++)
		UsartPrintf(USART_DEBUG, "Subscribe Topic: %s\r\n", topics[i]);
	
	if(MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL2, topics, topic_cnt, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);					//向平台发送订阅请求
		
		MQTT_DeleteBuffer(&mqttPacket);											//删包
	}

}

//==========================================================
//	函数名称：	OneNet_Publish
//
//	函数功能：	发布消息
//
//	入口参数：	topic：发布的主题
//				msg：消息内容
//
//	返回参数：	SEND_TYPE_OK-成功	SEND_TYPE_PUBLISH-需要重送
//
//	说明：		
//==========================================================
void OneNet_Publish(const char *topic, const char *msg)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//协议包
	
	UsartPrintf(USART_DEBUG, "Publish Topic: %s, Msg: %s\r\n", topic, msg);
	
	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL0, 0, 1, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);					//向平台发送订阅请求
		
		MQTT_DeleteBuffer(&mqttPacket);											//删包
	}

}

// 用来在步进电机循环时接收数据
void OneNet_RevInf(unsigned char *cmd){
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//协议包
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short topic_len = 0;
	unsigned short req_len = 0;
	
	unsigned char type = 0;
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	cJSON *json, *json_value;
	type = MQTT_UnPacketRecv(cmd);
	switch(type){	
		case MQTT_PKT_PUBLISH:														//接收的Publish消息
		
			result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
			if(result == 0){
				UsartPrintf(USART_DEBUG, "topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
																	cmdid_topic, topic_len, req_payload, req_len);
				// 对数据包req_payload进行JSON格式解析
                json = cJSON_Parse(req_payload);
                if(!json){
                    UsartPrintf(USART_DEBUG, "ERROR BEFORE: [%s]\n", cJSON_GetErrorPtr);
                }else{
                    json_value = cJSON_GetObjectItem(json, "target");
                    UsartPrintf(USART_DEBUG, "json_value: [%s]\n", json_value->string);
                    UsartPrintf(USART_DEBUG, "json_value: [%s]\n", json_value->valuestring);
                    if(strstr(json_value->valuestring, "curtain") != NULL){ // > 0且为整数
                        json_value = cJSON_GetObjectItem(json, "value");
                        if(json_value->valueint==0){
                            stop_flag = 1;
                        }
                    }
                }
                cJSON_Delete(json);
			}
            break;
		default:
            break;
	}
	
	ESP8266_Clear();									//清空缓存
	
	if(result == -1)
		return;
	
	dataPtr = strchr(req_payload, '}');					//搜索'}'

	if(dataPtr != NULL && result != -1)					//如果找到了
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
		{
			numBuf[num++] = *dataPtr++;
		}
		
		num = atoi((const char *)numBuf);				//转为数值形式
		
	}

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}


//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//协议包
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short topic_len = 0;
	unsigned short req_len = 0;
	
	unsigned char type = 0;
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	cJSON *json, *json_value;
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
		case MQTT_PKT_CMD:															//命令下发
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//解出topic和消息体
			if(result == 0)
			{
				UsartPrintf(USART_DEBUG, "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
                MQTT_DeleteBuffer(&mqttPacket);									//删包
			}
		
		break;
			
		case MQTT_PKT_PUBLISH:														//接收的Publish消息
		
			result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
			if(result == 0){
				UsartPrintf(USART_DEBUG, "topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
																	cmdid_topic, topic_len, req_payload, req_len);
				// 对数据包req_payload进行JSON格式解析
                json = cJSON_Parse(req_payload);
                if(!json){
                    UsartPrintf(USART_DEBUG, "ERROR BEFORE: [%s]\n", cJSON_GetErrorPtr);
                }else{
                    json_value = cJSON_GetObjectItem(json, "target");
                    UsartPrintf(USART_DEBUG, "json_value: [%s]\n", json_value->string);
                    UsartPrintf(USART_DEBUG, "json_value: [%s]\n", json_value->valuestring);
                    if(strstr(json_value->valuestring, "led") != NULL){ // > 0且为整数
                        json_value = cJSON_GetObjectItem(json, "value");
                        if(json_value->valueint==1){
                            LED2=0;
                            led_flag=1;
                        }else{
                            LED2=1;
                            led_flag=0;
                        }
                    }else if(strstr(json_value->valuestring, "beep") != NULL){
                        json_value = cJSON_GetObjectItem(json, "value");
                        if(json_value->valueint){
                            BEEP=1;
                        }else{
                            BEEP=0;
                        }
                    }else if(strstr(json_value->valuestring, "TempThreshold") != NULL){
                        json_value = cJSON_GetObjectItem(json, "value");
                        if (json_value != NULL){
                            temp_threshold = json_value->valueint;  // 将温度阈值赋值给 temp_threshold
                            if(check_temperature_humidity(temp, humi)){
                                BEEP=1;
                                LED1=0;
                            }else{
                                BEEP=0;
                                LED1=1;
                            }
                        }
                    }else if(strstr(json_value->valuestring, "HumThreshold") != NULL){
                        json_value = cJSON_GetObjectItem(json, "value");
                        if(json_value != NULL){
                            humi_threshold = json_value->valueint;  // 将湿度阈值赋值给 humi_threshold
                            if(check_temperature_humidity(temp, humi)){
                                BEEP=1;
                                LED1=0;
                            }else{
                                BEEP=0;
                                LED1=1;
                            }
                        }
                    }else if(strstr(json_value->valuestring, "curtain") != NULL){
                        json_value = cJSON_GetObjectItem(json, "value");
                        if (json_value != NULL) {
                            curtain = json_value->valueint;  // 将湿度阈值赋值给 humi_threshold
                            if(curtain == 1){
                                // 步进电机正转
                                int time=218;
                                while(time){
                                    Step_Motor_Run(0,1,1);
                                    time--;
                                    // 获取是否需要暂停的数据
                                    dataInf = ESP8266_GetIPD(3);
                                    if(dataInf != NULL)
                                        OneNet_RevInf(dataInf);
                                    if(stop_flag){
                                        break;
                                    }
                                }
                                stop_flag = 0;
                                Step_Motor_Run(0,1,0);
                            }else if(curtain == 0){
                                // 步进电机停止
                                Step_Motor_Run(0,1,0);
                            }else if(curtain == -1){
                                // 步进电机反转
                                int time=218;  // 5000ms / (8ms+15ms) = 217.39 = 218
                                while(time){
                                    Step_Motor_Run(1,1,1); // 8ms
                                    time--;
                                    // 获取是否需要暂停的数据
                                    dataInf = ESP8266_GetIPD(3); //15ms
                                    if(dataInf != NULL)
                                        OneNet_RevInf(dataInf);
                                    if(stop_flag){
                                        break;
                                    }
                                }
                                stop_flag = 0;
                                Step_Motor_Run(0,1,0);
                            }
                        }
                    }else if(strstr(json_value->valuestring, "exist_p") != NULL){
                        json_value = cJSON_GetObjectItem(json, "value");
                        if(json_value != NULL){
                            if(json_value->valueint){
                                exist_p = 1;
                            }else{
                                exist_p=0;
                            }
                        }
                    }
                }
                cJSON_Delete(json);
			}
            break;
			
		case MQTT_PKT_PUBACK:														//发送Publish消息，平台回复的Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				UsartPrintf(USART_DEBUG, "Tips:	MQTT Publish Send OK\r\n");
			
            break;
			
		case MQTT_PKT_PUBREC:														//发送Publish消息，平台回复的Rec，设备需回复Rel消息
		
			if(MQTT_UnPacketPublishRec(cmd) == 0)
			{
				UsartPrintf(USART_DEBUG, "Tips:	Rev PublishRec\r\n");
				if(MQTT_PacketPublishRel(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					UsartPrintf(USART_DEBUG, "Tips:	Send PublishRel\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
            break;
			
		case MQTT_PKT_PUBREL:														//收到Publish消息，设备回复Rec后，平台回复的Rel，设备需再回复Comp
			
			if(MQTT_UnPacketPublishRel(cmd, pkt_id) == 0)
			{
				UsartPrintf(USART_DEBUG, "Tips:	Rev PublishRel\r\n");
				if(MQTT_PacketPublishComp(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					UsartPrintf(USART_DEBUG, "Tips:	Send PublishComp\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
		
		break;
		
		case MQTT_PKT_PUBCOMP:														//发送Publish消息，平台返回Rec，设备回复Rel，平台再返回的Comp
		
			if(MQTT_UnPacketPublishComp(cmd) == 0)
			{
				UsartPrintf(USART_DEBUG, "Tips:	Rev PublishComp\r\n");
			}
		
		break;
			
		case MQTT_PKT_SUBACK:														//发送Subscribe消息的Ack
		
			if(MQTT_UnPacketSubscribe(cmd) == 0)
				UsartPrintf(USART_DEBUG, "Tips:	MQTT Subscribe OK\r\n");
			else
				UsartPrintf(USART_DEBUG, "Tips:	MQTT Subscribe Err\r\n");
		
		break;
			
		case MQTT_PKT_UNSUBACK:														//发送UnSubscribe消息的Ack
		
			if(MQTT_UnPacketUnSubscribe(cmd) == 0)
				UsartPrintf(USART_DEBUG, "Tips:	MQTT UnSubscribe OK\r\n");
			else
				UsartPrintf(USART_DEBUG, "Tips:	MQTT UnSubscribe Err\r\n");
		
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//清空缓存
	
	if(result == -1)
		return;
	
	dataPtr = strchr(req_payload, '}');					//搜索'}'

	if(dataPtr != NULL && result != -1)					//如果找到了
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
		{
			numBuf[num++] = *dataPtr++;
		}
		
		num = atoi((const char *)numBuf);				//转为数值形式
		
	}

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}
