#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <QTime>

#define ID 0xFFFE
#define TYPE_REQ 10
#define TYPE_ANS 20


#pragma pack(push, 1)


enum Messages{

    GET_TIME   = 100,
    GET_SIZE   = 101,
    GET_STAT   = 102,
    SET_DATA   = 200,
    CLEAR_DATA = 201
};

enum StatusMessages{

    STATUS_SUCCES = 1,

    ERR_NO_FREE_SPACE = 10, // Недостаточно свободного места
    ERR_CONNECT_TO_HOST = 11,

    ERR_NO_FUNCT = 20   //Функционал не реализован



};


/*!
 * \brief Стрктура описывает служебный заголовок пакета
 */
struct ServiceHeader{

    uint16_t id = ID;     //Идентификатор начала пакета
    uint16_t idData = 0;  //Идентификатор типа данных
    uint8_t status = 0;   //Тип сообщения (запрос/ответ)
    uint32_t len = 0;     //Длина пакета далее, байт
};

struct StatServer{

    StatServer(){
            memset(this, 0, sizeof(*this));
    }

    uint32_t incBytes;  //принято байт
    uint32_t sendBytes; //передано байт
    uint32_t revPck;    //принято пакетов
    uint32_t sendPck;   //передано пакетов
    uint32_t workTime;  //Время работы сервера секунд
    uint32_t clients;   //Количество подключенных клиентов

    QString toString() {
            QString res;
            res += "Bytes received: " + QString::number(incBytes) + "\n";
            res += "Bytes sent: " + QString::number(sendBytes) + "\n";
            res += "Packets received: " + QString::number(revPck) + "\n";
            res += "Packets sent: " + QString::number(sendPck) + "\n";
            res += "Up time: " + QString::number(workTime) + " sec\n";
            res += "Number of connected clients: " + QString::number(clients) + "\n";
            return res;
    }
};


#pragma pack(pop)
#endif // STRUCTS_H
