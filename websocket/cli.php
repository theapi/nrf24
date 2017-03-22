#!/usr/bin/env php
<?php
namespace Theapi\Nrf24\Websocket;

spl_autoload_register(function ($class) {
    include str_replace('\\', '/', $class) . '.php';
});

// Open a reqular socket to the nrf24 server
$nrf24 = new Socket('192.168.0.22', 2000);
// Start the websocket server
$websocket = new Server("192.168.0.22", "8080", $nrf24);

try {
  $websocket->run();
}
catch (Exception $e) {
  $echo->stdout($e->getMessage());
}

