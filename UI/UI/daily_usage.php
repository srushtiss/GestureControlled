<?php
$fDate = $date->format("Y-m-d");
$date = '2019-03-01';

$begin = new DateTime($date);
$end = new DateTime(date('Y-m-d', strtotime($date . '+ 1 year'))); // you can change + 1 year to what you need

$interval = new DateInterval('P1D'); // one day
//$interval = new DateInterval('P1W'); // one week

$daterange = new DatePeriod($begin, $interval, $end);

foreach ($daterange as $date) { 
	
	$dates="SELECT * FROM power WHERE `Date`='".$date."' ";
	$res=mysqli_query($conn,$dates) or die(mysqli_error());
	
	while($row = mysqli_fetch_assoc($res)){	
	$id+=$row[''];
	$pow += $row['power'];
	$time+=$row['duration'];
	}

	$sql = "INSERT INTO power_pred (id,dev,`date`, duration, power) VALUES ('{$fDate}','{$fDay}','{$fMonth}',.....)";

    if (mysqli_query($conn, $sql)) {
        echo "New records created successfully";
    } else {
        echo "Error: " . $sql . "<br>" . mysqli_error($conn);
    }
}
?>