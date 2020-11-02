<?php
	include_once("../Model/M_Switch.php");
	
	$m_switchs = new M_Switch();
	$switchs = $m_switchs->getAllData();
	
	if($_SERVER["REQUEST_METHOD"] == "POST"){
		$input = file_get_contents('php://input');
		for($i=0; $i<4; $i++){
			$name = 'sw'.($i+1);
			if(strpos($input, $name) !== false){
				$switchs[$i] = new SW($name, (strpos($input, $name.'_on') !== false)?$name."_on":$name."_off");
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
						<input type='radio' ".((strpos($switchs[$i]->value, 'on') !== false)?"checked":"")." value='sw".($i+1)."_on' name='".$switchs[$i]->name."' />ON
						<input type='radio' ".((strpos($switchs[$i]->value, 'off') !== false)?"checked":"")." value='sw".($i+1)."_off' name='".$switchs[$i]->name."' />OFF
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