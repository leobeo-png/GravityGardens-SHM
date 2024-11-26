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

	await espman.changeSettings(dataObj);

	res.status(200).redirect("/");
});

io.on('connection', (socket) => {
	socket.on('msg', (d) => {
		console.log(`Msg: ${d}`);
	});
	socket.on('experimentdataRequest', async () => {
		// Get data from database
		socket.emit("experimentdataResponse", await espman.getCurrentSettings());
	});
	socket.on('experimentchangeId', async (id) => {
		await espman.changeSettings(id);
		socket.emit("experimentdataResponse", await espman.getCurrentSettings());
	});

	console.log("User connected");
});

// setInterval(() => {
// 	io.emit("temperature", 50, 1);
// 	io.emit("temperature", 60, 2);
// 	io.emit("humidity", 50, 1);
// 	io.emit("humidity", 60, 2);
// }, 1000);

server.listen(port, async () => {
	console.log(`Running on http://localhost:${port}`);

	var sport = (await serialman.list())[0].path;
	console.log(`Starting serial on ${sport}`);
	serialman.setCallbacks(
		() => {
			console.log("Serial opened");
			serialman.send("69\n");
		},
		(data) => {
			var sdatarec = data.toString();
			// console.log(`Serial: ${sdatarec}`);

		},
		() => { console.log("Serial closed"); }
	);
	serialman.start(sport, 115200);
});
