<html>
<head>
<title>Device Configuration</title>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
<script>
function confirm(){
	alert("Delete device?");
}
</script>
</head>
<body style="background-image:url('bgneon.jpg');">

<div class="col-sm-offset-7 col-sm-4">
<div class="jumbotron" style="margin-top:250px; padding:50px; padding-top:20px; background-color:rgba(0, 0, 0, 0.8);">
<h2 style="color:#FFFFFF">REMOVE DEVICE</h2><br>
<form class="text-center border border-light p-5" action="remove.php" method="POST">

    <select type="text" name="device" placeholder="--Select type of device--" style=" width: 100%; padding: 7px;">
                    <option value="">--Select type of device--</option>
                    <option value=1>Light</option>
                    <option value=2>Fan</option>
                    <option value=3>Door</option>
                    <option value=4>Window</option></select><br><br>
    <input type="text" name="name" class="form-control mb-4" placeholder="Device Name"><br>

    <button class="btn btn-primary btn-block" type="submit" onclick="confirm()">REMOVE DEVICE</button>
</form>
</div>
</div>
</div>
</body>
</html>