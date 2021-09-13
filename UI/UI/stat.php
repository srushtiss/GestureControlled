<?php
include "db.php";
$data=array();
if ($conn->connect_error) {
	die("Database Connection failed: " . $conn->connect_error);
}
$sql="SELECT devices.name as name, device, curr_status FROM devices INNER JOIN current_status WHERE devices.name = current_status.name";
$res=$conn->query($sql)or die(mysqli_error($conn));

while($row = mysqli_fetch_assoc($res)){
	$data[]= $row;
	}
echo json_encode($data);
?>