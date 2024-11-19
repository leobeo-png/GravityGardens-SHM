(function() {
	const mariadb = require("mariadb");
	const poolobj = {
		host: "localhost",
		user: "gg",
		password: "GravityGardens!",
		connectionLimit: 5
	};

	async function getPool() {
		return mariadb.createPool(poolobj);
	}

	module.exports.rawQuery = async function(query, valueArray) {
		let pool = await getPool();
		let conn = await pool.getConnection();
		try {
			const rows = await conn.query(query, valueArray);
			return rows;
		} catch(e) {
			console.error(e);
			return -1;
		} finally {
			if(conn) {
				conn.end();
			}
		}
		await pool.end();
	}
}());
