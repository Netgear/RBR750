
function save_orbi_info()
{
	
    var cf = document.forms["enablePageForm"];
	return true;
}

function check_capture_packets(action)
{
	var cf2 = document.forms["debugLogForm"];
	
	if(cf2.capture_in_process.value == "1")
	{
		if(action=="push") {
			return;
		} else {
			$('#action_Start_Capture').removeClass('primary');
			$('#action_Start_Capture').addClass('disabled');	
			$('#action_Start_Capture').disabled = true;
			$('#Save_Debug_File').removeClass('disabled');
			$('#Save_Debug_File').addClass('primary');
			document.getElementById('txt_capture_in_progress').style.display = "block";	
		}
	}
	else
	{			
		if(action=="push") {
			$('#action_Start_Capture').removeClass('primary');
			$('#action_Start_Capture').addClass('disabled');
			cf2.capture_in_process.value = "2";
			document.forms['debugLogForm'].submit();
		} else {
			$('#action_Start_Capture').removeClass('disabled');
			$('#action_Start_Capture').addClass('primary');		
			$('#action_Start_Capture').disabled = false;	
			$('#Save_Debug_File').removeClass('primary');
			$('#Save_Debug_File').addClass('disabled');
			document.getElementById('txt_capture_in_progress').style.display = "none";	
		}
	}
	return;
}

function save_debug_log()
{
	if ($('#Save_Debug_File').hasClass("primary"))
	{
    var cf2 = document.forms["debugLogForm"];
	$('#action_Start_Capture').removeClass('disabled');
	$('#action_Start_Capture').addClass('primary');		
	$('#action_Start_Capture').disabled = false;	
	document.getElementById('txt_capture_in_progress').style.display = "none";
    cf2.capture_in_process.value = "0";
    
		$('#Save_Debug_File').removeClass('primary');
		$('#Save_Debug_File').addClass('disabled');

		window.location.href = 'Debug_log.zip';
}
}

function check_wlan_debug()
{	
    var cf = document.forms["ADVWLANForm"];
	
	if(cf.enable_lbd.checked == true)
	{		
		cf.enable_band_steering.checked = true;		
		cf.enable_multi_ap_steering.checked = true;		
	}
	else
	{
		cf.enable_band_steering.checked = false;		
		cf.enable_multi_ap_steering.checked = false;		
	}	
}

