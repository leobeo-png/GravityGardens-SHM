(function() {
	const SerialPort = require("serialport");
	const { ReadlineParser } = require('@serialport/parser-readline');
	var openedSerialport;

	var startCallback;
	var receiveCallback;
	var stopCallback;

	module.exports.list = async function() {
		return await SerialPort.SerialPort.list();
	}
	module.exports.start = async function(port, baudrate) {
		openedSerialport = new SerialPort.SerialPort({
			path: port,
			baudRate: baudrate,
			parser: new ReadlineParser("\n")
		});
		openedSerialport.on("open", () => {
			if(typeof(startCallback) === "function") startCallback();
		});
		openedSerialport.on("data", (data) => {
			if(typeof(receiveCallback) === "function") receiveCallback(data);
		});
		openedSerialport.on("close", () => {
			if(typeof(closeCallback) === "function") closeCallback();
		});

		/* var p = new Promise((resolve) => {
			openedSerialport.open((err) => {
				if(err) {
					console.error(err);
				} else resolve();
			});
		});
		return p;*/
	}
	module.exports.setCallbacks = function(start, rcv, stop) {
		startCallback = start;
		receiveCallback = rcv;
		closeCallback = stop;
	}
	module.exports.send = function(data) {
		return new Promise((resolve, reject) => {
			openedSerialport.write(data, (err) => {
				if(err) {
					console.error("Error: ", err);
					reject(err);
				}
				// console.log(`Wrote ${data} to serial`);
				resolve();
			});
			// console.log(openedSerialport.write);
		});
	}
	module.exports.stop = async function() {
		openedSerialport.close();
	}
}());
