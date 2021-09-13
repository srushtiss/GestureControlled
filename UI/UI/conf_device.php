<?php
include "db.php";
    //TABLE #1 DEVICES
    
    //data here is to be inserted at the beginning(configuration of all the devices in the system)
    //id	d_id    name	device		wattage
    //123	1		L1		light		10.000
    if(!empty($_POST['name']) && !empty($_POST['device']))
    {
        $name=$_POST['name'];
        $wattage=$_POST['watt'];
        $d_id=$_POST['device'];
        if($d_id==1){
            $device='Light';
            $curr_status='OF';
        }
        elseif($d_id==2){
            $device='Fan';
            $curr_status='OF';
        }
        elseif($d_id==3){
            $device='Door';
            $curr_status='CL';
        }
        elseif($d_id==4){
            $device='Window';
            $curr_status='CL';
        }

        $devices="INSERT INTO devices(name,d_id,device,wattage) VALUES ('".$name."','".$d_id."','".$device."','".$wattage."')";
        if ($conn->query($devices) === TRUE) {
            #$id = $conn->insert_id;
            echo "Device configured.";
		} else {
		    echo "Error: " . $devices . "<br>" . $conn->error;
        }

        $curr="INSERT INTO current_status(name,curr_status) VALUES ('".$name."','".$curr_status."')";
        if ($conn->query($curr) === TRUE) {
            echo "Added to status.";
		} else {
		    echo "Error: " . $devices . "<br>" . $conn->error;
        }
        header("location:status.php");
    }
    else{
        header("location:add_device.php");
    }
	$conn->close();
?>