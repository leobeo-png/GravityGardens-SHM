function increaseProgress() {
            const progressBar = document.getElementById("progressBar");
            if (progressBar.value <= progressBar.max) {
                progressBar.value += 10;
            }
        }
function decreaseProgress() {
			const progressBar = document.getElementById("progressBar");
            if (progressBar.value <= progressBar.max) {
                progressBar.value -= 10;
            }
        }
function stopProgress() {
			const progressBar = document.getElementById("progressBar");
            if (progressBar.value <= progressBar.max) {
                progressBar.value -=100;
            }
        }