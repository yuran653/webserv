document.addEventListener('DOMContentLoaded', function() {
    var greetingElement = document.getElementById('greeting');

    // Function to animate the greeting text
    function animateGreeting() {
        // Move the greeting text up and down by changing the margin-top property
        greetingElement.style.marginTop = '10px';

        // Use a timeout to set the margin-top back to 0 after a delay
        setTimeout(function() {
            greetingElement.style.marginTop = '0';
        }, 500); // 500 milliseconds delay (adjust as needed)

        // Call the animateGreeting function again after the animation is complete
        setTimeout(animateGreeting, 1000); // Repeat every 1 second (adjust as needed)
    }

    // Start the animation
    animateGreeting();
});
