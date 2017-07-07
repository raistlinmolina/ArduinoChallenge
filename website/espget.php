<?php
 $Parked=$_GET["parked"];
 $Presence=$_GET["presence"];
 $Sound=$_GET["sound"];
 $Tilt=$_GET["tilt"];
 $Write="<p>CarParked :" . $Parked . "</p>" . "<p>Presence :" . $Presence . "</p>" . "<p>Sound :" . $Sound . "</p>" . "<p>Tilt :" . $Tilt . "</p>";
 file_put_contents('sensor.html',$Write);
?>
