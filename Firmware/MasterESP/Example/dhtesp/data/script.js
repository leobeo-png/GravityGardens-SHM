    function fetchData(sensorId) {
      // Fetch temperature for the sensor
      setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("temperature" + sensorId).innerHTML = this.responseText + " °C";
          }
        };
        xhttp.open("GET", "/temperature" + sensorId, true);
        xhttp.send();
      }, 4000); // 4 seconds interval

      // Fetch humidity for the sensor
      setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("humidity" + sensorId).innerHTML = this.responseText + " %";
          }
        };
        xhttp.open("GET", "/humidity" + sensorId, true);
        xhttp.send();
      }, 4000); // 4 seconds interval
    }

    // Call fetchData for all three sensors
    fetchData(1);  // Sensor 1
    fetchData(2);  // Sensor 2
    fetchData(3);  // Sensor 3
