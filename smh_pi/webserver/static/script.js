var chartTH = new Highcharts.Chart({
    chart: {
        renderTo: 'chart-combined'
    },
    title: {
        text: undefined
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: {second: '%H:%M:%S'}
    },
    yAxis: [{ // Primary yAxis for Temperature
        title: {
            text: 'Temperature (°C)'
        },
        labels: {
            format: '{value}°C'
        },
        opposite: false
    }, { // Secondary yAxis for Humidity
        title: {
            text: 'Humidity (%)'
        },
        labels: {
            format: '{value}%'
        },
        opposite: true
    }],
    series: [
        {
            name: 'Temperature #1',
            type: 'line',
            color: '#101D42',
            yAxis: 0, // Link to the first yAxis (Temperature)
            marker: {
                symbol: 'circle',
                radius: 3,
                fillColor: '#101D42',
            }
        },
        {
            name: 'Temperature #2',
            type: 'line',
            color: '#00A6A6',
            yAxis: 0, // Link to the first yAxis (Temperature)
            marker: {
                symbol: 'square',
                radius: 3,
                fillColor: '#00A6A6',
            }
        },
        {
            name: 'Humidity #1',
            type: 'line',
            color: '#4CAF50',
            yAxis: 1, // Link to the second yAxis (Humidity)
            marker: {
                symbol: 'circle',
                radius: 3,
                fillColor: '#4CAF50',
            }
        },
        {
            name: 'Humidity #2',
            type: 'line',
            color: '#FF9800',
            yAxis: 1, // Link to the second yAxis (Humidity)
            marker: {
                symbol: 'square',
                radius: 3,
                fillColor: '#FF9800',
            }
        }
    ],
    credits: {
        enabled: false
    }
});

// Function to fetch temperature data and update the chart
function fetchTemperatureData(sensorId) {
    setInterval(function () {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState === 4 && this.status === 200) {
                const x = (new Date()).getTime(),
                    temperature = parseFloat(this.responseText); // Parse temperature value from response

                // Add point to the temperature series and limit to 1000 points
                if (chartTH.series[sensorId - 1].data.length > 1000) {
                    chartTH.series[sensorId - 1].addPoint([x, temperature], true, true, true);
                } else {
                    chartTH.series[sensorId - 1].addPoint([x, temperature], true, false, true);
                }
            }
        };
        xhttp.open("GET", "/temperature" + sensorId, true); // Fetch temperature data for the given sensor
        xhttp.send();
    }, 10000); // 10-second interval for temperature updates
}

// Function to fetch humidity data and update the chart
function fetchHumidityData(sensorId) {
    setInterval(function () {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState === 4 && this.status === 200) {
                const x = (new Date()).getTime(),
                    humidity = parseFloat(this.responseText); // Parse humidity value from response

                // Add point to the humidity series and limit to 500 points
                if (chartTH.series[sensorId + 1].data.length > 500) {
                    chartTH.series[sensorId + 1].addPoint([x, humidity], true, true, true);
                } else {
                    chartTH.series[sensorId + 1].addPoint([x, humidity], true, false, true);
                }
            }
        };
        xhttp.open("GET", "/humidity" + sensorId, true); // Fetch humidity data for the given sensor
        xhttp.send();
    }, 10000); // 10-second interval for humidity updates
}


var chartA = new Highcharts.Chart({
    chart: {
        renderTo: 'chart-accel'
    },
    series: [
        {
            name: 'Accelerometer 1 - X',
            type: 'line',
            color: '#101D42',
            marker: {
                symbol: 'circle',
                radius: 3,
                fillColor: '#101D42',
            }
        },
        {
            name: 'Accelerometer 1 - Y',
            type: 'line',
            color: '#00A6A6',
            marker: {
                symbol: 'square',
                radius: 3,
                fillColor: '#00A6A6',
            }
        },
        {
            name: 'Accelerometer 1 - Z',
            type: 'line',
            color: '#8B2635',
            marker: {
                symbol: 'triangle',
                radius: 3,
                fillColor: '#8B2635',
            }
        },
        {
            name: 'Accelerometer 2 - X',
            type: 'line',
            color: '#71B48D',
            marker: {
                symbol: 'triangle-down',
                radius: 3,
                fillColor: '#71B48D',
            }
        },
        {
            name: 'Accelerometer 2 - Y',
            type: 'line',
            color: '#844494',
            marker: {
                symbol: 'diamond',
                radius: 3,
                fillColor: '#844494',
            }
        },
        {
            name: 'Accelerometer 2 - Z',
            type: 'line',
            color: '#9aa83b',
            marker: {
                symbol: 'cross',
                radius: 3,
                fillColor: '#9aa83b',
            }
        },
    ],
    title: {
        text: undefined
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: {second: '%H:%M:%S'}
    },
    yAxis: {
        title: {
            text: 'Acceleration'
        }
    },
    credits: {
        enabled: false
    }
});

function fetchAccelData(sensorId) {
    setInterval(function () {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState === 4 && this.status === 200) {
                const x = (new Date()).getTime(),
                    accel = parseFloat(this.responseText);  // Parse

                if (chartA.series[sensorId - 1].data.length > 1000) {
                    chartA.series[sensorId - 1].addPoint([x, accel], true, true, true);
                } else {
                    chartA.series[sensorId - 1].addPoint([x, accel], true, false, true);
                }
            }
        };
        xhttp.open("GET", "/accel" + sensorId, true); // Fetch data for the given sensor
        xhttp.send();
    }, 300); //300ms
}

var chartR = new Highcharts.Chart({
    chart: {
        renderTo: 'chart-rpm'
    },
    series: [
        {
            name: 'RPM',
            type: 'line',
            color: '#101D42',
            marker: {
                symbol: 'triangle',
                radius: 3,
                fillColor: '#101D42',
            }
        },
    ],
    title: {
        text: undefined
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: {second: '%H:%M:%S'}
    },
    yAxis: {
        title: {
            text: 'RPM Speed'
        }
    },
    credits: {
        enabled: false
    }
});

function fetchRPMData() {
    setInterval(function () {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState === 4 && this.status === 200) {
                const x = (new Date()).getTime(),
                    rpm = parseFloat(this.responseText);  // Parse

                if (chartR.series[0].data.length > 1000) {
                    chartR.series[0].addPoint([x, rpm], true, true, true);
                } else {
                    chartR.series[0].addPoint([x, rpm], true, false, true);
                }
            }
        };
        xhttp.open("GET", "/rpm", true); // Fetch data for the given sensor
        xhttp.send();
    }, 1000); // 1 second interval
}

for (let i = 1; i <= 2; i++) {
    // fetchTemperatureData(i);
    // fetchHumidityData(i);
    // fetchAccelData(i);
    // fetchRPMData();
}

//global variables
let timeRemaining;
let countdownInterval;

inputTime = document.getElementById("input3");
progressBar = document.getElementById("progressBar");
statusText = document.getElementById("status-text");
timeRemainingElement = document.getElementById("time-remaining");

/*
document.getElementById('settingsForm').addEventListener('submit', function (event) {
  if (confirm("Start the machine with the current settings?") == true) {
    //----------------------------Sending Inputs-------------------------------------
    event.preventDefault(); // Prevent the default form submission

    // Get form data
    const formData = new FormData(this);
    const params = new URLSearchParams(formData);

    // Send GET request to the server
    fetch(`/get?${params.toString()}`) // Fine for now, maybe chagne later to fit with the RPi
        .then(response => response.json())
        .then(data => {
          const statusMessage = document.getElementById('statusMessage');
          if (data.status === "success") {
            statusMessage.textContent = data.message;
            statusMessage.className = "success";
            // StartTimer(); // Start timer when the data is successfully sent to the Server
            /*const experimentNameT = document.getElementById("input1").value;
            const experimentNameElement = document.getElementById("experiment-name");
            experimentNameElement.textContent = experimentNameT;	*//*
          } else {
            statusMessage.textContent = data.message;
            statusMessage.className = "error";
          }
        })
        .catch(error => {
          const statusMessages = document.getElementById('statusMessage');
          statusMessage.textContent = "An error occurred. Please try again.";
          statusMessage.className = "error";
          //return; // Breaking the function
        });
    StartTimer(); // temporary start
    // another seperate timer for the lights
    const experimentNameT = document.getElementById("input1").value;
    const experimentNameElement = document.getElementById("experiment-name");
    experimentNameElement.textContent = experimentNameT;
  } else {
    event.preventDefault();
  }
}); */
// Change RPM calc based on input
// document.getElementById("input2").addEventListener('input', (e) => { // Calculate the RPM based on the G's
//     document.getElementById("rpm-calc").innerHTML = `RPM = ${gforceToRPM(e.target.value)}`; // change this to the actual formula
// });

function gforceToRPM(gForce) {
  const radius = 50; // Centimeters

  const constant = 1.118 * Math.pow(10, -5);

  const rpm = Math.sqrt(gForce / (constant * radius));

  return rpm.toFixed(2);
}

function StartTimer() { // for the whole machine time
    const inputTime = document.getElementById("input3").value; // To constanly update the Value
    const progressBar = document.getElementById("progressBar");
    const statusText = document.getElementById("status-text");
    const timeRemainingElement = document.getElementById("time-remaining");

    if (!inputTime || isNaN(inputTime) || inputTime <= 0) {
        alert("Please enter a valid time (in minutes).");
        return;
    }

    // Clear any existing countdown
    if (countdownInterval) clearInterval(countdownInterval);

    const totalTime = inputTime * 60;
    timeRemaining = totalTime; // sets the time remaining variable
    progressBar.max = totalTime; // initializes the progress bar
    progressBar.value = totalTime;
    statusText.textContent = "Running";

    // Start the countdown
    countdownInterval = setInterval(() => {
        if (timeRemaining <= 0) {
            clearInterval(countdownInterval);
            statusText.textContent = "Completed";
            timeRemainingElement.textContent = "00:00";
            alert("Time is up!");

            // send POST to the RPi to send serial command to ESP32
        } else {
            timeRemaining--;

            // Calculate hours, minutes, and seconds
            const hours = Math.floor(timeRemaining / 3600);
            const minutes = Math.floor((timeRemaining % 3600) / 60);
            const seconds = timeRemaining % 60;

            // Format the time based on whether hours are needed
            const formattedTime =
                hours > 0
                    ? `${hours.toString().padStart(2, '0')}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`
                    : `${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;

            // Update the display
            timeRemainingElement.textContent = formattedTime;
            progressBar.value = timeRemaining;
        }
    }, 1000);
}

//------------------------------------------Log Window---------------------------------------------------------//

const openModal = document.getElementById('view-log');
const modal = document.getElementById('modal');
const closeModal = document.querySelector('.close');
const experimentContent = document.getElementById('experiment-content');

// Just a prototype right now
// Change this contents to the database data?
const logExperiments = [
	{ name: "Experiment #1", G: 10, startTime: "10:00 AM", endTime: "8:00 PM", desc: "This experiment consists of many things that affects thing and makes things do other stuff and yeah and nah." },
	{ name: "Experiment #2", G: 13, startTime: "12:00 AM", endTime: "10:00 PM", desc: "This experiment consists of many things that affects thing and makes things do other stuff and yeah and nah." },
	{ name: "Experiment #3", G: 15, startTime: "11:00 AM", endTime: "9:00 PM",  desc: "This experiment consists of many things that affects thing and makes things do other stuff and yeah and nah." },
	{ name: "Experiment #4", G: 15, startTime: "11:00 AM", endTime: "9:00 PM",  desc: "This experiment consists of many things that affects thing and makes things do other stuff and yeah and nah." },
	{ name: "Experiment #5", G: 15, startTime: "11:00 AM", endTime: "9:00 PM",  desc: "This experiment consists of many things that affects thing and makes things do other stuff and yeah and nah." },
	{ name: "Experiment #6", G: 15, startTime: "11:00 AM", endTime: "9:00 PM",  desc: "This experiment consists of many things that affects thing and makes things do other stuff and yeah and nah." },
	{ name: "Experiment #7", G: 15, startTime: "11:00 AM", endTime: "9:00 PM",  desc: "This experiment consists of many things that affects thing and makes things do other stuff and yeah and nah." },
	{ name: "Experiment #8", G: 15, startTime: "11:00 AM", endTime: "9:00 PM",  desc: "This experiment consists of many things that affects thing and makes things do other stuff and yeah and nah." },
];

function populateGrid(data) {
	experimentContent.innerHTML = "";
	data.forEach((experiment) => { // data is converted to an object (experiment)
		experimentContent.innerHTML +=
		`
		<tr>
		<td>${experiment.name}</td>
		<td>${experiment.G}</td>
		<td>${experiment.startTime}</td>
		<td>${experiment.endTime}</td>
		<td>${experiment.desc}</td>
		<td>
			<button class="normal-button" onclick="download-xml">Download</button>
		</td>
		</tr>
		`; // download button linked to another function to download according to the experiment number?
	});
}

openModal.addEventListener('click', () => {
	populateGrid(logExperiments);
	modal.style.display = 'block';
});

closeModal.addEventListener('click', () => {
	modal.style.display = 'none';
});

window.addEventListener('click', (event) => {
	if (event.target === modal) {
		modal.style.display = 'none';
	}
});





