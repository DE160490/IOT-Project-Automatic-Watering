<?php
//Creates new record as per request
    //Connect to database
    $servername = "localhost";
    $username = "root";
    $password = "";
    $dbname = "iot";

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
        die("Database Connection failed: " . $conn->connect_error);
    }

    //Get current date and time
    date_default_timezone_set('Asia/Bangkok');
    $d = date("Y-m-d");
    //echo " Date:".$d."<BR>";
    $t = date("H:i:s");

    if(!empty($_POST['doam']) && !empty($_POST['trangthaimua']))
    {
    	$doam = $_POST['doam'];
    	$trangthaimua = $_POST['trangthaimua'];
		$trangthaibom = $_POST['trangthaibom'];
	    $sql = "INSERT INTO iot (doam, trangthaimua, trangthaibom, Date, Time)
		
		VALUES ('".$doam."', N'".$trangthaimua."', N'".$trangthaibom."' , '".$d."', '".$t."')";

		if ($conn->query($sql) === TRUE) {
		    echo "OK";
		} else {
		    echo "Error: " . $sql . "<br>" . $conn->error;
		}
	}else  echo "Error: ";


	$conn->close();
?>