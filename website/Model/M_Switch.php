<?php
	include_once("Switch.php");
	
	class M_Switch{
		public $switchs;
		
		public function M_Switch(){
			$this->switchs = array();
		}
		
		public function getAllData(){
			$this->switchs = array();
			$json = file_get_contents('data.json');
			$dts = json_decode($json);
			if($dts!=null){				
				foreach($dts as $dt){
					$switch = new SW($dt->name, $dt->value);
					array_push($this->switchs, $switch);
				}
			}
			return $this->switchs;
		}
		
		public function saveData($datas){
			$fp = fopen('data.json', 'w');
			fwrite($fp, json_encode($datas));
			fclose($fp);
		}
		
	}

?>