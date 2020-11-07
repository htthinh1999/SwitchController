async function updateData(index){
	var changed = false;
	if(document.getElementById('sw'+index+'on').checked){
		document.getElementById('sw'+index+'off').checked = true;
		changed = true;
	}
	if(!changed && document.getElementById('sw'+index+'off').checked)
	{
		document.getElementById('sw'+index+'on').checked = true;
	}
	
	var rs = "";
	for(var i=0; i<4; i++){
		rs += "sw" + (i+1) + ((document.getElementById('switch'+(i+1)).checked)?"on":"off");
	}
	document.getElementById('data').value = rs;
	await new Promise(r => setTimeout(r, 600));
	document.getElementById('main-form').submit();
}