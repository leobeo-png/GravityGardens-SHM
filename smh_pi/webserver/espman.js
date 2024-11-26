(function() {
	serialman = require("./serialman");
	sqlman = require("./sqlman");

	var radiusMeter = 0.251;
	var currentExperimentid = 1;
	var settings = {};
	var espsensordata = {
		temperature: 0,
		humidity: 0,
		rpm: 0
	};
	var status = "";

	function gToRPM(g) {
		var womega = Math.sqrt(g * 9.81 / radiusMeter);
		var rpm = womega * (30 / Math.PI);
		console.log(rpm);
	}
	function checkAndExportLogs() {
		if(espsensordata.temperature != 0 && espsensordata.humidity != 0 && espsensordata.rpm != 0) {
			sqlman.writeExperimentLog(settings.experimentid, espsensordata.rpm, espsensordata.temperature, espsensordata.humidity, );
		}
	}
	async function getCurrentSettings() {
		return await sqlman.getExperimentSettings(1);
	}
	async function changeSettings(experimentid) {
		currentExperimentid = experimentid
		settings = await sqlman.getExperimentSettings(currentExperimentid);
	}
	async function editSettings(settingsdata) {
		settings = settingsdata;
		await writeCurrentSettings();
	}
	async function writeCurrentSettings() {
		await sqlman.setExperimentSettings(settings, currentExperimentid);
	}

	async function start() {
		console.log("Starting...");
		await serialman.send(`SL ${ gToRPM(settings.target_gravity) }`);
	}
	async function pause() {
		console.log("Pausing...");
		await serialman.send("SL 0");
	}
	async function hardstop() {
		console.log("Full stop!!!");
		await serialman.send("STOP ");
	}

	module.exports.gToRPM = gToRPM;
	module.exports.handleSerialData = function(data) {
		try {
			var splitdata = sdatarec.split(" ");
			switch(splitdata[0]) {
				case "HU": // Humidity
					sensordata.humidity = Number(splitdata[1]);
					break;
				case "TE": // Temperature
					sensordata.temperature = Number(splitdata[1]);
					break;
				case "A1": // Accelerometer 1
					break;
				case "A2": // Accelerometer 2
					break;
				case "DO": // Door sensor
					break;
				case "SP": // Accelerometer (in RPM)
					sensordata.rpm = Number(splitdata[1]);
					break;
				case "EST": // E-stop
					if(splitdata[1] == "0") {
						// Return back to normal operation
						start();
					} else if(splitdata[1] == "1") {
						// Immidiately stop as fast as possible
						hardstop();
					}
					break;
				default:
					console.warn("Unknown serial command")
					break;
			}
		} catch (e) {
			console.error(e);
		}
	}

	module.exports.getCurrentSettings = getCurrentSettings;
	module.exports.changeSettings = changeSettings;
	module.exports.editSettings = editSettings;

	module.exports.start = start;
	module.exports.pause = pause;
	module.exports.hardstop = hardstop;
}());
