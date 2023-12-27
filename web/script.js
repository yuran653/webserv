document.addEventListener("DOMContentLoaded", function() {
	// Dynamic greeting based on the time of day
	const greeting = document.getElementById('greeting');
	const hour = new Date().getHours();
	if (hour < 12) {
		greeting.textContent = 'Good Morning!';
	} else if (hour < 18) {
		greeting.textContent = 'Good Afternoon!';
	} else {
		greeting.textContent = 'Good Evening!';
	}

	// Smooth scrolling for navigation links
	document.querySelectorAll('nav a').forEach(anchor => {
		anchor.addEventListener('click', function(e) {
			e.preventDefault();
			document.querySelector(this.getAttribute('href')).scrollIntoView({
				behavior: 'smooth'
			});
		});
	});

	// Modal interaction
	const modal = document.getElementById("modal");
	const span = document.getElementsByClassName("close")[0];

	// When the user clicks on <span> (x), close the modal
	span.onclick = function() {
		modal.style.display = "none";
	}

	// When the user clicks anywhere outside of the modal, close it
	window.onclick = function(event) {
		if (event.target == modal) {
			modal.style.display = "none";
		}
	}

	// Handling coalition section clicks
	document.querySelectorAll('.coalition').forEach(coalition => {
		coalition.addEventListener('click', function() {
			const coalitionName = this.querySelector('h2').textContent;
			document.getElementById('form-title').textContent = `Join ${coalitionName}`;
			document.getElementById('selected-coalition').value = coalitionName;
			modal.style.display = "block";
		});
	});
});
