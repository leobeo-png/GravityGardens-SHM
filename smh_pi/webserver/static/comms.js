const socket = io();

var globalExperimentSettings;
var timerInterval;

socket.on("temperature", (t, sensorId) => {
	const x = (new Date()).getTime(),
		temperature = parseFloat(t); // Parse temperature value from response

	// Add point to the temperature series and limit to 1000 points
	if (chartTH.series[sensorId - 1].data.length > 100) {
		chartTH.series[sensorId - 1].addPoint([x, temperature], true, true, true);
	} else {
		chartTH.series[sensorId - 1].addPoint([x, temperature], true, false, true);
	}
});

socket.on("humidity", (h, sensorId) => {
	const x = (new Date()).getTime(),
		humidity = parseFloat(h); // Parse humidity value from response

	// Add point to the humidity series and limit to 500 points
	if (chartTH.series[sensorId + 1].data.length > 100) {
		chartTH.series[sensorId + 1].addPoint([x, humidity], true, true, true);
	} else {
		chartTH.series[sensorId + 1].addPoint([x, humidity], true, false, true);
	}
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
	document.getElementById("status-text").innerHTML = `${ ir.running == true ? "Running" : "Stopped" }`;

	progressBar.value = ir.timeleft.getTime() / 1000;

	if(ir.running == false) {
		clearInterval(timerInterval);
	}
}

socket.on("rpm", (rpm) => {
	console.log(rpm);
});

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

	progressBar.max = Number(res.experiment_length) * 60;
	timerInterval = setInterval(() => { updateGui() }, 100);

});
socket.emit("experimentdataRequest");

