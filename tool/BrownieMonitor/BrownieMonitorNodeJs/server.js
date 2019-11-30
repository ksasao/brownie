'use strict';

// logger
const Log4js = require('log4js');
Log4js.configure('log-config.json');
const systemLogger = Log4js.getLogger('system');
const detectedLogger = Log4js.getLogger('detected');
const errorLogger = Log4js.getLogger('error');

// file
const fs = require('fs');
const config = JSON.parse(fs.readFileSync('./config.json', 'utf8'));
const rules = config.rules;
const serverPort = config.serverPort;
const comPort = config.comPort;

// serial port settings
systemLogger.info(
    'Port: ' + serverPort.toString() + ', '
    + 'COM Port: ' + comPort);
const SerialPort = require('serialport');
const parsers = SerialPort.parsers;
const parser = new parsers.Readline({
    delimiter: '\r\n'
});
const port = new SerialPort(comPort, {
    baudRate: 115200,
    parity: 'none',
    dataBits: 8,
    stopBits: 1,
    rtscts: true,
});
port.pipe(parser);

// socket.io server setup
const app = require('http').createServer(function (req, res) {
    res.writeHead(200, { 'Content-Type': 'text/html' });
    res.end(fs.readFileSync('index.html'));
}).listen(serverPort);
const io = require('socket.io').listen(app);

port.on('open', () => {
    systemLogger.info('Open ' + comPort);
});
parser.on('data', (data) => {
    console.log(data.toString());
    io.sockets.emit('raw', data.toString());
    const detected = '[DETECTED]: ';
    if (data.startsWith(detected)) {
        let message = data.toString().substr(detected.length).trim();
        detectedLogger.info(message);

        // execute command
        rules.some((rule) => {
            if (rule.if.toString() === message) {
                const command = rule.then.toString();

                // shell execution 
                const exec = require('child_process').exec;
                exec(command, (err, stdout, stderr) => {
                    if (err) {
                        errorLogger.error(err);
                        console.log(err);
                    }
                    systemLogger.debug(stdout);
                    console.log(stdout);
                });
                message = message + " => " + command;
                return true;
            }
        });
        io.sockets.emit('detected', message);
    }
});