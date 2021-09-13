<html>
<head>
<title>Power Consumption</title>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
</head>
<?php
include "db.php";?>
<body style="background-image:url('bgneon.jpg');">
<div class="col-sm-offset-7 col-sm-4">
<div class="jumbotron" style="margin-top:250px; padding:50px; padding-top:20px; background-color:rgba(0, 0, 0, 0.8);">
<h2 style="color:#FFFFFF">POWER CONSUMPTION</h2><br>
<form class="text-center border border-light p-5" action="power_stats.php" method="POST">
<input type="date" class="form-control mb-4" name="d1" placeholder="FROM"><br>
<input type="date" class="form-control mb-4" name="d2" placeholder="TO"><br>
<button class="btn btn-primary btn-block" type="submit">SHOW POWER USAGE</button>
</form>
<?php
if(isset($_POST["d1"]) && isset($_POST["d2"]))
{
$d1=$_POST['d1'];
$d2=$_POST['d2'];
$pow=0.0;
$dates="SELECT * FROM power WHERE `Date` BETWEEN '".$d1."' AND '".$d2."'";
$res=mysqli_query($conn,$dates) or die(mysqli_error());
while($row = mysqli_fetch_assoc($res)){	
	$pow += $row['power'];
}
echo "<p style='color:#FFFFFF; font-size:18px;'>Power consumed : ".$pow." watt</p>";
}
?>
</div></div>
</body>
</html>