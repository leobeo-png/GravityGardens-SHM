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
	async function getExperimentSetting(setting) {
		var data = await rawGetQuery("SELECT settingdata FROM experimentsettings WHERE settingname = ?;", [ setting ]);
		console.log(data);
		return data;
	}
	async function getExperimentSettings() {
		try {
			const dataObj = {
				target_gravity: Number((await getExperimentSetting("target_gravity"))[0].settingdata),
				experiment_length: Number((await getExperimentSetting("experiment_length"))[0].settingdata),
				experiment_start_dt: Date.parse((await getExperimentSetting("experiment_start_dt"))[0].settingdata),
				lights_on_time: Number((await getExperimentSetting("lights_on_time"))[0].settingdata),
				lights_off_time: Number((await getExperimentSetting("lights_off_time"))[0].settingdata),
				experiment_name: (await getExperimentSetting("experiment_name"))[0].settingdata,
				experiment_description: (await getExperimentSetting("experiment_description"))[0].settingdata,
			};
			// console.log(dataObj);
			return dataObj;
		} catch(e) {
			console.error(e);
		}
	}
	async function setExperimentSetting(setting, value) {
		return await rawQuery("UPDATE experimentsettings SET settingdata = ? WHERE settingname = ?;", [value, setting]);
	}
	async function setExperimentSettings(settingsObject) {
		try {
			for(const [key, val] of Object.entries(settingsObject)) {
				console.log(key, val);
				setExperimentSetting(key, `${val}`);
			}
		} catch (e) {
			console.error(e);
		}
	}

	module.exports.setup = setupDatabase;
	module.exports.rawQuery = rawQuery;
	module.exports.rawGetQuery = rawGetQuery;
	module.exports.resetDatabase = resetDatabase;

	module.exports.getExperimentSettings = getExperimentSettings;
	module.exports.setExperimentSettings = setExperimentSettings;
}());
