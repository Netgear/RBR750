<!-- 

function loadSettings()
{
    var cf = document.forms[0];
    var block_str = "block";
    if(!$.browser.msie)
        block_str = "table-row";
        
    document.getElementById("split_line").style.display = "none";
    setKeys(cf.wepenc, cf.KEY1, cf.KEY2, cf.KEY3, cf.KEY4);
    setKeys(cf.wepenc_5g, cf.KEY_5g1, cf.KEY_5g2, cf.KEY_5g3, cf.KEY_5g4);
    
    //set ssid row display
    if("2" == cf.mode.value)
    {
        document.getElementById("ssid_24g_row").style.display = block_str;
        document.getElementById("ssid_5g_row").style.display = block_str;
    }
    else if("1" == cf.mode.value)
    {
        if("2.4G" == cf.ap_band.value)
        {
            document.getElementById("ssid_5g_row").style.display = "none";
            document.getElementById("ssid_24g_row").style.display = block_str;
        }
        else if("5G" == cf.ap_band.value)
        {
            document.getElementById("ssid_24g_row").style.display = "none";
            document.getElementById("ssid_5g_row").style.display = block_str;
        }
    }
    
    //set ether-bind row display
    //20120215: Don't set ether-bind row display in wizard
    if(("2" == cf.mode.value) && ("both" == cf.sta_band.value))
    {
        //document.getElementById("ether_port_row").style.display = block_str;
        document.getElementById("ether_port_row").style.display = "none";
        //if ("2.4G" == cf.eth_bind_band.value)
        //    cf.ether_port[0].checked = true;
        //else
        //    cf.ether_port[1].checked = true;
    }
    else
    {
        document.getElementById("ether_port_row").style.display = "none";
        if((("1" == cf.mode.value) && ("5G" == cf.ap_band.value)) || 
                (("1" == cf.mode.value) && ("2.4G" == cf.ap_band.value) && ("2.4G" == cf.sta_band.value)) || 
                (("2" == cf.mode.value) && ("2.4G" == cf.sta_band.value)))
        {
            cf.eth_bind_band.value = "2.4G";
        }
        else if((("1" == cf.mode.value) && ("2.4G" == cf.ap_band.value) && ("5G" == cf.sta_band.value)) || 
                (("2" == cf.mode.value) && ("5G" == cf.sta_band.value)))
        {
            cf.eth_bind_band.value = "5G";
        }
    }
    
    
    //set 2.4g or 5g div display
    if("1" == cf.wl_same_sec.value)
    {
        cf.use_same_sec.checked = true;
        document.getElementById("24g_div").style.display="none";
        document.getElementById("5g_div").style.display="none";
    }
    else
    {
        cf.use_same_sec.checked = false;
        if("2" == cf.mode.value)
        {
            show_24g_div();
            show_5g_div();
            document.getElementById("split_line").style.display = block_str;
        }
        else if("1" == cf.mode.value)
        {
            if("2.4G" == cf.ap_band.value)
                show_24g_div();
            else if("5G" == cf.ap_band.value)
                show_5g_div();
        }
    }  
}
function show_24g_div()
{
    var cf = document.forms[0];
    var secu_type_24g=cf.ap_24g_sec.value;
    
    document.getElementById("24g_div").style.display="block";
    document.getElementById("24g_sec_opt").style.display="block";
    
    if("None" == secu_type_24g) // None
    {
        document.getElementById("24g_wep").style.display="none";
        document.getElementById("24g_wpa").style.display="none";
    }
    else if ("WEP" == secu_type_24g) // WEP
    {
        document.getElementById("24g_wep").style.display="block";
        document.getElementById("24g_wpa").style.display="none";
    }
    else if ("WPA-PSK" == secu_type_24g || "WPA2-PSK" == secu_type_24g || "WPA-AUTO-PSK" == secu_type_24g)
    {
        document.getElementById("24g_wep").style.display="none";
        document.getElementById("24g_wpa").style.display="block";
        
        if("WPA-AUTO-PSK" == secu_type_24g)
            $("#24g_wpa_str").text("(WPA-PSK + WPA2-PSK)");
        else if("WPA-PSK" == secu_type_24g)
            $("#24g_wpa_str").text("(WPA-PSK)");
        else if("WPA2-PSK" == secu_type_24g)
            $("#24g_wpa_str").text("(WPA2-PSK)");
    }
}

function show_5g_div()
{
    var cf = document.forms[0];
    var secu_type_5g=cf.ap_5g_sec.value;
    
    document.getElementById("5g_div").style.display="block";
    document.getElementById("5g_sec_opt").style.display="block";
    
    if("None" == secu_type_5g) // None
    {
        document.getElementById("5g_wep").style.display="none";
        document.getElementById("5g_wpa").style.display="none";
    }
    else if ("WEP" == secu_type_5g) // WEP
    {
        document.getElementById("5g_wep").style.display="block";
        document.getElementById("5g_wpa").style.display="none";
    }
    else if ("WPA-PSK" == secu_type_5g || "WPA2-PSK" == secu_type_5g || "WPA-AUTO-PSK" == secu_type_5g)
    {
        document.getElementById("5g_wep").style.display="none";
        document.getElementById("5g_wpa").style.display="block";
        
        if("WPA-AUTO-PSK" == secu_type_5g)
            $("#5g_wpa_str").text("(WPA-PSK + WPA2-PSK)");
        else if("WPA-PSK" == secu_type_5g)
            $("#5g_wpa_str").text("(WPA-PSK)");
        else if("WPA2-PSK" == secu_type_5g)
            $("#5g_wpa_str").text("(WPA2-PSK)");
    }
}

function select_ether_band()
{
    var cf = document.forms[0];
    var ether_ports = cf.ether_port;
    if(null != ether_ports)
    {
        if(ether_ports[0].checked)
            cf.eth_bind_band.value = "2.4G";
        else if(ether_ports[1].checked)
            cf.eth_bind_band.value = "5G";
    }
}

function use_ornot_same_sec()
{
    var cf = document.forms[0];
    var block_str = "block";
    if(!$.browser.msie)
        block_str = "table-row";
        
    if(!cf.use_same_sec.checked)
    {                    
        if("2" == cf.mode.value)
        {
            show_24g_div();
            show_5g_div();
            document.getElementById("split_line").style.display = block_str;
        }
        else if("1" == cf.mode.value)
        {
            if("2.4G" == cf.ap_band.value)
                show_24g_div();
            else if("5G" == cf.ap_band.value)
                show_5g_div();
        }

    }
    else if(cf.use_same_sec.checked)
    {
        document.getElementById("24g_div").style.display = "none";
        document.getElementById("5g_div").style.display = "none";
        document.getElementById("split_line").style.display = "none";
    }
}

function change_sec(radio_name, sec_type)
{
    var cf = document.forms[0];
    if("security_type" == radio_name)
    {
        if("None" == sec_type)
        {
            document.getElementById("24g_wep").style.display = "none";
            document.getElementById("24g_wpa").style.display = "none";
        }
        else if( ("WPA-AUTO-PSK" == sec_type) || ("WPA-PSK" == sec_type) || ("WPA2-PSK" == sec_type) )
        {
            document.getElementById("24g_wep").style.display = "none";
            document.getElementById("24g_wpa").style.display = "block";
            
            if("WPA-AUTO-PSK" == sec_type)
                $("#24g_wpa_str").text("(WPA-PSK + WPA2-PSK)");
            else if("WPA-PSK" == sec_type)
                $("#24g_wpa_str").text("(WPA-PSK)");
            else if("WPA2-PSK" == sec_type)
                $("#24g_wpa_str").text("(WPA2-PSK)");
        }
        else if("WEP" == sec_type)
        {
            document.getElementById("24g_wpa").style.display = "none";
            document.getElementById("24g_wep").style.display = "block";
        }
        cf.ap_24g_sec.value = sec_type;
    }
    else if("security_type_5g" == radio_name)
    {
        if("None" == sec_type)
        {
            document.getElementById("5g_wep").style.display = "none";
            document.getElementById("5g_wpa").style.display = "none";
        }
        else if( ("WPA-AUTO-PSK" == sec_type) || ("WPA-PSK" == sec_type) || ("WPA2-PSK" == sec_type) )
        {
            document.getElementById("5g_wep").style.display = "none";
            document.getElementById("5g_wpa").style.display = "block";
            
            if("WPA-AUTO-PSK" == sec_type)
                $("#5g_wpa_str").text("(WPA-PSK + WPA2-PSK)");
            else if("WPA-PSK" == sec_type)
                $("#5g_wpa_str").text("(WPA-PSK)");
            else if("WPA2-PSK" == sec_type)
                $("#5g_wpa_str").text("(WPA2-PSK)");
        }
        else if("WEP" == sec_type)
        {
            document.getElementById("5g_wpa").style.display = "none";
            document.getElementById("5g_wep").style.display = "block";
        }
        cf.ap_5g_sec.value = sec_type;
    }
}

function CheckData()
{
    var cf = document.forms[0];
    var success_24g = false;
    var success_5g = false;
    var success = false;
    
    if("2" == cf.mode.value)
    {
        if(cf.ssid.value==cf.ssid_5g.value)
        {
            alert("<%2622%>");
            return false;
        }
        if (cf.ssid.value == "")
        {
            alert("<%53%>");
            return false;
        }
    
        if (cf.ssid.value.match( /[^\x20-\x7E]/ ))
        {
            alert("<%54%>");
            return false;
        }
        if (cf.ssid_5g.value == "")
        {
            alert("<%53%>");
            return false;
        }
    
        if (cf.ssid_5g.value.match( /[^\x20-\x7E]/ ))
        {
            alert("<%54%>");
            return false;
        }   
    }
    else if("1" == cf.mode.value && "5G" == cf.ap_band.value)
    {
        if (cf.ssid_5g.value == "")
        {
            alert("<%53%>");
            return false;
        }
    
        if (cf.ssid_5g.value.match( /[^\x20-\x7E]/ ))
        {
            alert("<%54%>");
            return false;
        }   
    }
    else if("1" == cf.mode.value && "2.4G" == cf.ap_band.value)
    {
        if (cf.ssid.value == "")
        {
            alert("<%53%>");
            return false;
        }
    
        if (cf.ssid.value.match( /[^\x20-\x7E]/ ))
        {
            alert("<%54%>");
            return false;
        }
    }
    if (cf.mode.value == "1")
    {
        if(cf.sta_band.value == "5G") 
        {
            if (cf.ap_24g_sec.value == "WPA-PSK")
            {
                alert("<%43%>");
            }
            else if (cf.ap_24g_sec.value == "WPA-AUTO-PSK")
            {
                alert("<%46%>");
            }    
        }
        else 
        {
            if (cf.ap_5g_sec.value == "WPA-PSK")
            {
                alert("<%43%>");
            }
            else if (cf.ap_5g_sec.value == "WPA-AUTO-PSK")
            {
                alert("<%46%>");
            }    
        }
    }
    else
    {
        if ((cf.ap_24g_sec.value == "WPA-PSK" && cf.ap_5g_sec.value == "WPA-AUTO-PSK") || 
            (cf.ap_24g_sec.value == "WPA-AUTO-PSK" && cf.ap_5g_sec.value == "WPA-PSK") )
            alert("<%43%>");
        else if(cf.ap_24g_sec.value == "WPA-PSK" || cf.ap_5g_sec.value == "WPA-PSK")
            alert("<%43%>");
        else if(cf.ap_24g_sec.value == "WPA-AUTO-PSK" || cf.ap_5g_sec.value == "WPA-AUTO-PSK")
            alert("<%46%>");
    }
    
    if(cf.use_same_sec.checked)
    {
        cf.wl_same_sec.value = "1";
        cf.submit();
    }
    else
    {
        cf.wl_same_sec.value = "0";
        if("2" == cf.mode.value)
        {
            //if(cf.ssid.value==cf.ssid_5g.value)
            //{
            //    alert("2.4G and 5G have the same SSID");
            //    return false;
            //}    
            
            success_24g = CheckData24G();
            success_5g = CheckData5G();
            success = success_24g && success_5g;
        }
        else if("1" == cf.mode.value && "5G" == cf.ap_band.value)
        {
            success_5g = CheckData5G();
            success = success_5g;  
        }
        else if("1" == cf.mode.value && "2.4G" == cf.ap_band.value)
        {
            success_24g = CheckData24G();
            success = success_24g;
        }



        
        if(success)
            cf.submit();
    }
}
function CheckData24G()
{
    var cf = document.forms[0];
    
    if (cf.ssid.value == "")
    {
        alert("<%53%>");
        return false;
    }

    if (cf.ssid.value.match( /[^\x20-\x7E]/ ))
    {
        alert("<%54%>");
        return false;
    }
    
    if("None" == cf.ap_24g_sec.value)
        return CheckData_NONE();
    else if( ("WPA-AUTO-PSK" == cf.ap_24g_sec.value) || ("WPA-PSK" == cf.ap_24g_sec.value) || ("WPA2-PSK" == cf.ap_24g_sec.value) ) 
        return CheckData_WPA(cf.passphrase, cf.ap_24g_sec.value);
    else if("WEP" == cf.ap_24g_sec.value)
        return CheckData_WEP(cf.wepenc, cf.wep_key_no, "KEY", cf.KEY1, cf.KEY2, cf.KEY3, cf.KEY4);
}

function CheckData5G()
{
    var cf = document.forms[0];
    
    if (cf.ssid_5g.value == "")
    {
        alert("<%53%>");
        return false;
    }

    if (cf.ssid_5g.value.match( /[^\x20-\x7E]/ ))
    {
        alert("<%54%>");
        return false;
    }
    
    if("None" == cf.ap_5g_sec.value)
        return CheckData_NONE();
    if( ("WPA-AUTO-PSK" == cf.ap_5g_sec.value) || ("WPA-PSK" == cf.ap_5g_sec.value) || ("WPA2-PSK" == cf.ap_5g_sec.value) ) 
        return CheckData_WPA(cf.passphrase_5g, cf.ap_5g_sec.value);
    else if("WEP" == cf.ap_5g_sec.value)
        return CheckData_WEP(cf.wepenc_5g, cf.wep_key_no_5g, "KEY_5g", cf.KEY_5g1, cf.KEY_5g2, cf.KEY_5g3, cf.KEY_5g4);
}

//-->