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

	function process(){
		$m_switchs = new M_Switch();
		$switchs = $m_switchs->getAllData();
		
		if($_SERVER["REQUEST_METHOD"] == "POST"){
			$input = file_get_contents('php://input');
			for($i=0; $i<4; $i++){
				$name = $switchs[$i]->name;
				if(strpos($input, $name) !== false){
					$switchs[$i] = new SW($name, (strpos($input, $name.'on') !== false)?$name."on":$name."off");
				}
			}
			$m_switchs->saveData($switchs);
		}
	}
	
	process();
?>
<html>
<head>
<title>Electronic Controller</title>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">

<link rel="stylesheet" href="assets/css/style.css">

<!-- Boostrap min CSS -->
<link rel="stylesheet" href="assets/css/bootstrap.min.css">
<!-- <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous"> -->
<!-- End Boostrap min CSS -->

<!-- Boostrap toggle min CSS -->
<link rel="stylesheet" href="assets/css/bootstrap4-toggle.min.css">
<!-- <link href="https://cdn.jsdelivr.net/gh/gitbrent/bootstrap4-toggle@3.6.1/css/bootstrap4-toggle.min.css" rel="stylesheet"> -->
<!-- End Boostrap toggle min CSS -->
<script src="assets/js/font-awesome.js"></script>
</head>
<body>
	<div>
		<nav class="navbar navbar-expand-lg navbar-light bg-light">
			<a class="navbar-brand" href="index.php"><i class="fas fa-plug"></i> Electronic Controller</a>
			<ul class="navbar-nav ml-auto">
				<li class="nav-item">
					<a class="nav-link" href="index.php">WELCOME KEYCODE MON</a>
				</li>
				<li class="nav-item">
					<a class="nav-link" href="#">Logout</a>
				</li>
			</ul>
		</nav>
	</div>
	
	<?php
		
		$m_switchs = new M_Switch();
		$switchs = $m_switchs->getAllData();

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
		</form>
	</div>
	
	<table>
	<tr>
		<td rowspan='5'>
			<img src="assets/images/switch.jpg" alt="switch">
		</td>
	</tr>
			<?php
				for($i=0; $i<4; $i++){
					echo "
				<tr>
					<td>
						<input class='form-check-input' type='checkbox' ".((strpos($switchs[$i]->value, 'on') !== false)?"checked":"")." data-toggle='toggle' data-onstyle='success' data-offstyle='danger' id='switch".($i+1)."' data-size='sm' onchange='updateData(".($i+1).")' >
					</td>
					
				</tr>";
				}
			?>	
	</table>
	
	<!-- Jquery min JS -->
	<script src="assets/js/jquery-3.5.1.min.js"></script>
	<!-- <script src="https://code.jquery.com/jquery-3.5.1.min.js"></script> -->
	<!-- End Jquery min JS -->
	
	<!-- Proper min JS -->
    <script src="assets/js/popper.min.js">
	<!-- <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js" integrity="sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q" crossorigin="anonymous"></script> -->
	<!-- End Proper min JS -->
	

	<!-- Boostrap min JS -->
    <script src="assets/js/bootstrap.min.js"></script>
	<!-- <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js" integrity="sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl" crossorigin="anonymous"></script> -->
	<!-- End Boostrap min JS -->

	<!-- Boostrap toggle min JS -->
	<script src="assets/js/bootstrap4-toggle.min.js"></script>
	<!-- <script src="https://cdn.jsdelivr.net/gh/gitbrent/bootstrap4-toggle@3.6.1/js/bootstrap4-toggle.min.js"></script> -->
	<!-- End Boostrap toggle min JS -->

	<script src="assets/js/main.js"></script>

</body>
</html>