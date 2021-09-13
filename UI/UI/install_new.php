<?php
//Create Data base if not exists
	$servername = "localhost";
	$username = "root";
	$password = "";

	// Create connection
	$conn = new mysqli($servername, $username, $password);
	// Check connection
	if ($conn->connect_error) {
	    die("Connection failed: " . $conn->connect_error);
	}

	// Create database
	$sql = "CREATE DATABASE gcha1";
	if ($conn->query($sql) === TRUE) {
	    echo "Database created successfully";
	} else {
	    echo "Error creating database: " . $conn->error;
	}

	$conn->close();
	echo "<br/>"; 
//Connect to database and create table
	$servername = "localhost";
	$username = "root";
	$password = "";
	$dbname = "gcha1";

	// Create connection
	$conn = new mysqli($servername, $username, $password, $dbname);
	// Check connection
	if ($conn->connect_error) {
	    die("Connection failed: " . $conn->connect_error);
	}

	//id	d_id(1:Light,2:Fan,3:Door,4:Window)		name	device		wattage
	//123	1										L1		light		10.000
	$devices= "CREATE TABLE devices(
	name VARCHAR(30) PRIMARY KEY,
	d_id INT(2),
	device VARCHAR(30),
	wattage FLOAT(10,3) DEFAULT NULL
	)";

	if ($conn->query($devices) === TRUE) {
	    echo "Table devices created successfully";
	} else {
	    echo "Error creating table: " . $conn->error;
	}
	echo "<br/>"; 
	//id	l_id		on_time 				off_time 				Date		duration	curr_status(ON, OF, OP, CL)
	//1		123         2017-06-15 13:10:10		2017-06-25 13:10:11		12-5-18    	30			OF
	// sql to create table
	$lights = "CREATE TABLE lights (
	id INT(6) AUTO_INCREMENT PRIMARY KEY,
	l_name VARCHAR(30),
	on_time TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,
	off_time TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	`Date` DATE NULL,
	duration TIME NULL, 
	curr_status CHAR(5),
	FOREIGN KEY(l_name) REFERENCES devices(name)
	ON UPDATE CASCADE ON DELETE SET NULL
	)";

	if ($conn->query($lights) === TRUE) {
	    echo "Table lights created successfully";
	} else {
	    echo "Error creating table: " . $conn->error;
	}
	echo "<br/>"; 
	//id	f_id	on_time 				off_time 				Date		duration	curr_status(ON, OF, OP, CL)		speed
	//1		223     2017-06-15 13:10:10		2017-06-25 13:10:11		12-5-18    	30			ON									2
	$fans = "CREATE TABLE fans (
	id INT(6) AUTO_INCREMENT PRIMARY KEY,
	f_name VARCHAR(30),
	on_time TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,
	off_time TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	`Date` DATE NULL,
	duration TIME NULL, 
	curr_status CHAR(5),
	speed INT(2),
	FOREIGN KEY(f_name) REFERENCES devices(name)
	ON UPDATE CASCADE ON DELETE SET NULL
	)";


	if ($conn->query($fans) === TRUE) {
	    echo "Table fans created successfully";
	} else {
	    echo "Error creating table: " . $conn->error;
	}
	echo "<br/>"; 
	//id	d_id		open_time 				close_time 				Date		duration	curr_status(ON, OF, OP, CL)
	//1		323         2017-06-15 13:10:10		2017-06-25 13:10:11		12-5-18    	30			NM
	$doors = "CREATE TABLE doors (
	id INT(6) AUTO_INCREMENT PRIMARY KEY,
	d_name VARCHAR(30),
	open_time TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,
	close_time TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	`Date` DATE NULL,
	duration TIME NULL, 
	curr_status CHAR(5),
	FOREIGN KEY(d_name) REFERENCES devices(name)
	ON UPDATE CASCADE ON DELETE SET NULL
	)";
	if ($conn->query($doors) === TRUE) {
	    echo "Table doors created successfully";
	} else {
	    echo "Error creating table: " . $conn->error;
	}
	echo "<br/>"; 

	//id	w_id		open_time 				close_time 				Date		duration	curr_status(ON, OF, OP, CL)
	//1		423         2017-06-15 13:10:10		2017-06-25 13:10:11		12-5-18    	30			ON
	$windows = "CREATE TABLE window (
	id INT(6) AUTO_INCREMENT PRIMARY KEY,
	w_name VARCHAR(30),
	open_time TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,
	close_time TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	`Date` DATE NULL,
	duration TIME NULL, 
	curr_status CHAR(5),
	FOREIGN KEY(w_name) REFERENCES devices(name)
	ON UPDATE CASCADE ON DELETE SET NULL
	)";


	if ($conn->query($windows) === TRUE) {
	    echo "Table window created successfully";
	} else {
	    echo "Error creating table: " . $conn->error;
	}
	echo "<br/>"; 

	//id		curr_status(OP,CL,ON,OF)
	//123		ON
	$current = "CREATE TABLE current_status (
	name VARCHAR(30),
	curr_status CHAR(5),
	FOREIGN KEY(name) REFERENCES devices(name)
	ON DELETE CASCADE ON UPDATE SET NULL
	)";


	if ($conn->query($current) === TRUE) {
	    echo "Table 'current' created successfully";
	} else {
	    echo "Error creating table: " . $conn->error;
	}
	echo "<br/>"; 

	//sno	id	duration(in mins)	power
	//1		123	0.5					1.000
	$power_usage="CREATE TABLE power(
	s_no INT(6) AUTO_INCREMENT PRIMARY KEY, 
	name VARCHAR(30),
	`Date` DATE NULL,
	duration TIME NULL,
	power FLOAT(10,3),
	FOREIGN KEY(name) REFERENCES devices(name)
	ON UPDATE CASCADE ON DELETE SET NULL
	)";

	if ($conn->query($power_usage) === TRUE) {
	    echo "Table power created successfully";
	} else {
	    echo "Error creating table: " . $conn->error;
	}
	echo "<br/>"; 
/*
	$power_prediction= "CREATE TABLE pow_pred(
		id INT(6) AUTO_INCREMENT PRIMARY KEY,
		dev INT(6),
		`Date` DATE NULL,
		duration TIME NULL,
		power FLOAT(10,3),
		FOREIGN KEY(dev) REFERENCES devices(id)
		ON UPDATE CASCADE ON DELETE SET NULL)";
	
	if ($conn->query($power_prediction) === TRUE) {
	    echo "Table power created successfully";
	} else {
	    echo "Error creating table: " . $conn->error;
	}
*/
	$conn->close();
?>