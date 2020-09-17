<!-- 

function loadSettings()
{
    var cf = document.forms["mainForm"];
    var failed_client_sec = "";
    
    //setKeys(cf.wepenc, cf.passphraseStr, cf.keygen, cf.KEY1, cf.KEY2, cf.KEY3, cf.KEY4);
    //setKeys(cf.wepenc_5g, cf.passphraseStr_5g, cf.keygen_5g, cf.KEY_5g1, cf.KEY_5g2, cf.KEY_5g3, cf.KEY_5g4);
    
    document.getElementById("msg_div").style.display="block";
    
    if("2" == cf.fail_band.value)
    {        
        failed_client_sec = cf.ap_24g_client_sec.value;
        if( ("WPA-AUTO-PSK" == failed_client_sec) || ("WPA-PSK" == failed_client_sec) || ("WPA2-PSK" == failed_client_sec) )
            document.getElementById("wpa_div").style.display="block";
        else if("WEP" == failed_client_sec)
            document.getElementById("wep_div").style.display="block";
        else if("None" == failed_client_sec)
            document.getElementById("msg_div").style.display="none";
    }   
    else if("5" == cf.fail_band.value)
    {
        failed_client_sec = cf.ap_5g_client_sec.value; 
        if( ("WPA-AUTO-PSK" == failed_client_sec) || ("WPA-PSK" == failed_client_sec) || ("WPA2-PSK" == failed_client_sec) )
            document.getElementById("wpa_div_5g").style.display="block";
        else if("WEP" == failed_client_sec)
            document.getElementById("wep_div_5g").style.display="block";
        else if("None" == failed_client_sec)
            document.getElementById("msg_div").style.display="none";
    }        
}

function CheckData()
{
    var success = false;
    var cf = document.forms["mainForm"];
    var failed_client_sec = "";
    if("2" == cf.fail_band.value)
    {
        failed_client_sec = cf.ap_24g_client_sec.value;
        if( ("WPA-AUTO-PSK" == failed_client_sec) || ("WPA-PSK" == failed_client_sec) || ("WPA2-PSK" == failed_client_sec) ) 
            success = CheckData_WPA(cf.passphrase);
        else if("WEP" == failed_client_sec)
            success = CheckData_WEP(cf.wep_enc, cf.wep_key_no, "key", cf.key1, cf.key2, cf.key3, cf.key4);
        else if("None" == failed_client_sec)
            success = true;
    }
    else if("5" == cf.fail_band.value)
    {
        failed_client_sec = cf.ap_5g_client_sec.value; 
        if( ("WPA-AUTO-PSK" == failed_client_sec) || ("WPA-PSK" == failed_client_sec) || ("WPA2-PSK" == failed_client_sec) ) 
            success = CheckData_WPA(cf.passphrase_5g);
        else if("WEP" == failed_client_sec)
            success = CheckData_WEP(cf.wep_enc_5g, cf.wep_key_no_5g, "key5g", cf.key5g1, cf.key5g2, cf.key5g3, cf.key5g4);
        else if("None" == failed_client_sec)
            success = true;
    }
    
    if(success)
        cf.submit();
}

function dfs_alert()
{
    var block_ch_time = "<%432%>";
    var minute = Math.floor(block_ch_time / 60);
    var second = Math.floor(block_ch_time % 60);
    var msg = "<%105%>" + "<%203%>"
        + minute + "<%204%>" 
        + second + "<%205%>";
    alert(msg);
}

function CheckSkip()
{
    var cf = document.forms["mainForm"];

    if("5" == cf.fail_band.value && 
       "1" == "<%433%>")
    {
        dfs_alert();
        cf.skipBt.disabled=true;
    }
    else
    {
        window.location.href='screen5-extender.html';
    }
}



//-->