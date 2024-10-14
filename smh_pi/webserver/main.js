const express = require("express");
const app = express();
const port = 8000;

app.use(express.static("static"));

app.get("/", (req, res) => {
	res.send("Test");
});
app.post("/data/{experminet_id}/humidty");

app.listen(port, () => {
	console.log(`Running on http://localhost:${port}`);
});