#include "Nrf24Protocol.h"


Nrf24Protocol::Nrf24Protocol(uint8_t device_id)
{
  _payload_out.device_id = device_id;
  _payload_out.type = '-';
  _payload_out.timestamp = 0;
  _payload_out.msg_id = 0;
  _payload_out.vcc = 0;
  _payload_out.a = 0;
  _payload_out.b = 0;
  _payload_out.c = 0;
  _payload_out.d = 0;
  _payload_out.y = 0;
  _payload_out.z = 0;

  _payload_in.device_id = '-';
  _payload_in.type = '-';
  _payload_in.timestamp = 0;
  _payload_in.msg_id = 0;
  _payload_in.vcc = 0;
  _payload_in.a = 0;
  _payload_in.b = 0;
  _payload_in.c = 0;
  _payload_in.d = 0;
  _payload_in.y = 0;
  _payload_in.z = 0;
}

uint8_t Nrf24Protocol::getPayloadSize()
{
  return 20;
}

uint8_t Nrf24Protocol::getDeviceId()
{
  return _payload_in.device_id;
}

void Nrf24Protocol::setDeviceId(uint8_t device_id)
{
  _payload_out.device_id = device_id;
}

// The id, not neccessarily unique, of the message.
uint16_t Nrf24Protocol::getId()
{
  return _payload_in.msg_id;
}

void Nrf24Protocol::setId(uint16_t id)
{
  _payload_out.msg_id = id;
}

// The type of message.
uint8_t Nrf24Protocol::getType()
{
  return _payload_in.type;
}

void Nrf24Protocol::setType(uint8_t type)
{
  _payload_out.type = type;
}

// The timestamp given to the message by the sender.
uint32_t Nrf24Protocol::getTimestamp()
{
  return _payload_in.timestamp;
}

void Nrf24Protocol::setTimestamp(uint32_t ts)
{
  _payload_out.timestamp = ts;
}

// The millivolts reported by the device in the message
uint16_t Nrf24Protocol::getVcc()
{
  return _payload_out.vcc;
}

void Nrf24Protocol::setVcc(uint16_t vcc)
{
  _payload_out.vcc = vcc;
}

// The first integer data
uint16_t Nrf24Protocol::getA()
{
  return _payload_in.a;
}

void Nrf24Protocol::setA(uint16_t val)
{
  _payload_out.a = val;
}

// The second integer data
uint16_t Nrf24Protocol::getB()
{
  return _payload_in.b;
}

void Nrf24Protocol::setB(uint16_t val)
{
  _payload_out.b = val;
}

// The third integer data
uint16_t Nrf24Protocol::getC()
{
  return _payload_in.c;
}

void Nrf24Protocol::setC(uint16_t val)
{
  _payload_out.d = val;
}

// The forth integer data
uint16_t Nrf24Protocol::getD()
{
  return _payload_in.d;
}

void Nrf24Protocol::setD(uint16_t val)
{
  _payload_out.d = val;
}

// The first char data
int8_t Nrf24Protocol::getY()
{
  return _payload_in.y;
}

void Nrf24Protocol::setY(int8_t val)
{
  _payload_out.y = val;
}

// The second char data
int8_t Nrf24Protocol::getZ()
{
  return _payload_in.z;
}

void Nrf24Protocol::setZ(int8_t val)
{
  _payload_out.z = val;
}

// Populates the given array with the payload data
void Nrf24Protocol::serialize(uint8_t payload[20])
{
  payload[0] = _payload_out.device_id;
  payload[1] = _payload_out.type;
  payload[2] = (_payload_out.msg_id >> 8);
  payload[3] = _payload_out.msg_id;
  payload[4] = (_payload_out.vcc >> 8);
  payload[5] = _payload_out.vcc;
  payload[6] = (_payload_out.a >> 8);
  payload[7] = _payload_out.a;
  payload[8] = (_payload_out.b >> 8);
  payload[9] = _payload_out.b;
  payload[10] = (_payload_out.c >> 8);
  payload[11] = _payload_out.c;
  payload[12] = (_payload_out.d >> 8);
  payload[13] = _payload_out.d;
  payload[14] = _payload_out.y; //@todo Signed
  payload[15] = _payload_out.z; //@todo Signed

  payload[16] = (_payload_out.timestamp >> 24);
  payload[17] = (_payload_out.timestamp >> 16);
  payload[18] = (_payload_out.timestamp >> 8);
  payload[19] = _payload_out.timestamp;
}

// Parse the read byte data from the radio
void Nrf24Protocol::unserialize(uint8_t payload[20])
{
  _payload_in.device_id = payload[0];
  _payload_in.type = payload[1];
  _payload_in.msg_id = (payload[2] << 8) | payload[3];
  _payload_in.vcc = (payload[4] << 8) | payload[5];
  _payload_in.a = (payload[6] << 8) | payload[7];
  _payload_in.b = (payload[8] << 8) | payload[9];
  _payload_in.c = (payload[10] << 8) | payload[11];
  _payload_in.d = (payload[12] << 8) | payload[13];
  _payload_in.y = payload[14]; //@todo Signed
  _payload_in.z = payload[15]; //@todo Signed

  // Need to cast to unsigned long for the larger bit shift
  _payload_in.timestamp = ((uint32_t)payload[16] << 24) | ((uint32_t)payload[17] << 16) | (payload[18] << 8) | payload[19];

}

