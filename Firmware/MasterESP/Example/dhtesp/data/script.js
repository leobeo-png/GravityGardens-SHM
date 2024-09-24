var chartT = new Highcharts.Chart({
  chart:{
    renderTo:'chart-temperature'
  },
  series: [
    {
      name: 'Temperature #1',
      type: 'line',
      color: '#101D42',
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
      marker: {
        symbol: 'square',
        radius: 3,
        fillColor: '#00A6A6',
      }
    },
    {
      name: 'Temperature #3',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#8B2635',
      }
    },
    {
      name: 'Temperature #4',
      type: 'line',
      color: '#71B48D',
      marker: {
        symbol: 'triangle-down',
        radius: 3,
        fillColor: '#71B48D',
      }
    },
	{
      name: 'Temperature #5',
      type: 'line',
      color: '#844494',
      marker: {
        symbol: 'diamond',
        radius: 3,
        fillColor: '#844494',
      }
    },
	{
      name: 'Temperature #6',
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
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: {
      text: 'Temperature Celsius Degrees'
    }
  },
  credits: {
    enabled: false
  }
});
function fetchTemperatureData(sensorId) {
  setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        var x = (new Date()).getTime(),
            temperature = parseFloat(this.responseText); // Parse temperature value from response
        
        // Update the corresponding temperature card
        document.getElementById("temperature" + sensorId).innerHTML = temperature + " °C";
        
        // Add point to the temperature chart and limit to 40 points
        if (chartT.series[sensorId - 1].data.length > 1000) {
          chartT.series[sensorId - 1].addPoint([x, temperature], true, true, true);
        } else {
          chartT.series[sensorId - 1].addPoint([x, temperature], true, false, true);
        }
      }
    };
    xhttp.open("GET", "/temperature" + sensorId, true); // Fetch temperature data for the given sensor
    xhttp.send();
  }, 10000); // 4-second interval for temperature updates
}
var chartH = new Highcharts.Chart({
  chart:{
    renderTo:'chart-humidity'
  },
  series: [
    {
      name: 'Humidity #1',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
    {
      name: 'Humidity #2',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'square',
        radius: 3,
        fillColor: '#00A6A6',
      }
    },
    {
      name: 'Humidity #3',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#8B2635',
      }
    },
    {
      name: 'Humidity #4',
      type: 'line',
      color: '#71B48D',
      marker: {
        symbol: 'triangle-down',
        radius: 3,
        fillColor: '#71B48D',
      }
    },
	{
      name: 'Humidity #5',
      type: 'line',
      color: '#844494',
      marker: {
        symbol: 'diamond',
        radius: 3,
        fillColor: '#844494',
      }
    },
	{
      name: 'Humidity #6',
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
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: {
      text: 'Humidity Percentage'
    }
  },
  credits: {
    enabled: false
  }
});
// Function to fetch temperature data and update the temperature chart


// Function to fetch humidity data and update the humidity chart
function fetchHumidityData(sensorId) {
  setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        var x = (new Date()).getTime(),
            humidity = parseFloat(this.responseText); // Parse humidity value from response
        
        // Update the corresponding humidity card
        document.getElementById("humidity" + sensorId).innerHTML = humidity + " %";
        
        // Add point to the humidity chart and limit to 40 points
        if (chartH.series[sensorId - 1].data.length > 1000) {
          chartH.series[sensorId - 1].addPoint([x, humidity], true, true, true);
        } else {
          chartH.series[sensorId - 1].addPoint([x, humidity], true, false, true);
        }
      }
    };
    xhttp.open("GET", "/humidity" + sensorId, true); // Fetch humidity data for the given sensor
    xhttp.send();
  }, 10000); // 4-second interval for humidity updates
}

// Call fetchTemperatureData and fetchHumidityData for all sensors
// Change the "<=" to the number of sensors
for (let i = 1; i <= 3; i++) {
  fetchTemperatureData(i);  
  fetchHumidityData(i);     
}
