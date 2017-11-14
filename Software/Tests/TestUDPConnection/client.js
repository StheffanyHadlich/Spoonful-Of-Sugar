var PORT = 8888;
var HOST = '192.168.1.45';

var dgram = require('dgram');
var message = new Buffer('Veinho também pega ônibus,porra!');

var client = dgram.createSocket('udp4');
client.send(message, 0, message.length, PORT, HOST, function(err, bytes) {
    if (err) throw err;
    console.log('UDP message sent to ' + HOST +':'+ PORT);
    client.close();
});