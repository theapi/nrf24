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
		// Send it through the socket.
		// todo: sanitation
		$this->socket->write($message);
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

