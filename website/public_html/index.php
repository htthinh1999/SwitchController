<?php
	include_once("../Model/M_Switch.php");
	
	$m_switchs = new M_Switch();
	$switchs = $m_switchs->getAllData();
	
	if($_SERVER["REQUEST_METHOD"] == "POST"){
		$input = file_get_contents('php://input');
		for($i=0; $i<4; $i++){
			$name = 'SW'.($i+1);
			if(strpos($input, $name) !== false){
				$switchs[$i] = new SW($name, (strpos($input, $name.'_ON') !== false)?$name."_ON":$name."_OFF");
			}
		}
		$m_switchs->saveData($switchs);
	}
	
	echo "
		<html>
		<head>
		<title>KEYCODE MON</title>
		</head>
		<body>
			<h1>Electronic Controller</h1>
			<form method='POST'>";
				for($i=0; $i<4; $i++){
					echo "
						SWITCH ".($i+1)."
						<input type='radio' name='".$switchs[$i]->name."' value='SW".($i+1)."_ON' ".((strpos($switchs[$i]->value, 'ON') !== false)?"checked":"")." />ON
						<input type='radio' name='".$switchs[$i]->name."' value='SW".($i+1)."_OFF' ".((strpos($switchs[$i]->value, 'OFF') !== false)?"checked":"")." />OFF
						<br>
					";
				}
	echo 		"<br>
				<input type='submit' />
			</form>
		</body>
		</html>
	";
	
?>