(function() {
	const sqlite3 = require("sqlite3");
	sqlite3.verbose();
	db = new sqlite3.Database("./database.db");
	fs = require("fs");

	async function setupDatabase() {

	}
	async function rawGetQuery(query, valueArray = []) {
		return new Promise(function (res, rej) {
			db.serialize(() => {
				db.all(query, valueArray, (err, rows) => {
					if(err) rej(err);
					res(rows);
				});
			});
		});
	}
	async function rawQuery(query, valueArray = []) {
		return new Promise(function (res, rej) {
			db.serialize(() => {
				db.run(query, valueArray, (err, rows) => {
					if(err) rej(err);
					res(rows);
				});
			});

		});
	}
	async function resetDatabase() {
		// const filedata = fs.readFileSync("../db_setup.sql").toString().split('\n');
		const filedata = fs.readFileSync("../db_setup.sql").toString();

		await rawQuery("PRAGMA foreign_keys=OFF;");
		await rawQuery("BEGIN TRANSACTION;");
		// for(var i = 0; i < filedata.length - 1; i++) {
		// 	// console.log(`Running ${i}: ${filedata[i]}`);
		// 	await rawQuery(`${filedata[i]}`);
		// }
		await db.exec(filedata);
		await rawQuery("COMMIT;");
	}
	async function getExperimentSetting(setting, experimentid) {
		var data = await rawGetQuery("SELECT settingdata FROM experimentsettings WHERE settingname = ? AND experimentid = ?;", [ setting, experimentid ]);
		console.log(data);
		return data;
	}
	async function getExperimentSettings(experimentid) {
		console.log(experimentid);
		try {
			const dataObj = {
				target_gravity: Number((await getExperimentSetting("target_gravity", experimentid))[0].settingdata),
				experiment_length: Number((await getExperimentSetting("experiment_length", experimentid))[0].settingdata),
				experiment_start_dt: Date.parse((await getExperimentSetting("experiment_start_dt", experimentid))[0].settingdata),
				lights_on_time: Number((await getExperimentSetting("lights_on_time", experimentid))[0].settingdata),
				lights_off_time: Number((await getExperimentSetting("lights_off_time", experimentid))[0].settingdata),
				experiment_name: (await getExperimentSetting("experiment_name", experimentid))[0].settingdata,
				experiment_description: (await getExperimentSetting("experiment_description", experimentid))[0].settingdata,
			};
			// console.log(dataObj);
			return dataObj;
		} catch(e) {
			console.error(e);
		}
	}
	async function setExperimentSetting(setting, value, experimentid) {
		// console.log("Id: ", experimentid);
		return await rawQuery("UPDATE experimentsettings SET settingdata = ? WHERE settingname = ? AND experimentid = ?;", [value, setting, experimentid]);
	}
	async function setExperimentSettings(settingsObject, experimentid) {
		try {
			for(const [key, val] of Object.entries(settingsObject)) {
				console.log(key, val);
				setExperimentSetting(key, `${val}`, experimentid);
			}
		} catch (e) {
			console.error(e);
		}
	}

	async function writeExperimentLog(experimentid, rpm, humidity, temperature, lightstatus) {
		return await rawQuery("INSERT INTO experimentlogs (experimentid, rpm, temperature, humidity, lightstatus) VALUES (?, ?, ?, ?)",
			[ experimentid, rpm, temperature, humidity, lightstatus ]
		);
	}


	module.exports.setup = setupDatabase;
	module.exports.rawQuery = rawQuery;
	module.exports.rawGetQuery = rawGetQuery;
	module.exports.resetDatabase = resetDatabase;

	module.exports.getExperimentSettings = getExperimentSettings;
	module.exports.setExperimentSettings = setExperimentSettings;
}());
