<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8" />
		<title>File Sharing Service</title>
	</head>
	<body>
		<h1>File Sharing Service</h1>
		<h2>Upload</h2>
		<form action="upload.php" method="post" enctype="multipart/form-data">
			<input type="file" name="file" id="file" />
			<input type="submit" name="submit" value="Upload" />
		</form>

		<h2>Uploaded Files</h2>
		<ul>
		<?php
			foreach (glob("uploads/*") as &$file) {
				$file_san = htmlspecialchars(basename($file));
				echo "<li>";
				echo "<a href='uploads/$file_san'>$file_san</a>";
				echo " <a href='remove.php?file=$file_san'><button>Remove</button></a>";
				echo "</li>";
			}
		?>
		</ul>
	</body>
</html>

