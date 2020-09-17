
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

function loadSettings()
{
	
    var cf = document.forms["ADVWLANForm"];
	var cf2 = document.forms["debugLogForm"];
	var cf3 = document.forms["telnetEnableForm"];
	var cf5 = document.forms["rstpEnableForm"];
	var cf4 = document.forms["CMScanEnableForm"];
	
	check_capture_packets("load");
	
	if(cf2.Start_When_Boot_Up.value == "1")
		cf2.action_Start_When_Boot_Up.checked = true;
	else
		cf2.action_Start_When_Boot_Up.checked = false;
	
	
	if(cf2.Enable_LANWAN_Packet_Capture.value == "1")
		cf2.action_Enable_LANWAN_Packet_Capture.checked = true;
	else
		cf2.action_Enable_LANWAN_Packet_Capture.checked = false;
		
    /*
	if(cf2.Enable_Roaming_Debug.value == "1")
		cf2.action_Enable_Roaming_Debug.checked = true;
	else
		cf2.action_Enable_Roaming_Debug.checked = false;
    */
		
	if(cf2.Enable_Wifi_Debug.value == "1")
		cf2.action_Enable_Wifi_Debug.checked = true;
	else
		cf2.action_Enable_Wifi_Debug.checked = false;
	
	if(cf3.telnet_enable.value == "1")
		cf3.action_telnet_enable.checked = true;
	else
		cf3.action_telnet_enable.checked = false;
		
       	if(cf5.rstp_enable.value == "1")
		cf5.action_rstp_enable.checked = true;
	else
		cf5.action_rstp_enable.checked = false;
		
	if(cf4.cm_scan_enable.value == "1")
		cf4.action_cm_scan_enable.checked = true;
	else
		cf4.action_cm_scan_enable.checked = false;
	
	
	/*
    if(cf.nvram_enable_band_steering.value == "1")
		cf.enable_band_steering.checked = true;
	else
		cf.enable_band_steering.checked = false;
	
    if(cf.nvram_enable_multi_ap_steering.value == "1")
		cf.enable_multi_ap_steering.checked = true;
	else
		cf.enable_multi_ap_steering.checked = false;		

	cf.max_log_file_size.value = cf.nvram_max_debug_log_file_size.value;
	
	cf.debug_option.value = cf.nvram_debug_option.value;
	*/
	
	return true;
}
