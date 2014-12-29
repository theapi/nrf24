#include "Nrf24Payload.h"


Nrf24Payload::Nrf24Payload()
{
  _payload.device_id = '-';
  _payload.type = '-';
  _payload.timestamp = 0;
  _payload.msg_id = 0;
  _payload.vcc = 0;
  _payload.a = 0;
  _payload.b = 0;
  _payload.c = 0;
  _payload.d = 0;
  _payload.e = 0;
}

uint8_t Nrf24Payload::getPayloadSize()
{
  return Nrf24Payload_SIZE;
}

uint8_t Nrf24Payload::getDeviceId()
{
  return _payload.device_id;
}

void Nrf24Payload::setDeviceId(uint8_t device_id)
{
  _payload.device_id = device_id;
}

// The id, not neccessarily unique, of the message.
uint16_t Nrf24Payload::getId()
{
  return _payload.msg_id;
}

void Nrf24Payload::setId(uint16_t id)
{
  _payload.msg_id = id;
}

// The type of message.
uint8_t Nrf24Payload::getType()
{
  return _payload.type;
}

void Nrf24Payload::setType(uint8_t type)
{
  _payload.type = type;
}

// The timestamp given to the message by the sender.
uint32_t Nrf24Payload::getTimestamp()
{
  return _payload.timestamp;
}

void Nrf24Payload::setTimestamp(uint32_t ts)
{
  _payload.timestamp = ts;
}

// The millivolts reported by the device in the message
uint16_t Nrf24Payload::getVcc()
{
  return _payload.vcc;
}

void Nrf24Payload::setVcc(uint16_t vcc)
{
  _payload.vcc = vcc;
}

// The first integer data
uint16_t Nrf24Payload::getA()
{
  return _payload.a;
}

void Nrf24Payload::setA(uint16_t val)
{
  _payload.a = val;
}

// The second integer data
uint16_t Nrf24Payload::getB()
{
  return _payload.b;
}

void Nrf24Payload::setB(uint16_t val)
{
  _payload.b = val;
}

// The third integer data
uint16_t Nrf24Payload::getC()
{
  return _payload.c;
}

void Nrf24Payload::setC(uint16_t val)
{
  _payload.c = val;
}

// The forth integer data
uint16_t Nrf24Payload::getD()
{
  return _payload.d;
}

void Nrf24Payload::setD(uint16_t val)
{
  _payload.d = val;
}

// The fith integer data
uint16_t Nrf24Payload::getE()
{
  return _payload.e;
}

void Nrf24Payload::setE(uint16_t val)
{
  _payload.e = val;
}

// Populates the given array with the payload data
void Nrf24Payload::serialize(uint8_t payload[Nrf24Payload_SIZE])
{
  payload[0] = _payload.device_id;
  payload[1] = _payload.type;
  payload[2] = (_payload.msg_id >> 8);
  payload[3] = _payload.msg_id;
  payload[4] = (_payload.vcc >> 8);
  payload[5] = _payload.vcc;
  payload[6] = (_payload.a >> 8);
  payload[7] = _payload.a;
  payload[8] = (_payload.b >> 8);
  payload[9] = _payload.b;
  payload[10] = (_payload.c >> 8);
  payload[11] = _payload.c;
  payload[12] = (_payload.d >> 8);
  payload[13] = _payload.d;
  payload[14] = (_payload.e >> 8);
  payload[15] = _payload.e;

  payload[16] = (_payload.timestamp >> 24);
  payload[17] = (_payload.timestamp >> 16);
  payload[18] = (_payload.timestamp >> 8);
  payload[19] = _payload.timestamp;
}

// Parse the read byte data from the radio
void Nrf24Payload::unserialize(uint8_t payload[Nrf24Payload_SIZE])
{
  _payload.device_id = payload[0];
  _payload.type = payload[1];
  _payload.msg_id = (payload[2] << 8) | payload[3];
  _payload.vcc = (payload[4] << 8) | payload[5];
  _payload.a = (payload[6] << 8) | payload[7];
  _payload.b = (payload[8] << 8) | payload[9];
  _payload.c = (payload[10] << 8) | payload[11];
  _payload.d = (payload[12] << 8) | payload[13];
  _payload.e = (payload[14] << 8) | payload[15];

  // Need to cast to unsigned long for the larger bit shift
  _payload.timestamp = ((uint32_t)payload[16] << 24) | ((uint32_t)payload[17] << 16) | ((uint32_t)payload[18] << 8) | (uint32_t)payload[19];

}

