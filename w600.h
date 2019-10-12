/*
 * seeed_w600.h
 *
 * Copyright (c) 2018 seeed technology inc.
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _SERIAL_STA_H
#define _SERIAL_STA_H


#include <Arduino.h>
#include "ATSerial.h"
#include "SoftwareSerial.h"


#define debug Serial
#define DEBUG_EN 1


enum WifiMode{
        STA = 0,
        AP = 2,
        STA_AP = 3,
    };

typedef enum
{
    OPEN,
    WEP64,
    WEP128,
    WPA_PSK_TKIP,
}Encry_mode;

typedef enum{
    FORMAT_HEX,
    FORMAT_ASCII,
}Key_format;

typedef enum
{
    DHCP,
    STATIC
}Get_ip_approach;

typedef enum
{
    TCP,
    UDP,
}NetProtocol;

typedef enum
{
    Client,
    Server,
}NetMode;

typedef enum
{
    ONE_SHOT,
    SOFT_AP,
    SOFT_AP_WEB,
}WifiConfig;

typedef enum
{
    DISCONNECT = -1,           //Indicate that wifi module does not connect in sta mode or does not configure ok in AP mode. 
    AP_MODE_SET_OK = 1,            //Indicate that wifi module runs in AP mode and configure ok! 
    STA_MODE_CONNECTED,        //Indicate that wifi module runs in STA mode and had connected a ap already.
    AP_STA_MODE_CONNECT_OK,    //Indicate that wifi module runs in STA&AP mode,and,AP configure ok & STA had connect to specifed ap.
}WifiStatus;

class AtWifi:public ATSerial
{
    public:
        AtWifi(){wifi_status_ = DISCONNECT;};
        inline void setWifiStatus(WifiStatus status){
            wifi_status_ = status;
        }
        inline WifiStatus getWifiStatus(void){
            return wifi_status_;
        }

        #if defined(SAMD21)
        void begin(HardwareSerial &uart,uint32_t baud = DEFAULT_BAUD);
        #else
        void begin(SoftwareSerial &uart,uint32_t baud = DEFAULT_BAUD);
        #endif

        const char* buffer() const {return _resp_buffer;}

        // Send an AT command. Returns true if response begins with +OK. 
        bool sendAT(const char* cmd);

        //Set wifi mode:1.station mode ,2.AP mode 3.station&AP mode.
        bool wifiSetMode(WifiMode mode);

        //Set target Ap ssid,Module work in STA or STA&AP mode.
        bool wifiStaSetTargetApSsid(const char* ssid);

        //Set target Ap password,Module work in STA or STA&AP mode.
        bool wifiStaSetTargetApPswd(const char* password);

        /** Set encryption mode
                 *  0 - OPEN
                 *  1 - WEP64
                 *  2 - WEP128
                 *  3 - WPA-PSK(TKIP)
                 *  4 - WPA-PSK(CCMP/AES)
                 *  5 - WPA2-PSK(TKIP)
                 *  6 - WPA2-PSK(CCMP/AES)
                 *  We use the 5 - WPA2-PSK(TKIP) hear;
                 * */
        bool wifiApSetEncry(int mode);

        /**Create a socket binding in STA mode.
                 * msg - The socket number that module return.
                 * pro - network protocol,TCP or UDP.
                 * mode - client or server.
                 * time_out - time out for client,when there is not a communication between server and client for a long time,abort connection.
                 * remote_port
                 * local_port
        * */
        int wifiCreateSocketSTA(NetProtocol pro,NetMode mode,const char* server,uint16_t remote_port,uint16_t local_port);

        /**Send msg from socket.
                 * socket - socket number.
                 * send - the string msg to send.
        * */
        bool wifiSocketSend(int32_t socket,const char* send);

        bool wifiSocketPrepareSend(int socket,int message_length);

        /**Recv msg from socket.
                 * scoket - socket number
                 * read_len - recv len.
        * */
        bool wifiSocketRead(int32_t socket,uint32_t read_len);

        bool httpPost(int socket, const __FlashStringHelper* post_url,const __FlashStringHelper* host,const __FlashStringHelper* user_agent, const __FlashStringHelper* content_type, const __FlashStringHelper* opt_headers, char* content);

        // AP mode: should ssid be broadcast? 
        bool broadcastApSsidSet(bool flag);


        // close socket
        bool wifiCloseSpecSocket(int32_t socket);

        //Set default socket for communication.
        bool wifiSetDefaultSocket(int32_t socket);

        /**Set config mode.
                 * 0 - oneshot
                 * 1 soft ap configuration
                 * 2 soft ap web configuration.
        * */
        bool setWifiConfigMode(WifiConfig mode);

        bool sendBinaryMsg(uint8_t *msg,uint32_t msg_len);
        bool recvData(uint8_t *recv_msg,uint32_t *len);
        bool setBaudrate(uint32_t baud);
        bool getSpecSocketInfo(int32_t socket);
        bool exit_Tc_mode();



-----------------------
bool AtWifi::sendAT(const char* cmd)
bool AtWifi::wifiSetMode(WifiMode mode)
bool AtWifi::wifiStaSetTargetApSsid(const char* ssid)
bool AtWifi::wifiStaSetTargetApPswd(const char* password)
/**Create a socket connection in STA mode.
 * msg - The socket number that module return.
 * pro - network protocol,TCP or UDP.
 * mode - client or server.
 * server - server ip addr.
 * remote_port
 * local_port
 * */
int AtWifi::wifiCreateSocketSTA(NetProtocol pro,NetMode mode,const char* server,uint16_t remote_port,uint16_t local_port)

/**Send msg from socket.
 * socket - socket number.
 * send - the string msg to send.
 * */
bool AtWifi::wifiSocketSend(int32_t socket,const char* send)
bool AtWifi::wifiSocketPrepareSend(int socket,int message_length)
void AtWifi::write_P(const __FlashStringHelper* data)
/***
 * each header must be terminated with \n
 * content must end with \n\n.
 ***/
bool AtWifi::httpPost(
    int socket, 
    const __FlashStringHelper* post_url,
    const __FlashStringHelper* host,
    const __FlashStringHelper* user_agent, 
    const __FlashStringHelper* content_type, 
    const __FlashStringHelper* opt_headers, 
    char* content)

/**Recv msg from socket.
 * msg - the msg recved from socket.
 * scoket - socket number
 * read_len - recv len.
 * */
bool AtWifi::wifiSocketRead(int32_t socket,uint32_t read_len)
bool AtWifi::setBaudrate(uint32_t baud)

bool AtWifi::getSpecSocketInfo(int32_t socket)

bool AtWifi::wifiCloseSpecSocket(int32_t socket)
{
    ATSerial::flush();
    sprintf_P(_cmd_buffer,PSTR("AT+SKCLS=%d"),socket);
    strcat(_cmd_buffer,AT_enter);
    return ATSerial::sendCmdAndCheckRsp(_cmd_buffer,RSP_OK,_resp_buffer);
}

bool AtWifi::wifiSetDefaultSocket(int32_t socket)
{
    ATSerial::flush();
    sprintf_P(_cmd_buffer,PSTR("AT+SKSDF=%d"),socket);
    strcat(_cmd_buffer,AT_enter);
    return ATSerial::sendCmdAndCheckRsp(_cmd_buffer,RSP_OK,_resp_buffer);
}

bool AtWifi::setWifiConfigMode(WifiConfig mode)
{
    ATSerial::flush();
    sprintf_P(_cmd_buffer,PSTR("AT+ONEMODE=!%d"),mode);
    strcat(_cmd_buffer,AT_enter);
    return ATSerial::sendCmdAndCheckRsp(_cmd_buffer,RSP_OK,_resp_buffer);
}

bool AtWifi::sendBinaryMsg(uint8_t *msg,uint32_t msg_len)
{
    ATSerial::flush();
    ATSerial::binWrite(msg,msg_len);
}

bool AtWifi::recvData(uint8_t *recv_msg,uint32_t *len)
{
    return ATSerial::waitForData(recv_msg,len); //prev had custom timeout 5000
}
---------------------



    private:
        WifiStatus wifi_status_;
        char _cmd_buffer[64];
        char _resp_buffer[UART_MAX_LEN + 1];
        void AtWifi::write_P(const __FlashStringHelper* data);

};

#endif