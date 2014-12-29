/*
  Nrf24Payload.h - Library for defining the payload datastructure for the NRF24 network.

  Created by Peter Clarke.
*/
#ifndef Nrf24Payload_h
#define Nrf24Payload_h

// Provides the correct headers for Arduino or Rapberry Pi
#include "RF24_config.h"

#define Nrf24Payload_SIZE 20

class Nrf24Payload
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
      uint16_t e;
      uint8_t type;
      uint8_t device_id;
    }
    payload_t;

    Nrf24Payload();

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

    // The fith integer data
    uint16_t getE();
    void setE(uint16_t val);

    // Creates a byte array for sending via the radio
    void serialize(uint8_t payload[Nrf24Payload_SIZE]);

    // Parse the read byte data from the radio
    void unserialize(uint8_t payload[Nrf24Payload_SIZE]);

  private:
    payload_t _payload;

};

#endif
