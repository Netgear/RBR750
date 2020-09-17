function loadSettings()
{
    loadSettings_wireless();

    $('#wifiSettingsForm').change(function() {
        $("input[name='save_bt']").removeAttr('disabled');
    });

    var cf = document.forms["enablePageForm"];
    var cf1 = document.forms["hyperFiForm"];
    var cf2 = document.forms["captureLogForm"];
    var cf3 = document.forms["steeringForm"];
    var cf4 = document.forms["hyperFiForm2"];
    var cf5 = document.forms["DFSForm"];

    if(cf.Start_When_Boot_Up.value == "1")
		cf.action_Start_When_Boot_Up.checked=true;
	else 
		cf.action_Start_When_Boot_Up.checked=false;

    if(cf1.enable_hyperFi_clicked.value == "disable")
    {
        cf3.enable_device_steering.disabled=true;
        document.getElementById("hyperFiStatus").innerHTML="Status: Disabled";
    }
    else
    {
        cf3.enable_device_steering.disabled=false;
        document.getElementById("hyperFiStatus").innerHTML="Status: Enabled";
    }

    if(cf3.disable_device_steer.value == "1")
        document.getElementById("steeringStatus").innerHTML="Status: Disabled";
    else
        document.getElementById("steeringStatus").innerHTML="Status: Enabled";

    if(cf4.log_output_modes.value == "5")
        cf4.mode.value="";
    else
        cf4.mode.value=cf4.log_output_modes.value;
    cf4.level.value=cf4.log_output_level.value;

    if(cf2.capture_in_process.value != "")
        cf2.action_Start_Capture.disabled=true;
    else
        cf2.action_Start_Capture.disabled=false;

    if(cf5.enable_DFS_clicked.value == "1")
        document.getElementById("DFSStatus").innerHTML="Status: Disabled";
    else
        document.getElementById("DFSStatus").innerHTML="Status: Enabled";
}

function clickDownloadButton()
{
    var cf2 = document.forms["captureLogForm"];
	if(cf2.action_Start_Capture.disabled)
        cf2.action_Start_Capture.disabled=false;
}

/**
* General section
*/

var initRegion;
var preRegion;
var changeMsg = "<%926%>";
var notShown = true;
var StartChannel = new Array(<%927%>);
var FinishChannel = new Array(<%928%>);

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
var Channel_All_80M = 
    new Array (
    <%931%>
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

var SecuOption_OFF_TEXT;
var SecuOption_OFF_VALUE;
var SecuOption_WEP_TEXT;
var SecuOption_WEP_VALUE;
var SecuOption_WPA_TEXT;
var SecuOption_WPA_VALUE;
var SecuOption_WPA2_TEXT;
var SecuOption_WPA2_VALUE;
var SecuOption_MIX_TEXT;
var SecuOption_MIX_VALUE;

var curr_region = "<%617%>";

function setDivVisiable(setDiv,flag)
{
    var $overlay = "<div id='divOverlay'></div>";

    if(setDiv.find('#divOverlay').length == 0)
        setDiv.append($overlay);

    if(1 == flag)
    {
        setDiv.find('#divOverlay').addClass('active');
    }
    else
        setDiv.find('#divOverlay').removeClass('active');
}

function loadSettings_wireless()
{
    var cf = document.forms["mainForm"];
    var i;

    OpMode_BG_TEXT=cf.opmode.options[0].text;
    OpMode_20M_TEXT=cf.opmode.options[1].text;
    OpMode_40M_TEXT=cf.opmode.options[2].text;
    OpMode_BG_VALUE=cf.opmode.options[0].value;
    OpMode_20M_VALUE=cf.opmode.options[1].value;
    OpMode_40M_VALUE=cf.opmode.options[2].value;

    OpMode_5G_BG_TEXT=cf.opmode_5g.options[0].text;
    OpMode_5G_20M_TEXT=cf.opmode_5g.options[1].text;
    OpMode_5G_40M_TEXT=cf.opmode_5g.options[2].text;
    OpMode_5G_BG_VALUE=cf.opmode_5g.options[0].value;
    OpMode_5G_20M_VALUE=cf.opmode_5g.options[1].value;
    OpMode_5G_40M_VALUE=cf.opmode_5g.options[2].value;

    if (cf.opmode_5g.options.length >= 4) {
        OpMode_5G_80M_TEXT=cf.opmode_5g.options[3].text;
        OpMode_5G_80M_VALUE=cf.opmode_5g.options[3].value;
    }

    SecuOption_OFF_TEXT=cf.sec_type.options[1].text;
    SecuOption_OFF_VALUE=cf.sec_type.options[1].value;
    SecuOption_WEP_TEXT=cf.sec_type.options[2].text;
    SecuOption_WEP_VALUE=cf.sec_type.options[2].value;
    SecuOption_WPA_TEXT=cf.sec_type.options[3].text;
    SecuOption_WPA_VALUE=cf.sec_type.options[3].value;
    SecuOption_WPA2_TEXT=cf.sec_type.options[4].text;
    SecuOption_WPA2_VALUE=cf.sec_type.options[4].value;
    SecuOption_MIX_TEXT=cf.sec_type.options[5].text;
    SecuOption_MIX_VALUE=cf.sec_type.options[5].value;  

    if(cf.tempSetting.value == 1)
    {
        var temp_region = parseInt(cf.tempRegion.value);
        if (cf.tempRegion.value == "")
        {
            if (parseInt(cf.setRegion.value) == 12)
                cf.wregion.selectedIndex=10;
            else if (parseInt(cf.setRegion.value) >= 10 && parseInt(cf.setRegion.value) <= 11)
                cf.wregion.selectedIndex=parseInt(cf.setRegion.value)+1;
            else
                cf.wregion.selectedIndex=parseInt(cf.setRegion.value);
        }
        else if (temp_region == 12)
            cf.wregion.selectedIndex=10;
        else if (temp_region >= 10 && temp_region <= 11)
            cf.wregion.selectedIndex=temp_region+1;
        else
            cf.wregion.selectedIndex=temp_region;
    }
        
    initRegion = cf.wregion.selectedIndex;  
    preRegion = cf.wregion.options[cf.wregion.selectedIndex].value;

    setOpMode();
    setChannel();

    setKeys(cf.wep_enc, cf.key1, cf.key2, cf.key3, cf.key4,2);
    setKeys(cf.wep_enc_5g, cf.key5g1, cf.key5g2, cf.key5g3, cf.key5g4,5);   
    
    if (cf.wire_mode.value == "1" || cf.ap_mode.value == "2")
    {
        $("#24g_network").css('display', 'block');
        $("#5g_network").css('display', 'block');

        cf.channel.disabled = false;
        cf.channel_5g.disabled = false;

        show_24g_div();
        show_5g_div();

        for (i=0; i<cf.channel.options.length; i++)
        {
            if (cf.channel.options[i].value == cf.initChannel.value)
            {
                cf.channel[i].selected = true;
                break;
            }
        }
        for (i=0; i<cf.channel_5g.options.length; i++)
        {
            if (cf.channel_5g.options[i].value == cf.initChannel_5g.value)
            {
                cf.channel_5g[i].selected = true;
                break;
            }
        }        
    } 
    else if((("1" == cf.ap_mode.value) && ("2.4G" == cf.ap_band.value)) || ("0" == cf.ap_mode.value))
    {
        $("#24g_network").css('display', 'block');
        $("#5g_network").css('display', 'none');
        show_24g_div();

        for (i=0; i<cf.channel.options.length; i++)
        {
            if (cf.channel.options[i].value == cf.initChannel.value)
            {
                cf.channel[i].selected = true;
                break;
            }
        }
    } 
    else if(("1" == cf.ap_mode.value) && ("5G" == cf.ap_band.value))
    {
        $("#24g_network").css('display', 'none');
        $("#5g_network").css('display', 'block');
        $("#5g_network").addClass('first');
        show_5g_div();

        for (i=0; i<cf.channel_5g.options.length; i++)
        {
            if (cf.channel_5g.options[i].value == cf.initChannel_5g.value)
            {
                cf.channel_5g[i].selected = true;
                break;
            }
        }
    }
    
    displayWepWPARadio();

    if (cf.wregion.options[cf.wregion.selectedIndex].value == "21")
        setDivVisiable($("#5g_network"), 1);
}

function show_24g_div()
{
    var cf = document.forms["mainForm"];
    var secu_type_24g = cf.ap_24g_sec.value;
    var i;

    if (cf.wire_mode.value == "1")
    {
        $("#24g_network").find('.securityOptionsWrap').slideDown();

        for (i=0; i<cf.sec_type.options.length; i++)
        {
            if (cf.sec_type.options[i].value == secu_type_24g)
            {
                cf.sec_type[i].selected = true;
                if ("None" == secu_type_24g)
                {
                    $("#24g_network").find('.wepInput').slideUp();
                    $("#24g_network").find('.pwdInput').slideUp();
                }
                else if ("WEP" == secu_type_24g)
                    $("#24g_network").find('.wepInput').slideDown();
                else
                    $("#24g_network").find('.pwdInput').slideDown();
                break;
            }
        }
    }
    else
    {
        $("#24g_network").find('.securityOptionsWrap').slideDown();

        for (i=0; i<cf.sec_type.options.length; i++)
        {
            if (cf.sec_type.options[i].value == secu_type_24g)
            {
                cf.sec_type[i].selected = true;
                if ("WEP" == secu_type_24g)
                    $("#24g_network").find('.wepInput').slideDown();
                else
                    $("#24g_network").find('.pwdInput').slideDown();
                break;
            }
        }
    }
}

function show_5g_div()
{
    var cf = document.forms["mainForm"];
    var secu_type_5g = cf.ap_5g_sec.value;
    var i;

    if (cf.wire_mode.value == "1")
    {
        $("#5g_network").find('.securityOptionsWrap').slideDown();

        for (i=0; i<cf.sec_type_5g.options.length; i++)
        {
            if (cf.sec_type_5g.options[i].value == secu_type_5g)
            {
                cf.sec_type_5g[i].selected = true;
                if ("None" == secu_type_5g)
                {
                    $("#5g_network").find('.wepInput').slideUp();
                    $("#5g_network").find('.pwdInput').slideUp();
                }
                else if ("WEP" == secu_type_5g)
                    $("#5g_network").find('.wepInput').slideDown();
                else
                    $("#5g_network").find('.pwdInput').slideDown();
                break;
            }
        }
    }
    else
    {
        $("#5g_network").find('.securityOptionsWrap').slideDown();

        for (i=0; i<cf.sec_type_5g.options.length; i++)
        {
            if (cf.sec_type_5g.options[i].value == secu_type_5g)
            {
                cf.sec_type_5g[i].selected = true;
                if ("WEP" == secu_type_5g)
                    $("#5g_network").find('.wepInput').slideDown();
                else
                    $("#5g_network").find('.pwdInput').slideDown();
                break;
            }
        }
    }
}

function change_sec(band_type, sec_type)
{
    var cf = document.forms["mainForm"];
    var thisParent;
    var sec_type;

    if ("24g" == band_type)
    {
        thisParent = $('#24g_network');
        cf.ap_24g_sec.value = sec_type;
    }
    else if ("5g" == band_type)
    {
        thisParent = $('#5g_network');
        cf.ap_5g_sec.value = sec_type;
    }

    if("WEP" == sec_type)
    {
        thisParent.find('.wepInput').slideDown();
        thisParent.find('.pwdInput').slideUp();
    }
    else if("WPA-PSK" == sec_type || "WPA2-PSK" == sec_type || "WPA-AUTO-PSK" == sec_type)
    {
        thisParent.find('.wepInput').slideUp();
        thisParent.find('.pwdInput').slideDown();
    }
    else
    {
        thisParent.find('.wepInput').slideUp();
        thisParent.find('.pwdInput').slideUp();
    }
}

function checkData_wireless()
{
    var cf = document.forms["mainForm"];
    var success_24g = false;
    var success_5g = false;
    var success = false;

    if ((cf.wire_mode.value == "1") || ("2" == cf.ap_mode.value))
    {
        success_24g = CheckData24G();
        
        if(!success_24g)
            return false;
        
        success_5g = CheckData5G();
        success = success_24g && success_5g;
    }
    else if("1" == cf.ap_mode.value && "5G" == cf.ap_band.value)
    {
        success_5g = CheckData5G();
        success = success_5g;  
    }
    else if("1" == cf.ap_mode.value && "2.4G" == cf.ap_band.value)
    {
        success_24g = CheckData24G();
        success = success_24g;
    }
    else if("0" == cf.ap_mode.value)
    {
        success_24g = CheckData24G();
        success = success_24g;
    }

    if (!success)
        return false;

    cf.security_type.value = cf.ap_24g_sec.value;
    cf.security_type_5g.value = cf.ap_5g_sec.value;

    return true;
}

function CheckData24G()
{   
    var cf = document.forms["mainForm"];
    var pass = cf.passphrase.value;
    var ver_pass = cf.verify_pwd.value;
    
    if (cf.ssid.value == "")
    {
        alert("<%480%>");
        return false;
    }

    if (cf.ssid.value.match( /[^\x20-\x7E]/ ))
    {
        alert("<%481%>");
        return false;
    }

    if(!checkRegion()) return false;

    if("None" == cf.ap_24g_sec.value)
        return CheckData_NONE();
    else if( ("WPA-AUTO-PSK" == cf.ap_24g_sec.value) || ("WPA-PSK" == cf.ap_24g_sec.value) || ("WPA2-PSK" == cf.ap_24g_sec.value) )
    {
        if (pass != ver_pass)
          return alertR("<%935%>");
        
        return CheckData_WPA(cf.passphrase);
    }
    else if("WEP" == cf.ap_24g_sec.value)
        return CheckData_WEP(cf.wep_enc, cf.wep_key_no, "key", cf.key1, cf.key2, cf.key3, cf.key4);
    else
    {
        cf.sec_type.focus();
        return alertR("<%482%>");
    }
}

function CheckData5G()
{
    var cf = document.forms["mainForm"];
    var pass = cf.passphrase_5g.value;
    var ver_pass = cf.verify_pwd_5g.value;

    if (cf.wregion.options[cf.wregion.selectedIndex].value == "21")
        return true;

    if (cf.ssid_5g.value == "")
    {
        alert("<%480%>");
        return false;
    }

    if (cf.ssid_5g.value.match( /[^\x20-\x7E]/ ))
    {
        alert("<%481%>");
        return false;
    }

    if(!checkRegion()) return false;

    if("None" == cf.ap_5g_sec.value)
        return CheckData_NONE();
    else if( ("WPA-AUTO-PSK" == cf.ap_5g_sec.value) || ("WPA-PSK" == cf.ap_5g_sec.value) || ("WPA2-PSK" == cf.ap_5g_sec.value) ) 
    {
        if (pass != ver_pass)
          return alertR("<%935%>");
        
        return CheckData_WPA(cf.passphrase_5g);
    }
    else if("WEP" == cf.ap_5g_sec.value)
        return CheckData_WEP(cf.wep_enc_5g, cf.wep_key_no_5g, "key5g", cf.key5g1, cf.key5g2, cf.key5g3, cf.key5g4);
    else
    {
        cf.sec_type_5g.focus();
        return alertR("<%482%>");
    }
}

function setChannel()
{
    var cf = document.forms["mainForm"];
    
    if(cf.wire_mode.value == "1")
    {
        setChannel_24g();
        setChannel_5g();
    }
    else if((("1" == cf.ap_mode.value) && ("2.4G" == cf.ap_band.value)) || ("0" == cf.ap_mode.value))
    {
        setChannel_24g();
    } 
    else if(("1" == cf.ap_mode.value) && ("5G" == cf.ap_band.value))
    {
        setChannel_5g();
    } 
    else if(("2" == cf.ap_mode.value) && ("2.4G" == cf.sta_band.value))
    {
        setAutoChannel_24g();
        setChannel_5g();
    } 
    else if(("2" == cf.ap_mode.value) && ("5G" == cf.sta_band.value))
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

function setAutoChannel_24g()
{
    var cf = document.forms["mainForm"];
    cf.channel.options[0].text = "<%936%>";
    cf.channel.options[0].value = 0;
}

function setAutoChannel_5g()
{
    var cf = document.forms["mainForm"];
    cf.channel_5g.options[0].text = "<%936%>";
    cf.channel_5g.options[0].value = 0;
}

function setChannel_24g()
{
    var cf = document.forms["mainForm"];
    var index = cf.wregion.selectedIndex;
    var chIndex = cf.channel.selectedIndex;

    if (FinishChannel[index]==14)
        cf.channel.options.length = FinishChannel[index] - StartChannel[index] + 1;
    else
        cf.channel.options.length = FinishChannel[index] - StartChannel[index] + 2;

    cf.channel.options[0].text = "<%936%>";
    cf.channel.options[0].value = 0;

    for (var i = StartChannel[index]; i <= FinishChannel[index]; i++) {
        if (i==14)
            continue;    
        cf.channel.options[i - StartChannel[index] + 1].value = i;
        cf.channel.options[i - StartChannel[index] + 1].text = ((i < 10)? "0" + i : i);
    }
    cf.channel.selectedIndex = ((chIndex > -1) && (chIndex < cf.channel.options.length)) ? chIndex : 0 ;        
}

function setChannel_5g()
{
    var cf = document.forms["mainForm"];
    var ch_list = cf.channel_5g;
    var index = cf.wregion.selectedIndex;
    var currChVal_5g = "<%614%>";
    var len=0, i=0;

    ch_list.options.length = 100;
    if (Channel_All_80M[index].length > 1)
    {
        if (Channel_All_80M[index][0] == 0)
        {
            ch_list.options[len].text = "<%936%>";
            ch_list.options[len].value = Channel_All_80M[index][0];
            len++;
        }
        for (i=1; i<Channel_All_80M[index].length; i++)
        {
            ch_list.options[len].text = Channel_All_80M[index][i];
            ch_list.options[len].value = Channel_All_80M[index][i];
            len++;
        }
        if (curr_region != cf.wregion.value)
            currChVal_5g = Channel_All_80M[index][0];
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
    var cf = document.forms["mainForm"];
    if (cf.wregion.options[cf.wregion.selectedIndex].value == "0")
    {
        cf.wregion.focus();
        return alertR("<%937%>");
    }
    if (notShown)
        notShown = false;
    else if(cf.wregion.selectedIndex != initRegion && notShown)
    {
        if(!confirm(changeMsg))
            return false;
        else notShown = false;
    }
    return true;
}

function chgCh(from)
{
    var cf = document.forms["mainForm"];

    if (form == 1)
    {
        if (cf.wregion.options[cf.wregion.selectedIndex].value != "21")
            setDivVisiable($("#5g_network"), 0);
        
        preRegion = cf.wregion.options[cf.wregion.selectedIndex].value;
    }

    setOpMode();
    setChannel();
    if ( from >= 2 )
    {        
        if(from==2)
            change_sec("24g", cf.ap_24g_sec.value);
        else
            change_sec("5g", cf.ap_5g_sec.value);
                    
        displayWepWPARadio();
    }
    else
    {
        if (cf.wregion.options[cf.wregion.selectedIndex].value == "21")
            setDivVisiable($("#5g_network"), 1);

        displayWepWPARadio();
    }
}

function setOpMode()
{
    var cf = document.forms["mainForm"];
    
    if(cf.wire_mode.value == "1")
    {
        setOpMode_24g();
        setOpMode_5g();
    }
    else if((("1" == cf.ap_mode.value) && ("2.4G" == cf.ap_band.value)) || ("0" == cf.ap_mode.value))
    {
        setOpMode_24g();
    } 
    else if(("1" == cf.ap_mode.value) && ("5G" == cf.ap_band.value))
    {
        setOpMode_5g();
    } 
    else if(("2" == cf.ap_mode.value) && ("2.4G" == cf.sta_band.value))
    {
        setOpMode_24g();
        setOpMode_5g();
    } 
    else if(("2" == cf.ap_mode.value) && ("5G" == cf.sta_band.value))
    {
        setOpMode_24g();
        setOpMode_5g();
    }
    else//both
    {
        setOpMode_24g();
        setOpMode_5g();
    }
}

function setOpMode_24g()
{
    var cf = document.forms["mainForm"];
    var currentMode = cf.opmode.selectedIndex;    
    var sec_type_2G = cf.ap_24g_sec.value;

    if ((sec_type_2G == "WEP") || (sec_type_2G == "WPA-PSK"))
    {
        cf.opmode.options.length = 1;
        cf.opmode.options[0].text = OpMode_BG_TEXT;
        cf.opmode.options[0].value = OpMode_BG_VALUE;
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

    if (cf.wire_mode.value != "1")
    {
        if(cf.rootap_link_24g.value == "1")
            cf.opmode.disabled = true;
        else
            cf.opmode.disabled = false;
    }
}

function setOpMode_5g()
{
    var cf = document.forms["mainForm"];
    var currentMode_5g = cf.opmode_5g.selectedIndex;
    var sec_type_5G = cf.ap_5g_sec.value;
    var index = cf.wregion.selectedIndex;
   
    if ((sec_type_5G == "WEP") || (sec_type_5G == "WPA-PSK"))
    {
        cf.opmode_5g.options.length = 1;
        cf.opmode_5g.options[0].text = OpMode_5G_BG_TEXT;
        cf.opmode_5g.options[0].value = OpMode_5G_BG_VALUE;
        //cf.opmode1.selectedIndex = currentMode;
    }
    else
    {
        if (Channel_All_80M[index].length > 1)
            cf.opmode_5g.options.length = 4;
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

    if (cf.wire_mode.value != "1")
    {
        if(cf.rootap_link_5g.value == "1")
            cf.opmode_5g.disabled = true;
        else
            cf.opmode_5g.disabled = false;
    }
}

function displayWepWPARadio()
{
    displayWepWPARadio_24g();
    displayWepWPARadio_5g();
}

function displayWepWPARadio_24g()
{
    var cf = document.forms["mainForm"];
    var tmp_secu = cf.sec_type.value;
    var tmp = "#secType option[value='"+tmp_secu+"']";
    
    if (cf.wire_mode.value == "1")
    {
        cf.sec_type.options.length = 5;
        cf.sec_type.options[1].text=SecuOption_OFF_TEXT;
        cf.sec_type.options[1].value=SecuOption_OFF_VALUE;
        cf.sec_type.options[2].text=SecuOption_WEP_TEXT;
        cf.sec_type.options[2].value=SecuOption_WEP_VALUE;
        cf.sec_type.options[3].text=SecuOption_WPA2_TEXT;
        cf.sec_type.options[3].value=SecuOption_WPA2_VALUE
        cf.sec_type.options[4].text=SecuOption_MIX_TEXT;
        cf.sec_type.options[4].value=SecuOption_MIX_VALUE;
    }
    else
    {
        cf.sec_type.options.length = 4;
        cf.sec_type.options[1].text=SecuOption_WEP_TEXT;
        cf.sec_type.options[1].value=SecuOption_WEP_VALUE;
        cf.sec_type.options[2].text=SecuOption_WPA2_TEXT;
        cf.sec_type.options[2].value=SecuOption_WPA2_VALUE
        cf.sec_type.options[3].text=SecuOption_MIX_TEXT;
        cf.sec_type.options[3].value=SecuOption_MIX_VALUE;
    }
    
    if ($(tmp).length>0)
        $("#secType").val(tmp_secu);
    else
        $("#secType").val(SecuOption_MIX_VALUE);
}

function displayWepWPARadio_5g()
{
    var cf = document.forms["mainForm"];
    var tmp_secu = cf.sec_type_5g.value;
    var tmp = "#secType5g option[value='"+tmp_secu+"']";

    if (cf.wire_mode.value == "1")
    {
        cf.sec_type_5g.options.length = 5;
        cf.sec_type_5g.options[1].text=SecuOption_OFF_TEXT;
        cf.sec_type_5g.options[1].value=SecuOption_OFF_VALUE;
        cf.sec_type_5g.options[2].text=SecuOption_WEP_TEXT;
        cf.sec_type_5g.options[2].value=SecuOption_WEP_VALUE;
        cf.sec_type_5g.options[3].text=SecuOption_WPA2_TEXT;
        cf.sec_type_5g.options[3].value=SecuOption_WPA2_VALUE   
        cf.sec_type_5g.options[4].text=SecuOption_MIX_TEXT;
        cf.sec_type_5g.options[4].value=SecuOption_MIX_VALUE   
    }
    else
    {
        cf.sec_type_5g.options.length = 4;
        cf.sec_type_5g.options[1].text=SecuOption_WEP_TEXT;
        cf.sec_type_5g.options[1].value=SecuOption_WEP_VALUE;
        cf.sec_type_5g.options[2].text=SecuOption_WPA2_TEXT;
        cf.sec_type_5g.options[2].value=SecuOption_WPA2_VALUE   
        cf.sec_type_5g.options[3].text=SecuOption_MIX_TEXT;
        cf.sec_type_5g.options[3].value=SecuOption_MIX_VALUE;
    }

    if ($(tmp).length>0)
        $("#secType5g").val(tmp_secu);
    else
        $("#secType5g").val(SecuOption_MIX_VALUE);
}

$(document).ready(function($) {

$("#saveBt1").on('click', function() {
    if(checkData_wireless())
        document.forms["mainForm"].submit();
    });

$("#uploadBt").on('click', function() {
    var msg = "<%704%>" + "1-2" + "<%705%>";
    $('body').msgshow(msg);
    document.forms["updateFirmwareForm"].submit();
    });

});
