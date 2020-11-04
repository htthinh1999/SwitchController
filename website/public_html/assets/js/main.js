async function updateData(index){
	var changed = false;
	if(document.getElementById('sw'+index+'_on').checked){
		document.getElementById('sw'+index+'_off').checked = true;
		changed = true;
	}
	if(!changed && document.getElementById('sw'+index+'_off').checked)
	{
		document.getElementById('sw'+index+'_on').checked = true;
	}
	await new Promise(r => setTimeout(r, 600));
	document.getElementById('main-form').submit();
}