#!/usr/bin/env php
<?php
//namespace Theapi\Nrf24\Websocket;

require 'WebSocket.php';
require 'Socket.php';

class nrf24Server extends WebSocketServer 
{

  protected $socket;
  
  public function __construct($host, $port, $socket)
  {
    parent::__construct($host, $port);
    $this->socket = $socket;
  }
  
  public function run()
  {
    while(true) {
      
      if ($message = $this->socket->loop()) {
        foreach ($this->users as $user) {
          $this->process($user, $message);
        }
      }
      
      parent::loop();
    }
  }
  
  protected function process ($user, $message) 
  {
    $this->send($user, $message);
  }
  
  protected function connected ($user) {}
  
  protected function closed ($user) {}
}

$nrf24 = new Socket('192.168.0.137', 2000);
$echo = new nrf24Server("localhost", "8080", $nrf24);


try {
  $echo->run();
}
catch (Exception $e) {
  $echo->stdout($e->getMessage());
}

