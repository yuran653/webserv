function sendDeleteRequest() {
	fetch('delete/file.jpeg', {
		method: 'DELETE',
		headers: {
			'Content-Type': 'application/json'
		},
	})
	.then(response => {
		if (!response.ok) {
			throw new Error('Network response was not ok');
		}
		return response.json();
	})
	.then(data => {
		console.log('Delete request successful', data);
	})
	.catch(error => {
		console.error('There was a problem with the delete request:', error);
	});
}