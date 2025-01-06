const socket = io();
const radius = 0.251;

var globalExperimentSettings;
var timerInterval;
var lightsOn;
var lightTimeEnd;
var highestAccelerometerTimeDecay;
var highestAccelerometerTime;
var secondHighestAccelerometerTime;

function gToRPM(g) {
	var womega = Math.sqrt(g * 9.81 / radius);
	var rpm = womega * (30 / Math.PI);
	return rpm;
}

function temperatureD(t, sensorId) {
	console.log(t);
	const x = (new Date()).getTime(),
		temperature = parseFloat(t); // Parse temperature value from response

	// Add point to the temperature series and limit to 1000 points
	if (chartTH.series[sensorId - 1].data.length > 100) {
		chartTH.series[sensorId - 1].addPoint([x, temperature], true, true, true); // redraw, shift, animation, with event
	} else {
		chartTH.series[sensorId - 1].addPoint([x, temperature], true, false, true);
	}
}

function humidityD(h, sensorId) {
	console.log(h);
	const x = (new Date()).getTime(),
		humidity = parseFloat(h); // Parse humidity value from response

	// Add point to the humidity series and limit to 500 points
	if (chartTH.series[sensorId + 1].data.length > 100) {
		chartTH.series[sensorId + 1].addPoint([x, humidity], true, true, true);
	} else {
		chartTH.series[sensorId + 1].addPoint([x, humidity], true, false, true);
	}
}
socket.on("sensorData", (temperature, humidity, rpm) => {
	temperatureD(temperature, 0);
	humidityD(humidityD, 0);
	rpmD(rpm);
});

function isRunning() {
	var endDate = new Date(globalExperimentSettings.experiment_start_dt.getTime() + globalExperimentSettings.experiment_length * 60000);
	// console.log(endDate);
	var timeLeft = new Date(endDate.getTime() - new Date().getTime());
	var running = true;

	if(timeLeft < 0) {
		running = false;
	}

	return { running: running, end: endDate, timeleft: timeLeft };
}

function updateGui() {
	var ir = isRunning();
	document.getElementById("time-remaining").innerHTML = `${Math.floor(ir.timeleft.getTime() / 1000)}`;
	document.getElementById("experiment-end").innerHTML = `${ir.end.toString()}`;
	// document.getElementById("status-text").innerHTML = `${ ir.running == true ? "Running" : "Stopped" }`;
	document.getElementById("light-status").innerHTML = `Lights are ${lightsOn == true ? "on" : "off"} for ${ Math.floor(((new Date(lightTimeEnd)).getTime() - (new Date()).getTime()) / 1000) } seconds`;

	progressBar.value = ir.timeleft.getTime() / 1000;

	if(ir.running == false) {
		clearInterval(timerInterval);
	}
}

function rpmD(rpm) {// Add point to the temperature series and limit to 1000 points
	console.log(rpm);
	const x = (new Date()).getTime();
	if (chartR.series[0].data.length > 100) {
		chartR.series[0].addPoint([x, rpm], true, true, true); // redraw, shift, animation, with event
	} else {
		chartR.series[0].addPoint([x, rpm], true, false, true);
	}
}

socket.on("experimentdataResponse", (res) => {
	console.log(res);
	globalExperimentSettings = res;
	globalExperimentSettings.experiment_start_dt = new Date(res.experiment_start_dt);

	document.getElementById("input1").value = res.experiment_name;
	document.getElementById("input2").value = res.target_gravity;
	document.getElementById("input3").value = res.experiment_length;
	document.getElementById("input4").value = res.lights_on_time;
	document.getElementById("input5").value = res.lights_off_time;
	document.getElementById("input6").value = res.experiment_description;

	document.getElementById("input1").dispatchEvent(new Event("input"));
	document.getElementById("input2").dispatchEvent(new Event("input"));
	document.getElementById("input3").dispatchEvent(new Event("input"));
	document.getElementById("input4").dispatchEvent(new Event("input"));
	document.getElementById("input5").dispatchEvent(new Event("input"));
	document.getElementById("input6").dispatchEvent(new Event("input"));

	document.getElementById("time-on").innerHTML = res.lights_on_time;
	document.getElementById("time-off").innerHTML = res.lights_off_time;

	progressBar.max = Number(res.experiment_length) * 60;
	timerInterval = setInterval(() => { updateGui() }, 100);

	const select = document.getElementById("dynamic-select");
	select.value = globalExperimentSettings.id;
});
socket.on("experimentList", (res) => {
	// console.log(res);
	const select = document.getElementById("dynamic-select");
	select.innerHTML = "";
	res.forEach(item => {
		const option = document.createElement("option");
		option.value = item.id;
		option.textContent = item.experiment_name;
		select.appendChild(option);
	});
});
socket.on("lights", (lightsOnIn, timeEnd) => {
	// console.log(timeStart);
	lightsOn = lightsOnIn;
	lightTimeEnd = timeEnd;
});

socket.on("accData", (accNum, xyz, accelerometerTimingMillis, value) => {
	// if(value != 0) console.log(`${accNum} ${xyz} ${accelerometerTimingMillis} ${value}`);
	const accel = parseFloat(value); 
	// return;
	
	let accelAxis;
	switch(xyz) {
		case "X":
		accelAxis = (accNum - 1) * 3; // index 0 and 3
		break;
		case "Y":
		accelAxis = (accNum - 1) * 3 + 1; // index 1 and 4
		break;
		case "Z":
		accelAxis = (accNum - 1) * 3 + 2; // index 2 and 5
		break;
		default:
		// how to error
		console.log('"Incorrect Axis given"');
		break;
	}
		
	if (chartA.series[accelAxis].data.length > 100) {
		chartA.series[accelAxis].addPoint([accelerometerTimingMillis, accel], true, true, true);
	} else {
		chartA.series[accelAxis].addPoint([accelerometerTimingMillis, accel], true, false, true);
	}

});
socket.on("status", (status) => {
	document.getElementById("status-text").innerHTML = `${ status }`;
})

function resetAccel () { // call this function for when the RPM changes
	chartA.series.forEach(series => { 
		series.setData([], false);
	});
	chartA.reDraw();
}
socket.on("rpm", (rpm) => {
	console.log(rpm);
	rpmD(rpm);
});

socket.emit("experimentdataRequest");

window.onload = () => {
	document.getElementById("input2").addEventListener('input', (e) => { // Calculate the RPM based on the G's
		console.log("Input");
		document.getElementById("rpm-calc").innerHTML = `RPM = ${Math.floor(gToRPM(e.target.value) * 10) / 10}`; // change this to the actual formula
	});

	//-------------------------------------------Select Dropdown-------------------------------------//
	const select = document.getElementById("dynamic-select");

	// Default position wiht nothing inside
	const defaultOption = document.createElement("option");
	defaultOption.value = "";
	defaultOption.textContent = "Select Previous Runs";
	defaultOption.selected = true;
	select.appendChild(defaultOption);


	select.addEventListener("change", (ev) => {
		console.log(`Change to ${ev.target.value}`);
		socket.emit("experimentchangeId", ev.target.value);
	});
}

