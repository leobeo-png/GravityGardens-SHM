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

const port = 8000;

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
	var dataObj = await sqlman.getExperimentSettings();

	dataObj.target_gravity = `${req.body.input2}`;
	dataObj.experiment_length = `${req.body.input3}`;
	dataObj.lights_on_time = `${req.body.input4}`;
	dataObj.lights_off_time = `${req.body.input5}`;
	dataObj.experiment_name = `${req.body.input1}`;
	dataObj.experiment_description = `${req.body.input6}`;
	dataObj.experiment_start_dt = new Date().toString();
	console.log(dataObj);
	await sqlman.setExperimentSettings(dataObj);



	res.status(200).redirect("/");

	// Connect to correct usb port
	// Start experiment

});

io.on('connection', (socket) => {
	socket.on('msg', (d) => {
		console.log(`Msg: ${d}`);
	});
	socket.on('experimentdataRequest', async () => {
		// Get data from database
		socket.emit("experimentdataResponse", await sqlman.getExperimentSettings());
	});

	console.log("User connected");
});

setInterval(() => {
	io.emit("temperature", 50, 1);
	io.emit("temperature", 60, 2);
	io.emit("humidity", 50, 1);
	io.emit("humidity", 60, 2);
}, 1000);

server.listen(port, () => {
	console.log(`Running on http://localhost:${port}`);
});
