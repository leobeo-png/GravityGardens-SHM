(function() {
	const sp = require("serialport");
	var openedSerialport;

	module.exports.list = async function() {
		return await sp.SerialPort.list();
	}
	module.exports.start = async function(port, baudrate) {
		openedSerialport = new sp.SerialPort({
			path: port,
			baudRate: baudrate
		});
		let isOpen = await openedSerialport.open();
		if(isOpen) {
			console.log(isOpen);
		}
	}
	module.exports.stop = async function() {
		openedSerialport.close();
	}
}());
