<?php
    include "db.php";
    date_default_timezone_set('Asia/Kolkata');
    $d = date("Y-m-d");
    function calc_duration($t1,$t2)
    {
        $t1=strtotime($t1);
        $t2=strtotime($t2);
        $duration=$t2-$t1;
        return ($duration/3600);
    }
    //TABLE #2 CURRENT

    //This table shows the status of every device. At the beginning by default it will have OFF/CLOSED values(given by above trigger) and will be affected whenever any change occurs
    //name/station		status(OP,CL,ON,OF)
    if(!empty($_POST['status']) && !empty($_POST['name']))
    {
        $name=$_POST['name'];
        $status=$_POST['status'];
        /*$getid="SELECT id FROM devices WHERE name='".$name."'";
        $res=$conn->query($getid);
        $res=mysqli_fetch_assoc($res);
        $id=$res['id'];*/
        $current="UPDATE CURRENT_STATUS SET curr_status='".$status."' WHERE name='".$name."'";
        if ($conn->query($current) === TRUE) {
            echo "Status updated.";
        
        $sql="SELECT d_id,wattage FROM devices WHERE name='".$name."'";
        if ($conn->query($sql)) {
            echo "Device name fetched.<br>";
            $res = $conn->query($sql);
            $tuple=mysqli_fetch_assoc($res);
            $d_id=$tuple['d_id'];
            $watt=$tuple['wattage'];
		} else {
		    echo "Error: " . $sql . "<br>" . $conn->error;
		}
    
        //lights
        /*every time a light is switched ON, new row is inserted with start time and end time same, 0 duration.
        If the same light is switched off, the row is updated. Hence off time is also updated and then duration is calculated. These values are put in the updated row.
        Power is calculated after switching off based on duration. Hence a new row will be inserted in the power table everytime a device is switched off. Same for fan.
        */
        //id	l_id		on_time 				off_time 				Date		duration	curr_status(ON, OF, OP, CL)
	    //1		123         2017-06-15 13:10:10		2017-06-25 13:10:11		12-5-18    	30      	ON
        
        if($d_id==1)
        {
            if($status=='ON')
            {
                $light_on="INSERT INTO lights(l_name,date,curr_status) VALUES ('".$name."','".$d."','".$status."')";
                if ($conn->query($light_on) === TRUE)
                {
                    echo "Light ON log created";
                } else {
                    echo "Error: " . $light_on . "<br>" . $conn->error;
                }
                echo "<br>";
            }
            elseif($status=='OF')
            {
                $loff="UPDATE lights SET off_time=NOW() WHERE l_name='".$name."' AND curr_status='ON'";
                if($conn->query($loff) === TRUE){
                    echo "OFF time updated";
                }else{
                    echo "Error: " . $loff . "<br>" . $conn->error;
                }
                echo "<br>";

                $sel="SELECT * FROM lights WHERE l_name='".$name."' AND curr_status='ON'";
                if ($conn->query($sel)) {
                    echo "ON and OFF time fetched.";
                } else {
                    echo "Error: " . $sel . "<br>" . $conn->error;
                }
                echo "<br>";

                $res=$conn->query($sel);
                $timediff=mysqli_fetch_assoc($res);
                $t1=$timediff['on_time'];
                $t2=$timediff['off_time'];
                                            
                $light_off="UPDATE lights SET duration=(SELECT TIMEDIFF('".$t2."','".$t1."')), curr_status='OF' WHERE l_name='".$name."' AND curr_status='ON'";
                if ($conn->query($light_off) === TRUE) {
                    echo "Duration updated. Light OFF";
                } else {
                    echo "Error: " . $light_off . "<br>" . $conn->error;
                }
                echo "<br>";

                $duration=calc_duration($t1,$t2);
                $pow = $watt * $duration;
                $power = "INSERT INTO power(name,date,duration,power) VALUES ('".$name."','".$d."',(SELECT TIMEDIFF('".$t2."','".$t1."')),'".$pow."')";
                if($conn->query($power) === TRUE){
                    echo "Data entered into Power table.";
                } else {
                    echo "Error: " . $power . "<br>" . $conn->error;
                }
                echo "<br>";                
            }
        }
        
        //fans
        //id	f_id	on_time 				off_time 				Date		duration	curr_status(ON, OF, OP, CL)		speed
	    //1		223     2017-06-15 13:10:10		2017-06-25 13:10:11		12-5-18    	20         	ON									2
        if($d_id==2)
        {
            if($status=='ON')
            {
                #$speed=$_POST['speed'];
                $fan_on="INSERT INTO fans(f_name,date,curr_status) VALUES ('".$name."','".$d."','".$status."')";
                if ($conn->query($fan_on) === TRUE)
                {
                    echo "Fan ON log created.";
                } else {
                    echo "Error: " . $fan_on . "<br>" . $conn->error;
                }
                echo "<br>";
            }
            elseif($status=='OF')
            {
                $foff="UPDATE fans SET off_time=NOW() WHERE f_name='".$name."' AND curr_status='ON'";
                if($conn->query($foff) === TRUE){
                    echo "OFF time updated";
                }else{
                    echo "Error: " . $foff . "<br>" . $conn->error;
                }
                echo "<br>";

                $sel="SELECT * FROM fans WHERE f_name='".$name."' AND curr_status='ON'";
                if ($conn->query($sel)) {
                    echo "ON and OFF time fetched.";
                } else {
                    echo "Error: " . $sel . "<br>" . $conn->error;
                }
                echo "<br>";

                $res=$conn->query($sel);
                $timediff=mysqli_fetch_assoc($res);
                $t1=$timediff['on_time'];
                $t2=$timediff['off_time'];
                            
                $fan_off="UPDATE fans SET duration=(SELECT TIMEDIFF('".$t2."','".$t1."')), curr_status='OF' WHERE f_name='".$name."' AND curr_status='ON'";
                if ($conn->query($fan_off) === TRUE) {
                    echo "Duration updated. Fan OFF";
                } else {
                    echo "Error: " . $fan_off . "<br>" . $conn->error;
                }
                echo "<br>";

                $duration=calc_duration($t1,$t2);
                $pow = $watt * $duration;
                $power = "INSERT INTO power(name,date,duration,power) VALUES ('".$name."','".$d."',(SELECT TIMEDIFF('".$t2."','".$t1."')),'".$pow."')";
                if($conn->query($power) === TRUE){
                    echo "Data entered into Power table.";
                } else {
                    echo "Error: " . $power . "<br>" . $conn->error;
                }
                echo "<br>";
            }
        }

        //doors
        //name	d_name		open_time 				close_time 				Date		duration	curr_status(ON, OF, OP, CL)
	    //1		323         2017-06-15 13:10:10		2017-06-25 13:10:11		12-5-18    	20      	OP
        if($d_id==3)
        {
            if($status=='OP')
            {
                $door_open="INSERT INTO doors(d_name,date,curr_status) VALUES ('".$name."','".$d."','".$status."')";
                if ($conn->query($door_open) === TRUE)
                {
                    echo "Door opened.";
                } else {
                    echo "Error: " . $door_open . "<br>" . $conn->error;
                }
                echo "<br>";
            }
            elseif($status=='CL')
            {
                $dcl="UPDATE doors SET close_time=NOW() WHERE d_name='".$name."' AND curr_status='OP'";
                if($conn->query($dcl) === TRUE){
                    echo "CLOSE time updated.";
                }else{
                    echo "Error: " . $dop . "<br>" . $conn->error;
                }
                echo "<br>";

                $sel="SELECT * FROM doors WHERE d_name='".$name."' AND curr_status='OP'";
                if ($conn->query($sel)) {
                    echo "ON and OFF time fetched.";
                } else {
                    echo "Error: " . $sel . "<br>" . $conn->error;
                }
                echo "<br>";

                $res=$conn->query($sel);
                $timediff=mysqli_fetch_assoc($res);
                $t1=$timediff['open_time'];
                $t2=$timediff['close_time'];

                $door_close="UPDATE doors SET duration=(SELECT TIMEDIFF('".$t2."','".$t1."')), curr_status='OF' WHERE d_name='".$name."' AND curr_status='OP'";
                if ($conn->query($door_close) === TRUE) {
                    echo "Duration updated. Door closed.";
                } else {
                    echo "Error: " . $door_close . "<br>" . $conn->error;
                }
                echo "<br>";
            }
        }

        //window
        //name	w_name		open_time 				close_time 				Date		duration	curr_status(ON, OF, OP, CL)
	    //1		423         2017-06-15 13:10:10		2017-06-25 13:10:11		12-5-18    	30      	OP
        if($d_id==4)
        {
            if($status=='OP')
            {
                $window_open="INSERT INTO window(w_name,date,curr_status) VALUES ('".$name."','".$d."','".$status."')";
                if ($conn->query($window_open) === TRUE)
                {
                    echo "Window opened.";
                } else {
                    echo "Error: " . $window_open . "<br>" . $conn->error;
                }
                echo "<br>";
            }
            elseif($status=='CL')
            {
                $wcl="UPDATE window SET close_time=NOW() WHERE w_name='".$name."' AND curr_status='OP'";
                if($conn->query($wcl) === TRUE){
                    echo "CLOSE time updated.";
                }else{
                    echo "Error: " . $dop . "<br>" . $conn->error;
                }
                echo "<br>";

                $sel="SELECT * FROM window WHERE w_name='".$name."' AND curr_status='OP'";
                if ($conn->query($sel)) {
                    echo "ON and OFF time fetched.";
                } else {
                    echo "Error: " . $sel . "<br>" . $conn->error;
                }
                echo "<br>";

                $res=$conn->query($sel);
                $timediff=mysqli_fetch_assoc($res);
                $t1=$timediff['open_time'];
                $t2=$timediff['close_time'];
                
                $window_close="UPDATE window SET duration=(SELECT TIMEDIFF('".$t2."','".$t1."')), curr_status='OF' WHERE w_name='".$name."' AND curr_status='OP'";
                if ($conn->query($window_close) === TRUE) {
                    echo "Duration updated. Door closed.";
                } else {
                    echo "Error: " . $window_close . "<br>" . $conn->error;
                }
                echo "<br>";
            }
        }
    }
		else {
            echo "Error: " . $current . "<br>" . $conn->error;
        }
    }

    //for bill create another table maybe of daily/monthly usage ???

	$conn->close();
?>