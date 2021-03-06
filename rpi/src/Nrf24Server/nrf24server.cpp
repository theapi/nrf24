/**
 *
 * A socket server that listens for mumerical commands
 * and forwards them to listening nRF24L01+ nodes.
 *
 * If it has been possible to write to the radio receiver
 * the return code will be 200.
 *
 * If it has not been possible to write to the radio receiver
 * the return code will be 504.
 *
*/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <strings.h>
#include <sys/time.h>

/**
 * Adapted from
 * https://github.com/TMRh20/RF24/blob/master/examples_RPi/gettingstarted.cpp
 */
// RF24
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <RF24/RF24.h>

// The payload class
#include <NRF24/Nrf24Payload.h>


#define MAX_SOCKET_BYTES 255

using namespace std;
//
// Hardware configuration
//

// CE Pin, CSN Pin, SPI Speed

// Setup for GPIO 22 CE and CE1 CSN with SPI Speed @ 1Mhz
//RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_26, BCM2835_SPI_SPEED_1MHZ);

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 4Mhz
//RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ);

// NEW: Setup for RPi B+
//RF24 radio(RPI_BPLUS_GPIO_J8_15,RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ);

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);


// Addresses of potential listening nodes
const uint8_t radio_clients[][6] = {"AAAAA", "BBBBB", "CCCCC", "DDDDD", "EEEEE", "FFFFF", "HHHHH"};
// Default to 4 listening nodes.
uint8_t num_clients = 4;
uint8_t num_clients_max = 7;
// Radio pipes
const uint8_t pipes[][6] = {"1BASE", "2BASE", "3BASE", "4BASE", "5BASE"};

uint8_t test_mode = 0;
uint16_t msg_id = 0;

int master_socket;
fd_set active_fd_set;

void error(const char *msg)
{
  perror(msg);
  //exit(1);
}

/**
 * Send the response and check for failure.
 */
void respond(int sock, const char *msg)
{
  if (sock < 1) {
    // Pretent it was sent
    return;
  }
  int n = write(sock, msg, MAX_SOCKET_BYTES);
  if (n < 0) {
    perror("Oops, ERROR writing to socket");
    return;
  }
  return;
}

int makeSocket(uint16_t port)
{
  int sock;
  struct sockaddr_in name;

  /* Create the socket. */
  sock = socket (PF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror ("socket");
    exit (EXIT_FAILURE);
  }

  /* Give the socket a name. */
  name.sin_family = AF_INET;
  name.sin_port = htons (port);
  name.sin_addr.s_addr = htonl (INADDR_ANY);
  if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0) {
    perror ("bind");
    exit (EXIT_FAILURE);
  }

  return sock;
}

/**
 * Send it to the clients via the nRF24L01+.
 */
int sendPayloadToRadios(Nrf24Payload payload, int sock)
{
  printf ("Sending: %c %c %d %d %d %d %d %d \n",
    payload.getDeviceId(),
    payload.getType(),
    payload.getId(),
    payload.getVcc(),
    payload.getA(),
    payload.getB(),
    payload.getC(),
    payload.getD());


  for (int i = 0; i < num_clients; i++) {
    // Send message to the node on the pipe address
    radio.stopListening();
    radio.openWritingPipe(radio_clients[i]);

    printf(" %s ", radio_clients[i]);
    char respond_msg[MAX_SOCKET_BYTES];
    bzero(respond_msg, MAX_SOCKET_BYTES);

    // NB: seems to be that it does not send the same message twice in a row
    // BUT radio.write still returns true AND the ack payload is the same as last time.

    uint8_t tx_buffer[Nrf24Payload_SIZE];
    payload.serialize(tx_buffer);
    bool ok = radio.write( &tx_buffer, Nrf24Payload_SIZE);
    if (!ok) {
      sprintf (respond_msg, ".,%d,%s\n", 504, radio_clients[i]);
      //respond(sock, respond_msg);
      printf("(0)");
    } else {
      sprintf (respond_msg, ".,%d,%s\n", 200, radio_clients[i]);
      //respond(sock, respond_msg);
      printf("(1)");
    }

    // Continue listening
    radio.startListening();

  }
  printf("\n");

  return 0;
}

/**
 * Sent it to all connected sockets
 */
void sendPayloadToSockets(Nrf24Payload payload)
{

printf ("Sending to socket:,%c,%c,%d,%d,%d,%d,%d,%d\n",
    payload.getDeviceId(),
    payload.getType(),
    payload.getId(),
    payload.getVcc(),
    payload.getA(),
    payload.getB(),
    payload.getC(),
    payload.getD());

  char buf[MAX_SOCKET_BYTES];
  bzero(buf, MAX_SOCKET_BYTES);
  sprintf (buf, ":,%c,%c,%d,%d,%d,%d,%d,%d\n",
    payload.getDeviceId(),
    payload.getType(),
    payload.getId(),
    payload.getVcc(),
    payload.getA(),
    payload.getB(),
    payload.getC(),
    payload.getD());

  for (int i = 0; i < FD_SETSIZE; ++i) {
    if (FD_ISSET (i, &active_fd_set) && i != master_socket) {
printf ("socket:%d\n",i);
      int n = write(i, buf, MAX_SOCKET_BYTES);
      if (n < 0) {
        perror("sendPayloadToSockets ERROR writing to socket");
      }
    }
  }
}

Nrf24Payload parseSocketInput(char buf[MAX_SOCKET_BYTES])
{
  Nrf24Payload payload = Nrf24Payload();
  // Expects a csv string
  // eg; p,t,0,0,23,32767,32768,65535
  payload.setDeviceId(buf[0]);
  payload.setType(buf[2]);

  int i = 0;
  char *token;
  token = strtok(buf, ",");
  while (token != NULL) {
    //printf("%i: %s\n", i, token);
    switch (i) {
      case 2:
        payload.setId(atoi(token));
        break;
      case 3:
        payload.setVcc(atoi(token));
        break;
      case 4:
        payload.setA(atoi(token));
        break;
      case 5:
        payload.setB(atoi(token));
        break;
      case 6:
        payload.setC(atoi(token));
        break;
      case 7:
        payload.setD(atoi(token));
        break;
    }
    i++;
    token = strtok(NULL, ",");
  }

  return payload;
}

int readSocket(int sock)
{
  char buffer[MAX_SOCKET_BYTES];
  int nbytes;

  bzero(buffer, MAX_SOCKET_BYTES);
  nbytes = read (sock, buffer, MAX_SOCKET_BYTES);
  if (nbytes < 0) {
      // Read error.
      perror ("read");
      return -1;
  } else if (nbytes == 0) {
    // End-of-file.
    return -1;
  } else {
    // Send "exit" to close the connection
    char *pos = strstr(buffer, "exit");
    if (pos - buffer == 0) {
      printf("Got: exit\n");
      return -1;
    }

    Nrf24Payload payload = Nrf24Payload();
    if (test_mode) {

      // Pretty much hardcoded payload except for atoi(buffer)
      payload.setDeviceId('P'); // P for Pi
      payload.setType('T'); // test mode
      payload.setId(msg_id++);
      payload.setA(atoi(buffer)); // Just create an int from whatever came in
      payload.setB(255);
      payload.setC(32768);
      payload.setD(65535); // Max size of uint16_t
/*
      // Tmp test
      uint8_t buff[Nrf24Payload_SIZE];
      payload.serialize(buff);
      Nrf24Payload b_payload = Nrf24Payload();
      b_payload.unserialize(buff);

      // Dump it to screen
      printf("  test:%d %d %d %d\n", b_payload.getA(), b_payload.getB(), b_payload.getC(), b_payload.getD());
*/

    } else {
      // The socket should be providing payload data.
      payload = parseSocketInput(buffer);
    }

    sendPayloadToSockets(payload);
    // Pass the message to the radios.
    return sendPayloadToRadios(payload, sock);
  }
}


int main(int argc, char *argv[])
{
  uint16_t port;
  extern int makeSocket(uint16_t port);

  fd_set read_fd_set;
  int i;
  struct sockaddr_in clientname;
  size_t size;
  struct timeval tv;

  if (argc < 2) {
     fprintf(stderr,"ERROR, no port provided\n");
     exit(1);
  }
  port = atoi(argv[1]);

  // Set the number of clients if given.
  // eg; sudo ./nrf24server 2000 2 (for 2 clients on port 2000)
  if (argc > 2) {
    num_clients = atoi(argv[2]);
    if (num_clients > num_clients_max) {
      num_clients = num_clients_max;
    }
  }

  // Payload test mode.
  // eg; sudo ./nrf24server 2000 4 1
  if (argc > 3) {
    test_mode = atoi(argv[3]);
  }

  // Create the socket and set it up to accept connections.
  master_socket = makeSocket(port);
  if (listen (master_socket, 1) < 0) {
    perror ("listen");
    exit (EXIT_FAILURE);
  }

  // Initialize the set of active sockets.
  FD_ZERO (&active_fd_set);
  FD_SET (master_socket, &active_fd_set);

  // Allow select to block for 200000 us
  tv.tv_sec = 0;
  tv.tv_usec = 200000;

  printf("\nListening for commands on %d...\n", port);


/***** RF24 *********/

  // Setup and configure rf radio
  radio.begin();
  // 2 byte payload
  radio.setPayloadSize(Nrf24Payload_SIZE);
  // Ensure autoACK is enabled
  radio.setAutoAck(1);
  // Try a few times to get the message through
  radio.setRetries(0,15);

  // Listen to the pipes
  for (int i = 0; i < 5; i++) {
    radio.openReadingPipe(i+1, pipes[i]);
  }
  radio.startListening();

  // Dump the configuration of the rf unit for debugging
  radio.printDetails();

  printf("Size of payload = %d\n", radio.getPayloadSize());

/**** end rf24 ***/


  while (1) {
    // Listen for input on one or more active sockets.
    // For a max time of tv.tv_usec
    read_fd_set = active_fd_set;
    if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, &tv) < 0) {
      perror ("select");
      //exit (EXIT_FAILURE);
    }

    // Service all the sockets with input pending
    for (i = 0; i < FD_SETSIZE; ++i) {
      if (FD_ISSET (i, &read_fd_set)) {
        if (i == master_socket) {
          // Connection request on original socket.
          int new_client;
          size = sizeof (clientname);
          new_client = accept (master_socket,
                        (struct sockaddr *) &clientname,
                        &size);
          if (new_client < 0) {
            perror ("accept");
          }
          printf("Server: connect from host %s, port %hd.\n",
                inet_ntoa (clientname.sin_addr),
                ntohs (clientname.sin_port));
          FD_SET (new_client, &active_fd_set);
        } else {
          // Data arriving on an already-connected socket.
          if (readSocket(i) < 0) {
            printf("\nClose socket: %d...\n", i);
            close (i);
            FD_CLR (i, &active_fd_set);
          }
        }
      }
    }

    // Handle any messages from the radio
    while (radio.available()) {
      // Create a byte array to hold the raw bytes from the radio.
      uint8_t rx[Nrf24Payload_SIZE];
      // Read from the radio into the byte array.
      radio.read(&rx, Nrf24Payload_SIZE);
      // Create a payload object to handle the radio data.
      Nrf24Payload rx_payload = Nrf24Payload();
      // Convert the raw byte array into useful data.
      rx_payload.unserialize(rx);

      // Dump it to screen
      printf("payload:%c %c %d\n", rx_payload.getDeviceId(), rx_payload.getType(), rx_payload.getId());

      // Tell all who care
      sendPayloadToRadios(rx_payload, 0);
      sendPayloadToSockets(rx_payload);

    }

  }

}


