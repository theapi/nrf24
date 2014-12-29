/*
  Protocol.h - Library for the communication protocol being used for the nrf24 network.

  Created by Peter Clarke.
*/
#ifndef Nrf24Protocol_h
#define Nrf24Protocol_h

// Provides the correct headers for Arduino or Rapberry Pi
#include "RF24_config.h"

class Nrf24Protocol
{
  public:
    typedef struct{
      uint32_t timestamp;
      uint16_t msg_id;
      uint16_t vcc;
      uint16_t a;
      uint16_t b;
      uint16_t c;
      uint16_t d;
      uint8_t type;
      uint8_t device_id;
      int8_t y;
      int8_t z;
    }
    payload_t;

    Nrf24Protocol(uint8_t device_id);

    // How big the radion payload is.
    uint8_t getPayloadSize();

    // The id of the device.
    uint8_t getDeviceId();
    void setDeviceId(uint8_t device_id);

    // The id, not neccessarily unique, of the message.
    uint16_t getId();
    void setId(uint16_t id);

    // The type of message.
    uint8_t getType();
    void setType(uint8_t type);

    // The timestamp given to the message by the sender.
    uint32_t getTimestamp();
    void setTimestamp(uint32_t ts);

    // The millivolts reported by the device in the message
    uint16_t getVcc();
    void setVcc(uint16_t vcc);

    // The first integer data
    uint16_t getA();
    void setA(uint16_t val);

    // The second integer data
    uint16_t getB();
    void setB(uint16_t val);

    // The third integer data
    uint16_t getC();
    void setC(uint16_t val);

    // The forth integer data
    uint16_t getD();
    void setD(uint16_t val);

    // The first char data
    int8_t getY();
    void setY(int8_t val);

    // The second char data
    int8_t getZ();
    void setZ(int8_t val);

    // Creates a byte array for sending via the radio
    void serialize(uint8_t payload[20]);

    // Parse the read byte data from the radio
    void unserialize(uint8_t payload[20]);

  private:
    payload_t _payload_out;
    payload_t _payload_in;

};

#endif
