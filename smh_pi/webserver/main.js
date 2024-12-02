const express = require("express");
const http = require("http");
const { Server } = require("socket.io");

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const bodyparser = require("body-parser");

const sqlman = require("./sqlman");
const serialman = require("./serialman");
sqlman.setup();

const espman = require("./espman");

const port = 8000;

function isEmptyOrSpaces(str){
    return str === null || str.match(/^ *$/) !== null;
}

app.use(bodyparser.urlencoded({extended: false}));
app.use(express.static("static"));

app.get("/", (req, res) => {
	res.send("Test");
});
app.get("/reset", async (req, res) => {
	await sqlman.resetDatabase();
	res.status(200).redirect("/");
});
app.get("/serialports", async (req, res) => {
	res.send(await serialman.list());
});
app.post("/start", async (req, res) => {
	// Convert info to sql database
	var dataObj = await espman.getCurrentSettings()

	dataObj.target_gravity = `${req.body.input2}`;
	dataObj.experiment_length = `${req.body.input3}`;
	dataObj.lights_on_time = `${req.body.input4}`;
	dataObj.lights_off_time = `${req.body.input5}`;
	dataObj.experiment_name = `${req.body.input1}`;
	dataObj.experiment_description = `${req.body.input6}`;
	dataObj.experiment_start_dt = new Date().toString();
	// console.log(dataObj);

	await espman.editSettings(dataObj);
	await espman.start();

	res.status(200).redirect("/");
});

io.on('connection', (socket) => {
	socket.on('msg', (d) => {
		console.log(`Msg: ${d}`);
	});
	socket.on('experimentdataRequest', async () => {
		// Get data from database
		socket.emit("experimentList", await espman.getSettingsList());
		socket.emit("experimentdataResponse", await espman.getCurrentSettings());
		espman.statusReturn();
		espman.updateLights();
	});
	socket.on('experimentchangeId', async (id) => {
		await espman.changeSettings(id);
		socket.emit("experimentdataResponse", await espman.getCurrentSettings());
	});

	socket.on("continueButton", async() => { await espman.start(); });
	socket.on("pauseButton", async() => { await espman.pause(); });
	socket.on("stopButton", async() => { await espman.hardstop(); });

	console.log("User connected");
});

// setInterval(() => {
// 	io.emit("temperature", 50, 1);
// 	io.emit("temperature", 60, 2);
// 	io.emit("humidity", 50, 1);
// 	io.emit("humidity", 60, 2);
// }, 1000);
var datactrobj = {
	1: {
		X: 0,
		Y: 0,
		Z: 0
	},
	2: {
		X: 0,
		Y: 0,
		Z: 0
	}
};
var dataprvobj = {
	1: {
		X: 0,
		Y: 0,
		Z: 0
	},
	2: {
		X: 0,
		Y: 0,
		Z: 0
	}
};
var dataspdobj = {
	1: {
		X: 0,
		Y: 0,
		Z: 0
	},
	2: {
		X: 0,
		Y: 0,
		Z: 0
	}
};
server.listen(port, async () => {
	console.log(`Running on http://localhost:${port}`);

	var sport = (await serialman.list())[0].path;
	console.log(`Starting serial on ${sport}`);
	serialman.setCallbacks(
		() => {
			console.log("Serial opened");
		},
		(data) => {
			// var sdatarec = data.toString();
			// console.log(`Serial: ${data}`);
			// console.log("Serial2: ", data.toString().split("\r\n"));
			espman.handleSerialData(data.toString());
		},
		() => { console.log("Serial closed"); }
	);
	serialman.start(sport, 115200);

	espman.setAccelerometerCallback((accNum, xyz, accelerometerTimingMillis, value) => {
		datactrobj[`${accNum}`][xyz]++;

		// dataprvobj[`${accNum}`][xyz] = accelerometerTimingMillis - dataprvobj[`${accNum}`][xyz];
		// dataspdobj[`${accNum}`][xyz] += (dataprvobj[`${accNum}`][xyz] * 0.001) * value;

		if(datactrobj[`${accNum}`][xyz] > 1) datactrobj[`${accNum}`][xyz] = 0;
		else return;
		if(accNum != 1) return;

		// io.sockets.emit("accData", accNum, xyz, accelerometerTimingMillis, dataspdobj[`${accNum}`][xyz]);
		io.sockets.emit("accData", accNum, xyz, accelerometerTimingMillis, value);
	});
	espman.setSensorCallback((temperature, humidity, rpm) => {
		io.sockets.emit("sensorData", temperature, humidity, rpm);
	});
	espman.setStatusUpdateCallback((status) => {
		io.sockets.emit("status", status);
	});
	espman.setLightsCallback((lightsOn, timeEnd) => {
		// console.log(lightsOn, timeEnd);
		io.sockets.emit("lights", lightsOn, timeEnd);
	});
	// setInterval(() => {
	// 	io.sockets.emit("accData", 1, 'x', 215, 0.01);
	// }, 5000);
});
