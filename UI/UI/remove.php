<?php
include "db.php";
$name=$_POST['name'];
$del="DELETE FROM devices WHERE name='".$name."'";
if ($conn->query($del) === TRUE) {
	echo "Device deleted.";
	header("Location:status.php");
} else {
	echo "Error: " . $del . "<br>" . $conn->error;
}
?>