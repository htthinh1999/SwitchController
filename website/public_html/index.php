<?php
	
	function authenticate(){
		if(!isset($_SERVER['PHP_AUTH_USER']) || ($_SERVER['PHP_AUTH_USER'] !== 'admin' || $_SERVER['PHP_AUTH_PW'] !== 'admin123')){				
			header("WWW-Authenticate: Basic realm=\"keycode\"");
			header("HTTP\ 1.0 401 Unauthorized");
			die('Something goes wrong!');
		}
	}

	authenticate();
	
	include_once("../Model/M_Switch.php");
	
	$m_switchs = new M_Switch();
	$switchs = $m_switchs->getAllData();
	
	if($_SERVER["REQUEST_METHOD"] == "POST"){
		$input = file_get_contents('php://input');
		for($i=0; $i<4; $i++){
			$name = 'sw'.($i+1);
			if(strpos($input, $name) !== false){
				$switchs[$i] = new SW($name, (strpos($input, $name.'on') !== false)?$name."on":$name."off");
			}
		}
		$m_switchs->saveData($switchs);
	}
?>
<html>
<head>
<title>Electronic Controller</title>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="assets/css/style.css">
<link rel="stylesheet" href="assets/css/bootstrap.min.css">
<link rel="stylesheet" href="assets/css/bootstrap4-toggle.min.css">

<script src="assets/js/font-awesome.js"></script>
</head>
<body>
	<div>
		<nav class="navbar navbar-expand-lg navbar-light bg-light">
			<a class="navbar-brand" href="#"><i class="fas fa-plug"></i> Electronic Controller</a>
			<ul class="navbar-nav ml-auto">
				<li class="nav-item">
					<a class="nav-link" href="#">WELCOME KEYCODE MON</a>
				</li>
				<li class="nav-item">
					<a class="nav-link" href="#">Logout</a>
				</li>
			</ul>
		</nav>
	</div>
	
	<?php
		$rs = '';
		for($i=0; $i<4; $i++){
			$rs.=$switchs[$i]->value;
		}
		echo "<input value='".$rs."' id='data' type='hidden'>";
	?>
	
	<div>
		<form id='main-form' method='POST'>
			<div style='display:none'>
			<?php
				for($i=0; $i<4; $i++){
					echo "
						<div class='form-check'>
							<label>SWITCH ".($i+1)."</label>
							<input type='radio' ".((strpos($switchs[$i]->value, 'on') !== false)?"checked":"")." value='sw".($i+1)."on' name='".$switchs[$i]->name."' id='sw".($i+1)."on' class='form-check-input'>
							<label class='form-check-label' for='sw".($i+1)."on'>ON</label>
							<input type='radio' ".((strpos($switchs[$i]->value, 'off') !== false)?"checked":"")." value='sw".($i+1)."off' name='".$switchs[$i]->name."' id='sw".($i+1)."off' class='form-check-input'>
							<label class='form-check-label' for='sw".($i+1)."off'>OFF</label>
						</div>
					";
				}
			?>
			</div>
			<?php
				for($i=0; $i<4; $i++){
					echo "
					<div class='form-check'>
						<label class='form-check-label' for='switch".($i+1)."'><b>SWITCH ".($i+1)."</b></label>
						<input class='form-check-input' type='checkbox' ".((strpos($switchs[$i]->value, 'on') !== false)?"checked":"")." data-toggle='toggle' data-onstyle='success' data-offstyle='danger' id='switch".($i+1)."' data-size='sm' onchange='updateData(".($i+1).")' >
					</div>
					";
				}
			?>
		</form>
	</div>
	
	<script src="assets/js/main.js"></script>
	<script src="assets/js/jquery-3.2.1.slim.min.js"></script>
    <script src="assets/js/popper.min.js"></script>
    <script src="assets/js/bootstrap.min.js"></script>
	<script src="assets/js/bootstrap4-toggle.min.js"></script>
</body>
</html>