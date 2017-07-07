<?php
 $Parked=$_POST["parked"];
 $Presence=$_POST["presence"];
 $Sound=$_POST["sound"];
 $Tilt=$_POST["tilt"];
 $Write="<p>CarParked :" . $Parked . "</p>" . "<p>Presence :" . $Presence . "</p>" . "<p>Sound :" . $Sound . "</p>" . "<p>Tilt :" . $Tilt . "</p>";
 file_put_contents('sensor.html',$Write);
?>
