//not Working----------------------------------------------------------------
const coap = require('coap');
const server = coap.createServer();

// Define the LED resource
server.on('request', (req, res) => {
  const payload = req.payload.toString();
  if (payload === '0') {
    // Turn off the LED
    console.log('LED turned off');
    res.end('0');
  } else if (payload === '1') {
    // Turn on the LED
    console.log('LED turned on');
    res.end('1');
  } else {
    // Get the LED status
    const ledStatus = '1'; // Assuming the LED is on
    res.end(ledStatus);
  }
});

server.listen(() => {
  console.log('CoAP server is listening');
});