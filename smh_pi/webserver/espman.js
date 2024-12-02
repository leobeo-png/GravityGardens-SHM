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

	var accCallback;
	var sensorCallback;
	var statusUpdateCallback;
	var lightsCallback;

	var dataGetInterval;

	var lightsOn = true;
	var timeEnd;
	var lightsTimer;

	function gToRPM(g) {
		var womega = Math.sqrt(g * 9.81 / radiusMeter);
		var rpm = womega * (30 / Math.PI);
		console.log(rpm);
	}
	function checkAndExportLogs() {
		if(espsensordata.temperature != 0 && espsensordata.humidity != 0 && espsensordata.rpm != 0) {
			if(typeof(sensorCallback) === "function")
				sensorCallback(espsensordata.temperature, espsensordata.humidity);

			sqlman.writeExperimentLog(settings.experimentid, espsensordata.rpm, espsensordata.temperature, espsensordata.humidity, );

			espsensordata.temperature = 0;
			espsensordata.humidity = 0;
			espsensordata.rpm = 0;
		}
	}
	function getLightStatus() {
		return lightsOn();
	}
	function lightStatusFun() {
		if (lightsOn == false) {
			lightsOn = true;
			timeEnd = new Date((new Date()).getTime() + settings.lights_on_time * 60000);
			lightsTimer = setTimeout(lightStatusFun, settings.lights_on_time * 60000);
		} else if (lightsOn == true) {
			lightsOn = false;
			timeEnd = new Date((new Date()).getTime() + settings.lights_off_time * 60000);
			lightsTimer = setTimeout(lightStatusFun, settings.lights_off_time * 60000);
		}
		updateLights();
	}
	function updateLights() {
		// Set lights on/off
		if(!timeEnd) {
		// 	lightStatusFun();
			console.log("Error: Lights not started!");
			return;
		}
		console.log(`Lights are ${lightsOn == true ? "on" : "off"}`);
		if(typeof(lightsCallback) === "function") {
			lightsCallback(lightsOn, timeEnd.toString());
		}
	}
	async function getCurrentSettings() {
		return await sqlman.getExperimentSettings(currentExperimentid);
	}
	async function getSettingsList() {
		return await sqlman.getSettingsList();
	}
	async function changeSettings(experimentid) {
		currentExperimentid = experimentid;
		settings = await sqlman.getExperimentSettings(currentExperimentid);
	}
	async function editSettings(settingsdata) {
		settings = settingsdata;
		await writeCurrentSettings();
	}
	async function writeCurrentSettings() {
		await sqlman.setExperimentSettings(settings, currentExperimentid);
	}

	function statusReturn() {
		if(typeof(statusUpdateCallback) === "function") {
			statusUpdateCallback(status);
		}
	}
	function statusUpdate(newStatus) {
		status = newStatus;
		statusReturn();
	}
	async function start() {
		console.log("Starting...");
		statusUpdate("starting");

		dataGetInterval = setInterval(() => {
			serialman.send("GD \n");
		}, 5000);

		lightsOn = false;
		lightStatusFun();

		await serialman.send(`SL ${ gToRPM(settings.target_gravity) }\n`);
	}
	async function pause() {
		console.log("Pausing...");
		clearInterval(dataGetInterval);
		statusUpdate("pausing");
		await serialman.send("SL 0\n");
	}
	async function hardstop() {
		statusUpdate("FULL STOP!");
		console.log("Full stop!!!");
		await serialman.send("STOP \n");
	}

	var accelerometerTimingMillis = 0;
	function handleAccelData(accNum, xyz, value) {
		if(value != 0) console.log(accNum, xyz, value);
		if(typeof(accCallback === "function")) {
			accCallback(accNum, xyz, accelerometerTimingMillis, Number(value));
		}
	}

	module.exports.gToRPM = gToRPM;
	module.exports.handleSerialData = function(data) {
		try {
			var splitdata = data.split(" ");
			switch(splitdata[0]) {
				case "HU": // Humidity
					espsensordata.humidity = Number(splitdata[1]);
					checkAndExportLogs();
					break;
				case "TE": // Temperature
					espsensordata.temperature = Number(splitdata[1]);
					checkAndExportLogs();
					break;
				case "A1": // Accelerometer 1
					handleAccelData("1", splitdata[1], splitdata[2]);
					break;
				case "A2": // Accelerometer 2
					handleAccelData("2", splitdata[1], splitdata[2]);
					break;
				case "DO": // Door sensor
					pause();
					break;
				case "SP": // Accelerometer (in RPM)
					espsensordata.rpm = Number(splitdata[1]);
					checkAndExportLogs();

					if(espsensordata.rpm == 0) {
						statusUpdate("Stopped");
					}
					break;
				case "SG": // Signal light
					break;
				case "AT": // Accelerometer timing start
					accelerometerTimingMillis = Number(splitdata[1]);
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
					console.warn("Unknown serial command: ", data, splitdata);
					break;
			}
		} catch (e) {
			console.error(e);
			console.error("Incoming data on error: ", data);
		}
	}

	module.exports.getCurrentSettings = getCurrentSettings;
	module.exports.getSettingsList = getSettingsList;
	module.exports.changeSettings = changeSettings;
	module.exports.editSettings = editSettings;
	module.exports.statusReturn = statusReturn;
	module.exports.updateLights = updateLights;

	module.exports.start = start;
	module.exports.pause = pause;
	module.exports.hardstop = hardstop;

	module.exports.setAccelerometerCallback = function(accFun) { accCallback = accFun; }
	module.exports.setSensorCallback = function(cb) { sensorCallback = cb; }
	module.exports.setStatusUpdateCallback = function(cb) { statusUpdateCallback = cb; }
	module.exports.setLightsCallback = function(cb) { lightsCallback = cb; }
}());
