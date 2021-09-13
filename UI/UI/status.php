<!DOCTYPE html>
<html>
<head><title></title>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
<script type="text/javascript">
	setInterval(function(){
		var data={};
		$.get('stat.php',function(res){
			html="";
			for(var a = 0 ; a < res.length ; a++)
				{
					var name = res[a].name;
					var device = res[a].device;
					var stat = res[a].curr_status;

					html += "<tr>" ;
					html += "<td>" + name + "</td>" ;
					html += "<td>" + device + "</td>" ;
					html += "<td>" + stat + "</td>" ;
					html += "</tr>" ;
				}
				$('#tabledata').empty();
				$('#tabledata').html(html);				
		},'json');
	},1000);
</script>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css"></head>
<body style="background-image:url('bgneon.jpg');">
<div class="col-sm-offset-1 col-sm-2" style="margin-top: 100px; padding:25px; background-color:rgba(0, 0, 75, 0.7)">
<a href="add_device.php" style="text-decoration: none;"><button class="btn btn-default btn-block" type="submit">Add a device</button></a><br>
<a href="remove_device.php" style="text-decoration: none;"><button class="btn btn-default btn-block" type="submit">Remove a device</button></a><br>
<a href="power_stats.php" style="text-decoration: none;"><button class="btn btn-default btn-block" type="submit">View power usage</button></a><br>
</div>
<div class="col-sm-6" style="color:#FFFFFF; margin-top:100px; background-color:rgba(0, 0, 0, 0.85); padding:25px;">
<table class="table">
  <thead class="thead-dark">
    <tr>
      <th scope="col">NAME</th>
      <th scope="col">DEVICE</th>
      <th scope="col">STATUS</th>
    </tr>
  </thead>
  <tbody id="tabledata">
  </tbody>
</table>

</div>
</body>
</html>