<?php
namespace Theapi\Nrf24\Websocket;

class Server extends AbstractServer 
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
          $this->send($user, $message);
        }
      }
      
      parent::loop();
    }
  }
  
  protected function process ($user, $message) 
  {
	// Send it through the socket.
    // todo: sanitation

    //$this->send($user, $message);
    $this->socket->write($message);
  }
  
  protected function connected ($user) {}
  
  protected function closed ($user) {}
}

