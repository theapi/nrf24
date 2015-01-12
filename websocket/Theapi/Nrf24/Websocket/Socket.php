<?php
namespace Theapi\Nrf24\Websocket;

class Socket 
{
  protected $sock;
  protected $host;
  protected $port;


  public function __construct($host, $port)
  {
    $this->host = $host;
    $this->port = $port;
    
    $this->sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    
    if (!socket_connect($this->sock, $host, $port)) {
      throw new \Exception(socket_strerror(socket_last_error()));
    }

  }
  
  public function loop() 
  {
    $read = array($this->sock);
    $write = $except = null;
    $changed = socket_select($read, $write, $except, 0, 200000); // Do not block for long
    if ($changed > 0) {
      foreach ($read as $socket) {
        
        
        $buf = socket_read($this->sock, 2048); 
        if ($buf === '') {
          // disconnected. 
          throw new \Exception(socket_strerror(socket_last_error()));
        } 
        echo '> ' . $buf;
        return $buf;

      }
    }
    
    return false;
  }
  
  public function write($msg)
  {
	$len = strlen($msg);
	// todo check whole message was sent
    socket_write($this->sock, $msg, $len);
  }
   
}

