<!-- 

var initRegion;
var changeMsg = "<%15%>";
var notShown = true; // only shown region change message once
var initialized = false;
// "--",Africa,Asia,Australia,Canada,Europe,Israel,Japan,Korea,Mexico,Middle East,South America,USA
var StartChannel = new Array(<%3365%>);
var FinishChannel = new Array(<%3366%>);

/* region define:
 * value    -- region
 * 0        -- Select Region
 * 1        -- Africa
 * 2        -- Asia
 * 3        -- Australia
 * 4        -- Canada
 * 5        -- Europe
 * 6        -- Israel
 * 7        -- Japan
 * 8        -- Korea
 * 9        -- Mexico
 * 10       -- South America
 * 11       -- US -- North America (include Canada)
 * 12       -- Middle East (Saudi Arabia)
 * 13       -- Reserve1 (U12H081_PR treat it as Australia)
 * 14       -- Russia
 * 15       -- Reserve2
 * 16       -- China
 * 17       -- India
 * 18       -- Malaysia
 * 19       -- Middle East (Iran/Lebanon/Qatar)
 * 20       -- Middle East (Turkey/Egypt/Tunisia/Kuwait)
 * 21       -- Middle East (Algeria/Syria/Yemen)
 * 22       -- Middle East (United Arab Emirates)
 * 23       -- Singapore
 * 24       -- Taiwan
 */
var Channel_All_20M = 
    new Array (
    <%3367%>
        );
var Channel_All_40M = 
    new Array (
    <%3368%>
        );
var Channel_All_80M = 
    new Array (
    <%3369%>
        );

var OpMode_BG_TEXT;
var OpMode_20M_TEXT;
var OpMode_40M_TEXT;
var OpMode_5G_BG_TEXT;
var OpMode_5G_20M_TEXT;
var OpMode_5G_40M_TEXT;
var OpMode_5G_80M_TEXT;
var OpMode_BG_VALUE;
var OpMode_20M_VALUE;
var OpMode_40M_VALUE;
var OpMode_5G_BG_VALUE;
var OpMode_5G_20M_VALUE;
var OpMode_5G_40M_VALUE;
var OpMode_5G_80M_VALUE;

var selected2=100,selected3=100, ch_selected=100;

var curr_region = "<%20%>"
var curr_channel_5g_value = "<%3327%>"
var curr_channel_2g_value = "<%1941%>"


var chgCh_cnt = 0;  

var block_str = "block";
var show_wep_or_wps_msg = false;
var wps_version = "<%49%>";  
var show_tkip_or_wps_v20_msg = false;
var show_ssid_bc_or_wps_v20_msg = false;
var show_open_or_wps_v20_msg = false;

//var wla_temp_mode_2G;

$(document).ready(function()
{
//        $('.scroll-pane').jScrollPane('scrollbarMargin:5px');
    $("#target").submit(function() {
        buttonFilter();
    });

});

function setDivDisable(setDiv,flag)
{
    if(1 == flag)
        setDiv.find("*").attr("disabled", "true");
    else
        setDiv.find("*").attr("disabled", "");
}
function loadSettings()
{
    highLightMenu('setup_header', 'wireless_set');
    
    var cf = document.forms[0];
    var i;
    //var block_str = "block";
    //if(!(("2" == cf.mode.value) && (("2.4G" == cf.sta_band.value) || ("both" == cf.sta_band.value))))
    //{
        OpMode_BG_TEXT=cf.opmode.options[0].text; 
        OpMode_20M_TEXT=cf.opmode.options[1].text; 
        OpMode_40M_TEXT=cf.opmode.options[2].text; 
        OpMode_BG_VALUE=cf.opmode.options[0].value;
        OpMode_20M_VALUE=cf.opmode.options[1].value;
        OpMode_40M_VALUE=cf.opmode.options[2].value;
    //}
    //if(!(("2" == cf.mode.value) && (("5G" == cf.sta_band.value) || ("both" == cf.sta_band.value))))
    //{
        OpMode_5G_BG_TEXT=cf.opmode_5g.options[0].text; 
        OpMode_5G_20M_TEXT=cf.opmode_5g.options[1].text; 
        OpMode_5G_40M_TEXT=cf.opmode_5g.options[2].text; 
        OpMode_5G_80M_TEXT=cf.opmode_5g.options[3].text; 
        OpMode_5G_BG_VALUE=cf.opmode_5g.options[0].value;
        OpMode_5G_20M_VALUE=cf.opmode_5g.options[1].value;
        OpMode_5G_40M_VALUE=cf.opmode_5g.options[2].value;
        OpMode_5G_80M_VALUE=cf.opmode_5g.options[3].value;
        //wla_temp_mode_2G = cf.opmode.selectedIndex;
    //}
    
    if(!$.browser.msie)
        block_str = "table-row";
    
    if(cf.tempSetting.value == 1)
    {
        var temp_region = parseInt(cf.tempRegion.value);
        if (cf.tempRegion.value == "")
        {
            if (parseInt(cf.setRegion.value) == 12)
                cf.WRegion.selectedIndex=10;
            else if (parseInt(cf.setRegion.value) >= 10 && parseInt(cf.setRegion.value) <= 11)
                cf.WRegion.selectedIndex=parseInt(cf.setRegion.value)+1;
            else
                cf.WRegion.selectedIndex=parseInt(cf.setRegion.value);
        }
        else if (temp_region == 12)
            cf.WRegion.selectedIndex=10;
        else if (temp_region >= 10 && temp_region <= 11)
            cf.WRegion.selectedIndex=temp_region+1;
        else
            cf.WRegion.selectedIndex=temp_region;
    }
        
    initRegion = cf.WRegion.selectedIndex;  
		
    setOpMode();
    setChannel();	
    	
    setKeys(cf.wepenc, cf.KEY1, cf.KEY2, cf.KEY3, cf.KEY4);
    setKeys(cf.wepenc_5g, cf.KEY_5g1, cf.KEY_5g2, cf.KEY_5g3, cf.KEY_5g4);	
    
    if ("1" == cf.enable_ap_mode.value)
    {
        document.getElementById("24g_network").style.display="block";
        document.getElementById("5g_network").style.display="block";
        document.getElementById("ether_port_row").style.display = "none";
        show_24g_div();
        show_5g_div();

        for (i=0; i<cf.w_channel.options.length; i++)
        {
            if (cf.w_channel.options[i].value == cf.initChannel.value)
            {
                cf.w_channel[i].selected = true;
                break;
            }
        }
        for (i=0; i<cf.w_channel_5g.options.length; i++)
        {
            if (cf.w_channel_5g.options[i].value == cf.initChannel_5g.value)
            {
                cf.w_channel_5g[i].selected = true;
                break;
            }
        }
    }
    else if("2" == cf.mode.value)
    {  
        document.getElementById("24g_network").style.display="block";
        document.getElementById("5g_network").style.display="block";
        if("both" == cf.sta_band.value)
            document.getElementById("ether_port_row").style.display = block_str;
        else
            document.getElementById("ether_port_row").style.display = "none";
        //document.getElementById("ssid_24g_row").style.display = block_str;
        //document.getElementById("ssid_5g_row").style.display = block_str;
		
        if("2.4G" == cf.sta_band.value)
        {
            //document.getElementById("channel_24g_row").style.display = "none";
            //document.getElementById("channel_24g_row").style.display = block_str;
            //document.getElementById("channel_5g_row").style.display = block_str;
            //document.getElementById("opmode_24g_row").style.display = "none";
            //document.getElementById("opmode_24g_row").style.display = block_str;
            //document.getElementById("opmode_5g_row").style.display = block_str; 
            cf.w_channel.disabled = true;
            cf.opmode.disabled = true;
            //if(("WEP" == cf.ap_24g_sec.value) || ("WPA-PSK" == cf.ap_24g_sec.value))
            //    cf.opmode.disabled = true;
            //else 
            //    cf.opmode.disabled = false;
            
        }
        else if("5G" == cf.sta_band.value)
        {
            //document.getElementById("channel_24g_row").style.display = block_str;
            //document.getElementById("channel_5g_row").style.display = "none";
            //document.getElementById("channel_5g_row").style.display = block_str
            //document.getElementById("opmode_24g_row").style.display = block_str;
            //document.getElementById("opmode_5g_row").style.display = "none"; 
            //document.getElementById("opmode_5g_row").style.display = block_str 
            cf.w_channel_5g.disabled = true;
            cf.opmode_5g.disabled = true;
        }
        else if("both" == cf.sta_band.value)
        {
            //document.getElementById("channel_24g_row").style.display = "none";
            //document.getElementById("channel_5g_row").style.display = "none";
            //document.getElementById("opmode_24g_row").style.display = "none";
            //document.getElementById("opmode_5g_row").style.display = "none"; 
            //document.getElementById("channel_24g_row").style.display = block_str;
            //document.getElementById("channel_5g_row").style.display = block_str;
            //document.getElementById("opmode_24g_row").style.display = block_str;
            //document.getElementById("opmode_5g_row").style.display = block_str;
            cf.w_channel.disabled = true;
            cf.w_channel_5g.disabled = true;
            cf.opmode_5g.disabled = true;
            cf.opmode.disabled = true;
            //if(("WEP" == cf.ap_24g_sec.value) || ("WPA-PSK" == cf.ap_24g_sec.value))
            //    cf.opmode.disabled = true;
            //else 
            //    cf.opmode.disabled = false;
        }
        
        
        if ("2.4G" == cf.eth_bind_band.value)
            cf.ether_port[0].checked = true;
        else
            cf.ether_port[1].checked = true;
            
        show_24g_div();
        show_5g_div();

        for (i=0; i<cf.w_channel.options.length; i++)
        {
            if (cf.w_channel.options[i].value == curr_channel_2g_value)
            {
                cf.w_channel[i].selected = true;
                break;
            }
        }


        for (i=0; i<cf.w_channel_5g.options.length; i++)
        {
            if (cf.w_channel_5g.options[i].value == curr_channel_5g_value)
            {
                cf.w_channel_5g[i].selected = true;
                break;
            }
        }
        
    }
    else if((("1" == cf.mode.value) && ("2.4G" == cf.ap_band.value)) || ("0" == cf.mode.value))
    {
        document.getElementById("24g_network").style.display="block";
        document.getElementById("5g_network").style.display="none";
        document.getElementById("ether_port_row").style.display = "none";
        document.getElementById("split_line").style.display = "none";
        document.getElementById("split_line5g").style.display = "none";
        //document.getElementById("ssid_5g_row").style.display = "none";
        //document.getElementById("ssid_24g_row").style.display = block_str;
        //document.getElementById("channel_24g_row").style.display = block_str;
        //document.getElementById("channel_5g_row").style.display = "none";
        //document.getElementById("opmode_24g_row").style.display = block_str;
        //document.getElementById("opmode_5g_row").style.display = "none";
        cf.eth_bind_band.value = "2.4G";
        show_24g_div();   

        for (i=0; i<cf.w_channel.options.length; i++)
        {
            if (cf.w_channel.options[i].value == cf.initChannel.value)
            {
                cf.w_channel[i].selected = true;
                break;
            }
        }
    } 
    else if(("1" == cf.mode.value) && ("5G" == cf.ap_band.value))
    {
        document.getElementById("24g_network").style.display="none";
        document.getElementById("5g_network").style.display="block";
        document.getElementById("ether_port_row").style.display = "none";
        document.getElementById("split_line").style.display = "none";
        document.getElementById("split_line24g").style.display = "none";
        //document.getElementById("ssid_24g_row").style.display = "none";
        //document.getElementById("ssid_5g_row").style.display = block_str;
        //document.getElementById("channel_24g_row").style.display = "none";
        //document.getElementById("channel_5g_row").style.display = block_str;
        //document.getElementById("opmode_24g_row").style.display = "none";
        //document.getElementById("opmode_5g_row").style.display = block_str;
        cf.eth_bind_band.value = "5G";
        show_5g_div();

        for (i=0; i<cf.w_channel_5g.options.length; i++)
        {
            if (cf.w_channel_5g.options[i].value == cf.initChannel_5g.value)
            {
                cf.w_channel_5g[i].selected = true;
                break;
            }
        }
    }
    displayWepWPARadio();     
    if (cf.WRegion.options[cf.WRegion.selectedIndex].value == "21")
    {
        setDivDisable($("#5g_network"), 1);
        setDivDisable($("#5g_div"), 1);
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
        else if("WEP" == sec_type)
        {
            document.getElementById("24g_wpa").style.display = "none";
            document.getElementById("24g_wep").style.display = "block";
        }
        else if("WPA-PSK" == sec_type)
        {
            //If the interface is psr mode, the mode should be Auto and grey out
            //if(!(("2" == cf.mode.value) && (("2.4G" == cf.sta_band.value) || ("both" == cf.sta_band.value))))
            //{
                if (cf.opmode.selectedIndex != 0)
                {
                    /* we add a redundant assignment here so that the 
                       following warning alert wouldn't appear twice */
                    cf.opmode.selectedIndex = 0;
                    alert("<%43%>");
                    cf.opmode.selectedIndex = 0;
                }
            //}
            document.getElementById("24g_wep").style.display = "none";
            document.getElementById("24g_wpa").style.display = "block";            
            $("#24g_wpa_str").text("(WPA-PSK)");
        }
        else if("WPA2-PSK" == sec_type)
        {
            document.getElementById("24g_wep").style.display = "none";
            document.getElementById("24g_wpa").style.display = "block";
            $("#24g_wpa_str").text("(WPA2-PSK)");
        }
        else if("WPA-AUTO-PSK" == sec_type)
        {
            //if(!(("2" == cf.mode.value) && (("2.4G" == cf.sta_band.value) || ("both" == cf.sta_band.value))))
            //{
                if ((cf.opmode.selectedIndex != 0) && (cf.opmode.selectedIndex!=selected2))
                {
                    selected2=cf.opmode.selectedIndex;
                    alert("<%43%>");
                }
            //}
            document.getElementById("24g_wep").style.display = "none";
            document.getElementById("24g_wpa").style.display = "block";
            $("#24g_wpa_str").text("(WPA-PSK + WPA2-PSK)");
        }
        cf.ap_24g_sec.value = sec_type;
        //if(("2" == cf.mode.value) && (("2.4G" == cf.sta_band.value) || ("both" == cf.sta_band.value)))
        //{
        //    setOpMode_24g_psr();
        //} 
    }
    else if("security_type_5g" == radio_name)
    {
        if("None" == sec_type)
        {
            document.getElementById("5g_wep").style.display = "none";
            document.getElementById("5g_wpa").style.display = "none";
        }        
        else if("WEP" == sec_type)
        {
            document.getElementById("5g_wpa").style.display = "none";
            document.getElementById("5g_wep").style.display = "block";
        }
        else if("WPA-PSK" == sec_type)
        {
            //If the interface is psr mode, the mode should be Auto and grey out
            //if(!(("2" == cf.mode.value) && (("2.4G" == cf.sta_band.value) || ("both" == cf.sta_band.value))))
            //{
                if (cf.opmode_5g.selectedIndex != 0)
                {
                    /* we add a redundant assignment here so that the 
                       following warning alert wouldn't appear twice */
                    cf.opmode_5g.selectedIndex = 0;
                    alert("<%43%>");
                    setChannel();
                    cf.opmode_5g.selectedIndex = 0;
                }
            //}
            document.getElementById("5g_wep").style.display = "none";
            document.getElementById("5g_wpa").style.display = "block";
            
            $("#5g_wpa_str").text("(WPA-PSK)");
        }
        else if("WPA2-PSK" == sec_type)
        {
            document.getElementById("5g_wep").style.display = "none";
            document.getElementById("5g_wpa").style.display = "block";

            $("#5g_wpa_str").text("(WPA2-PSK)");
        }
        else if("WPA-AUTO-PSK" == sec_type)
        {
            //If the interface is psr mode, the mode should be Auto and grey out
            //if(!(("2" == cf.mode.value) && (("2.4G" == cf.sta_band.value) || ("both" == cf.sta_band.value))))
            //{
                if ((cf.opmode_5g.selectedIndex != 0) && (cf.opmode_5g.selectedIndex!=selected3))
                {
                    selected3=cf.opmode_5g.selectedIndex;
                    alert("<%46%>");
                }
            //}
            document.getElementById("5g_wep").style.display = "none";
            document.getElementById("5g_wpa").style.display = "block";
            
            $("#5g_wpa_str").text("(WPA-PSK + WPA2-PSK)");
        }
        cf.ap_5g_sec.value = sec_type;
               
    }
    
    setOpMode();
        
}

function CheckData()
{
    var cf = document.forms[0];
    var success_24g = false;
    var success_5g = false;
    var success = false;
    
    cf.wl_same_sec.value = "0";
    if(("1" == cf.enable_ap_mode.value) || ("2" == cf.mode.value))
    {
        if(cf.ssid.value==cf.ssid_5g.value)
        {
            alert("<%2595%>");
            return false;
        }    
        
        success_24g = CheckData24G();
        if(!success_24g)
            return false;
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
    else if("0" == cf.mode.value)
    {
        success_24g = CheckData24G();
        success = success_24g;
    }
    
    if(needGrayOutWPS)
        WPS_wizard_grayout();
    else
        WPS_wizard_ungrayout();
    if(success)
        cf.submit();
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
 
    
    if(!checkRegion()) return false;
    
    //if ((cf.opmode.selectedIndex == 2) && (cf.opmode.selectedIndex != wla_temp_mode_2G))
   // {
   //     alert("get_warning_message");
   // }
    
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
    
    if (cf.WRegion.options[cf.WRegion.selectedIndex].value == "21")
        return true;
           
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
    
    if(!checkRegion()) return false;
    
    if("None" == cf.ap_5g_sec.value)
        return CheckData_NONE();
    if( ("WPA-AUTO-PSK" == cf.ap_5g_sec.value) || ("WPA-PSK" == cf.ap_5g_sec.value) || ("WPA2-PSK" == cf.ap_5g_sec.value) ) 
        return CheckData_WPA(cf.passphrase_5g, cf.ap_5g_sec.value);
    else if("WEP" == cf.ap_5g_sec.value)
        return CheckData_WEP(cf.wepenc_5g, cf.wep_key_no_5g, "KEY_5g", cf.KEY_5g1, cf.KEY_5g2, cf.KEY_5g3, cf.KEY_5g4);
}

function setChannel()
{
    var cf = document.forms[0];
    if ("1" == cf.enable_ap_mode.value)
    {
        setChannel_24g();
        setChannel_5g();
    }
    else if((("1" == cf.mode.value) && ("2.4G" == cf.ap_band.value)) || ("0" == cf.mode.value))
    {
        setChannel_24g();
    } 
    else if(("1" == cf.mode.value) && ("5G" == cf.ap_band.value))
    {
        setChannel_5g();
    } 
    else if(("2" == cf.mode.value) && ("2.4G" == cf.sta_band.value))
    {
        setAutoChannel_24g();
        setChannel_5g();
    } 
    else if(("2" == cf.mode.value) && ("5G" == cf.sta_band.value))
    {
        setChannel_24g();
        setAutoChannel_5g();
    } 
    else//both case
    {
        setAutoChannel_24g();
        setAutoChannel_5g();
    }
}

//If the interface is psr mode, the channel should be Auto and grey out
function setAutoChannel_24g()
{
    var cf = document.forms[0];
    cf.w_channel.options[0].text = "<%40%>";
    cf.w_channel.options[0].value = 0;
}

function setAutoChannel_5g()
{
    var cf = document.forms[0];
    cf.w_channel_5g.options[0].text = "<%40%>";
    cf.w_channel_5g.options[0].value = 0;
}


function setChannel_24g()
{
    var cf = document.forms[0];
    var index = cf.WRegion.selectedIndex;
    var chIndex = cf.w_channel.selectedIndex;

    var b1=0, b2=0, b3=0, b4=0;
    
    if (FinishChannel[index]==14 && cf.opmode.selectedIndex!=0)
        cf.w_channel.options.length = FinishChannel[index] - StartChannel[index];
    else
        cf.w_channel.options.length = FinishChannel[index] - StartChannel[index] + 2;

    cf.w_channel.options[0].text = "<%40%>";
    cf.w_channel.options[0].value = 0;

    for (var i = StartChannel[index]; i <= FinishChannel[index]; i++) {
        if (i==14 && cf.opmode.selectedIndex!=0)
            continue;
        cf.w_channel.options[i - StartChannel[index] + 1].value = i;
        cf.w_channel.options[i - StartChannel[index] + 1].text = ((i < 10)? "0" + i : i);
    }
    cf.w_channel.selectedIndex = ((chIndex > -1) && (chIndex < cf.w_channel.options.length)) ? chIndex : 0 ;        
}

function setChannel_5g()
{
    var cf = document.forms[0];
    var ch_list = cf.w_channel_5g;
    //var index = cf.WRegion.options[cf.WRegion.selectedIndex].value;
    var index = cf.WRegion.selectedIndex;
    var currChVal_5g = "<%3362%>";
    var len=0, i=0;

    ch_list.options.length = 100;
    if (cf.opmode_5g.selectedIndex == 3)    // 40MHz
    {
        if (Channel_All_80M[index].length > 1)
        {
            if (Channel_All_80M[index][0] == 0)
            {
                ch_list.options[len].text = "<%40%>";
                ch_list.options[len].value = Channel_All_80M[index][0];
                len++;
            }
            for (i=1; i<Channel_All_80M[index].length; i++)
            {
                ch_list.options[len].text = Channel_All_80M[index][i];
                ch_list.options[len].value = Channel_All_80M[index][i];
                len++;
            }
            if (curr_region != cf.WRegion.value)
                currChVal_5g = Channel_All_80M[index][0];
        }
    }
    else if (cf.opmode_5g.selectedIndex == 2)    // 40MHz
    {
        if (Channel_All_40M[index].length > 1)
        {
            if (Channel_All_40M[index][0] == 0)
            {
                ch_list.options[len].text = "<%40%>";
                ch_list.options[len].value = Channel_All_40M[index][0];
                len++;
            }
            for (i=1; i<Channel_All_40M[index].length; i++)
            {
                ch_list.options[len].text = Channel_All_40M[index][i];
                ch_list.options[len].value = Channel_All_40M[index][i];
                len++;
            }
            if (curr_region != cf.WRegion.value)
                currChVal_5g = Channel_All_40M[index][0];
        }
    }
    else
    {
        if (Channel_All_20M[index].length > 1)
        {
            if (Channel_All_20M[index][0] == 0)
            {
                ch_list.options[len].text = "<%40%>";
                ch_list.options[len].value = Channel_All_20M[index][0];
                len++;
            }
            for (i=1; i<Channel_All_20M[index].length; i++)
            {
                ch_list.options[len].text = Channel_All_20M[index][i];
                ch_list.options[len].value = Channel_All_20M[index][i];
                len++;
            }
            if (curr_region != cf.WRegion.value)
                currChVal_5g = Channel_All_20M[index][0];
        }
    }
    ch_list.options.length = len;

    for (i=0; i<ch_list.options.length; i++)
    {
        if (ch_list.options[i].value == currChVal_5g)
        {
            ch_list[i].selected = true;
            break;
        }
    }
}


function checkRegion()
{
    var cf = document.forms[0];
    if(cf.WRegion.selectedIndex == 0)
    {
        cf.WRegion.focus();
        return alertR("<%38%>");
    }
    if (notShown)
        notShown = false;
    else if(cf.WRegion.selectedIndex != initRegion && notShown)
    {
        if(!confirm(changeMsg))
            return false;
        else notShown = false;
    }
    return true;
}

function chgCh(from)
{
    var cf = document.forms[0];
    if (from == 1 && cf.WRegion.options[cf.WRegion.selectedIndex].value != "21")
    {
        setDivDisable($("#5g_network"), 0);
        setDivDisable($("#5g_div"), 0);
        if ("1" == cf.enable_ap_mode.value)
        {
            //cf.w_channel_5g.disabled = true;
        }
        else if("2" == cf.mode.value)
        {  
            if(("5G" == cf.sta_band.value)||("both" == cf.sta_band.value))
                cf.w_channel_5g.disabled = true;
        }
    }
    setOpMode();
    setChannel();
    if ( from >= 2 )
    {        
        //cf.only_mode.value = "1";
        if(from==2)
            change_sec("security_type",cf.ap_24g_sec.value);
        else
            change_sec("security_type_5g",cf.ap_5g_sec.value);
                    
        displayWepWPARadio();  
        //cf.submit();
    }
    else
    {
        if (cf.WRegion.options[cf.WRegion.selectedIndex].value == "21")
        {
            setDivDisable($("#5g_network"), 1);
            setDivDisable($("#5g_div"), 1);
        }
        displayWepWPARadio();
    }
}
function setOpMode()
{
    var cf = document.forms[0];
    if ("1" == cf.enable_ap_mode.value)
    {
        setOpMode_24g();
        setOpMode_5g();
    }
    else if((("1" == cf.mode.value) && ("2.4G" == cf.ap_band.value)) || ("0" == cf.mode.value))
    {
        setOpMode_24g();
    } 
    else if(("1" == cf.mode.value) && ("5G" == cf.ap_band.value))
    {
        setOpMode_5g();
    } 
    //else if(("2" == cf.mode.value) && ("2.4G" == cf.sta_band.value))
    //{
    //    setOpMode_5g();
    //} 
    //else if(("2" == cf.mode.value) && ("5G" == cf.sta_band.value))
    //{
    //    setOpMode_24g();
    //}
    else if ("2" == cf.mode.value)
    {
        setOpMode_24g();
        setOpMode_5g();
    }
    
}

function setOpMode_24g()
{
    var cf = document.forms[0];
    var currentMode = cf.opmode.selectedIndex;    
    var sec_type_2G = cf.ap_24g_sec.value;

    if ((sec_type_2G == "WEP") || (sec_type_2G == "WPA-PSK"))
    {
        cf.opmode.options.length = 1;
        cf.opmode.options[0].text = OpMode_BG_TEXT;
        cf.opmode.options[0].value = OpMode_BG_VALUE;                     
        //cf.opmode.selectedIndex = currentMode;                      
    }                          
    else
    {
        cf.opmode.options.length = 3;
        cf.opmode.options[0].text = OpMode_BG_TEXT;
        cf.opmode.options[1].text = OpMode_20M_TEXT;
        cf.opmode.options[2].text = OpMode_40M_TEXT;
        cf.opmode.options[0].value = OpMode_BG_VALUE;
        cf.opmode.options[1].value = OpMode_20M_VALUE;
        cf.opmode.options[2].value = OpMode_40M_VALUE;
        cf.opmode.selectedIndex = currentMode;
    }   
    return;
}

function setOpMode_24g_psr()
{
    var cf = document.forms[0];
    var currentMode = cf.opmode.selectedIndex;    
    var sec_type_2G = cf.ap_24g_sec.value;

    if ((sec_type_2G == "None") || (sec_type_2G == "WPA2-PSK")|| (sec_type_2G == "WPA-AUTO-PSK"))
    {
        cf.opmode.disabled = false;
        cf.opmode.options.length = 2;
        cf.opmode.options[0].text = "<%3370%>";
        cf.opmode.options[1].text = "<%3371%>";
        cf.opmode.options[0].value = "145Mbps";
        cf.opmode.options[1].value = "300Mbps";
        cf.opmode.selectedIndex = currentMode;
    }
    else
    {
        cf.opmode.disabled = true;
        cf.opmode.options.length = 1;
        cf.opmode.options[0].text = "<%40%>";
        cf.opmode.options[0].value = "g and b";
    }
    return;
}

function setOpMode_5g()
{
    var cf = document.forms[0];
    var currentMode_5g = cf.opmode_5g.selectedIndex;
    var sec_type_5G = cf.ap_5g_sec.value;
    var index = cf.WRegion.selectedIndex;
   
    if ((sec_type_5G == "WEP") || (sec_type_5G == "WPA-PSK"))
    {
        cf.opmode_5g.options.length = 1;
        cf.opmode_5g.options[0].text = OpMode_5G_BG_TEXT;
        cf.opmode_5g.options[0].value = OpMode_5G_BG_VALUE;
        //cf.opmode.selectedIndex = currentMode;
    }
    else
    {
        if (Channel_All_80M[index].length > 1)
            cf.opmode_5g.options.length = 4;
        else if (Channel_All_40M[index].length > 1)
            cf.opmode_5g.options.length = 3;
        else if (Channel_All_20M[index].length > 1)
            cf.opmode_5g.options.length = 2;
        else
            cf.opmode_5g.options.length = 0;
        if (cf.opmode_5g.options.length > 0)
        {
            cf.opmode_5g.options[0].text = OpMode_5G_BG_TEXT;
            cf.opmode_5g.options[0].value = OpMode_5G_BG_VALUE;
        }
        if (cf.opmode_5g.options.length > 1)
        {
            cf.opmode_5g.options[1].text = OpMode_5G_20M_TEXT;
            cf.opmode_5g.options[1].value = OpMode_5G_20M_VALUE;
        }
        if (cf.opmode_5g.options.length > 2)
        {
            cf.opmode_5g.options[2].text = OpMode_5G_40M_TEXT;
            cf.opmode_5g.options[2].value = OpMode_5G_40M_VALUE;
        }
        if (cf.opmode_5g.options.length > 3)
        {
            cf.opmode_5g.options[3].text = OpMode_5G_80M_TEXT;
            cf.opmode_5g.options[3].value = OpMode_5G_80M_VALUE;
        }
        
        if (currentMode_5g < cf.opmode_5g.options.length && currentMode_5g>=0)
            cf.opmode_5g.selectedIndex = currentMode_5g;
        else
            cf.opmode_5g.selectedIndex = cf.opmode_5g.options.length-1;
    }
    return;
}

function displayWepWPARadio()
{
    var cf = document.forms[0];
    if ("1" == cf.enable_ap_mode.value)
    {
        displayWepWPARadio_24g();
        displayWepWPARadio_5g();
        //document.getElementById("wep_radio_5G").style.display=block_str;
        //document.getElementById("wpa_radio_5G").style.display=block_str;
        //document.getElementById("wep_radio_2G").style.display=block_str;
        //document.getElementById("wpa_radio_2G").style.display=block_str;
    }
    else if((("1" == cf.mode.value) && ("2.4G" == cf.ap_band.value)) || ("0" == cf.mode.value))
    {
        displayWepWPARadio_24g();
    } 
    else if(("1" == cf.mode.value) && ("5G" == cf.ap_band.value))
    {
        displayWepWPARadio_5g();
    } 
    else
    {
        displayWepWPARadio_24g();
        displayWepWPARadio_5g();
    }
    //else if(("2" == cf.mode.value) && ("2.4G" == cf.sta_band.value))
    //{
    //    displayWepWPARadio_5g();
    //} 
    //else if(("2" == cf.mode.value) && ("5G" == cf.sta_band.value))
    //{
    //    displayWepWPARadio_24g();
    //}
    //else if(("2" == cf.mode.value) && ("both" == cf.sta_band.value))
    //{
    //    document.getElementById("wep_radio_5G").style.display=block_str;
        //document.getElementById("wpa_radio_5G").style.display=block_str;
    //    document.getElementById("wep_radio_2G").style.display=block_str;
        //document.getElementById("wpa_radio_2G").style.display=block_str;
    //}   
}

function displayWepWPARadio_24g()
{
    var cf = document.forms[0];
    var currentMode = cf.opmode.selectedIndex;
    //var currentMode_an = cf.opmode_an.selectedIndex;
    //var sec_type_2G = cf.security_type_2G.value;
    //var sec_type_5G = cf.security_type_5G.value;
    //document.getElementById("wep_radio_5G").style.display=block_str;
    //document.getElementById("wpa_radio_5G").style.display=block_str;
    
    if (currentMode == 0)//Up to 54Mbps
    {
        document.getElementById("wep_radio_2G").style.display=block_str;
        //document.getElementById("wpa_radio_2G").style.display=block_str;
    }
    else
    {
        document.getElementById("wep_radio_2G").style.display="none";
        //document.getElementById("wpa_radio_2G").style.display="none";
    }    
}

function displayWepWPARadio_5g()
{
    var cf = document.forms[0];
    var currentMode = cf.opmode_5g.selectedIndex;
    //var currentMode_an = cf.opmode_an.selectedIndex;
    //var sec_type_2G = cf.security_type_2G.value;
    //var sec_type_5G = cf.security_type_5G.value;
    //document.getElementById("wep_radio_2G").style.display=block_str;
    //document.getElementById("wpa_radio_2G").style.display=block_str;
    
    if (currentMode == 0)//Up to 54Mbps
    {
        document.getElementById("wep_radio_5G").style.display=block_str;
        //document.getElementById("wpa_radio_5G").style.display=block_str;
    }
    else
    {
        document.getElementById("wep_radio_5G").style.display="none";
        //document.getElementById("wpa_radio_5G").style.display="none";
    }    
}
function loadWpaHelp(band)
{
    var cf  = document.forms[0];
    if(2 == band)
    {
        if("WPA-PSK" == cf.ap_24g_sec.value)
            loadhelp("ap_settings","wpa-psk");
        else if("WPA2-PSK" == cf.ap_24g_sec.value)
            loadhelp("ap_settings","wpa2-psk");
        else if("WPA-AUTO-PSK" == cf.ap_24g_sec.value)
            loadhelp("ap_settings","wpa-psk+wpa2-psk");
    }
    else if(5 == band)
    {
        if("WPA-PSK" == cf.ap_5g_sec.value)
            loadhelp("ap_settings","wpa-psk");
        else if("WPA2-PSK" == cf.ap_5g_sec.value)
            loadhelp("ap_settings","wpa2-psk");
        else if("WPA-AUTO-PSK" == cf.ap_5g_sec.value)
            loadhelp("ap_settings","wpa-psk+wpa2-psk");
    }
}
//-->