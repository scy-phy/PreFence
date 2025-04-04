<?php
if ( ! isset($_GET["file"])) {
	http_response_code(400);
	die("No filename supplied");
}
$targetDir = "uploads/";
$realTargetDir = realpath($targetDir);
$path = $realTargetDir . "/" . $_GET["file"];
$realPath = realpath($path);

if ($realPath === false) {
	http_response_code(400);
	die("Invalid file name.");
}
if (strpos($realPath, $path) !== 0) {
	http_response_code(400);
	die("No traversals please.");
}

unlink($realPath);

header("Location: index.php");
