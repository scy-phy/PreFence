<?php
// based on: https://www.w3schools.com/php/php_file_upload.asp

if ( ! isset($_FILES["file"])) {
	http_response_code(400);
	die("Sorry, upload failed.");
}

$targetDir = "uploads/";
$targetFile = $targetDir . basename($_FILES["file"]["name"]);

// Check if file already exists
if (file_exists($targetFile)) {
	http_response_code(400);
	die("Sorry, file exists.");
}

$uploadSuccessful = move_uploaded_file($_FILES["file"]["tmp_name"], $targetFile);
if (! $uploadSuccessful) {
	http_response_code(400);
	die("Sorry, there was an error moving the uploaded file.");
}

?>

<!doctype html>
<html>
	<head>
		<meta charset="utf-8" />
		<title>Upload</title>
	</head>
	<body>
		<p>Upload OK.</p>
		<p><a href='index.php'>Go back</a></p>
	</body>
</html>

