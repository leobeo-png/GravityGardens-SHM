const express = require("express");
const http = require("http");
const { Server } = require("socket.io");

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const bodyparser = require("body-parser");

const sqlman = require("./sqlman");
const serialman = require("./serialman");

const port = 8000;

app.use(bodyparser.urlencoded({extended: false}));
app.use(express.static("static"));

app.get("/", (req, res) => {
	res.send("Test");
});
app.get("/serialports", async (req, res) => {
	res.send(await serialman.list());
});
app.post("/start", (req, res) => {
	// Convert info to sql database
	// Connect to correct usb port
	// Start experiment

});

io.on('connection', (socket) => {
	socket.on('msg', (d) => {
		console.log(`Msg: ${d}`);
	});

	console.log("User connected");
});

server.listen(port, () => {
	console.log(`Running on http://localhost:${port}`);
});
