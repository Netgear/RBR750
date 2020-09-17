<!-- 

/**
* this script file handle the wifiSettings.html page of four parts
* --->General/IP Address/Connected Devices/WPS
*/

function loadSettings()
{
    loadSettings_wireless();
    loadSettings_ipAddress();
    loadSettings_WPS();
    showWpsAlert();
    showDFSAlert();

    $('#wifiSettingsForm').change(function() {
        $("input[name='save_bt']").removeAttr('disabled');
    });

    $("input[name='num_of_entries']").blur(function () {
        var cf=document.forms["mainForm"];
        var numberOfEntries = parseInt(cf.num_of_entries.value,10);

        if (!_isNumeric(cf.num_of_entries.value) || isNaN(numberOfEntries)
            || numberOfEntries > 10 || numberOfEntries < 3) {
            alert("<%172%>");
            cf.num_of_entries.value="<%173%>";
        }
    });

    $("input[name='reset_bt']").on('click', function () {
        document.mainForm.reset();
        $(".primaryPwd").siblings('.pwdError').remove();
        $(".primaryPwd").siblings('.top_testresult').remove();
        $(".primaryPwd").siblings('.verifyPwd').val('').removeClass('alert');
        $(".primaryPwd").siblings('.error').remove();
        loadSettings_ipAddress();
    });
}

function checkData()
{
    if( checkData_wireless() 
        && checkData_ipAddress() 
        && checkData_WPS())
    {
        document.forms["mainForm"].submit();
        return true;
    }
    else
        return false;
}

/**
* General section
*/

var initRegion;
var preRegion;
var changeMsg = "<%174%>";
var notShown = true; // only shown region change message once
var initialized = false;
// "--",Africa,Asia,Australia,Canada,Europe,Israel,Japan,Korea,Mexico,Middle East,South America,USA
var StartChannel = new Array(<%175%>);
var FinishChannel = new Array(<%176%>);

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
    <%177%>
        );
var Channel_All_40M = 
    new Array (
    <%178%>
        );
var Channel_All_80M = 
    new Array (
    <%179%>
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

var selected1 = 100, selected2 = 100;
var security_2g = "init", security_5g = "init";
var curr_region = "<%180%>";
var have_alert=0;
var keep_ssid_warning="<%181%>";

if(!Array.indexOf){  
   Array.prototype.indexOf = function(Object){  
     for(var i = 0;i<this.length;i++){  
        if(this[i] == Object){  
           return i;  
         }  
     }  
     return -1;  
   }  
}

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
    
    if (cf.wire_mode.value == "1")
    {
        $(".coex_row").css('display', 'block');
        $("#24g_network").css('display', 'block');
        $("#5g_network").css('display', 'block');

        $("#ether_port_row").css('display', 'none');
        $("#whatPwd_li_24g").css('display','none');
        $("#whatPwd_li_5g").css('display','none');
        
        cf.channel.disabled = false;
        cf.channel_5g.disabled = false;
 
        if ("2.4G" == cf.eth_bind_band.value)
            cf.ether_port[0].checked = true;
        else
            cf.ether_port[1].checked = true;
            
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
    else if("2" == cf.ap_mode.value)
    {
        $(".coex_row").css('display', 'none');
        $("#24g_network").css('display', 'block');
        $("#5g_network").css('display', 'block');
        $("#whatPwd_li_24g").css('display','block');
        $("#whatPwd_li_5g").css('display','block');
        if("both" == cf.sta_band.value && cf.wire_mode.value != "1" && cf.nat_mode.value != "1")
            $("#ether_port_row").css('display', 'block');
        else
            $("#ether_port_row").css('display', 'none');
        
        if("2.4G" == cf.sta_band.value)
        {
            if(cf.wire_mode.value != "1")
            {
                cf.channel.disabled = true;
            }
        }
        else if("5G" == cf.sta_band.value)
        {
            $(".coex_row").css('display', 'block');

            if(cf.wire_mode.value != "1")
            {
                cf.channel_5g.disabled = true;
            }
        }
        else if("both" == cf.sta_band.value)
        {
            if(cf.wire_mode.value != "1")
            {
                cf.channel.disabled = true;
                cf.channel_5g.disabled = true;
            }
        }
        
        if ("2.4G" == cf.eth_bind_band.value)
            cf.ether_port[0].checked = true;
        else
            cf.ether_port[1].checked = true;
            
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
        $(".coex_row").css('display', 'block');
        $("#24g_network").css('display', 'block');
        $("#5g_network").css('display', 'none');
        $("#ether_port_row").css('display', 'none');
        $("#whatPwd_li_24g").css('display','block');
        $("#whatPwd_li_5g").css('display','block');
        cf.eth_bind_band.value = "2.4G";
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
        $(".coex_row").css('display', 'none');
        $("#24g_network").css('display', 'none');
        $("#5g_network").css('display', 'block');
        $("#5g_network").addClass('first');
        $("#ether_port_row").css('display', 'none');
        $("#whatPwd_li_24g").css('display','block');
        $("#whatPwd_li_5g").css('display','block');
        cf.eth_bind_band.value = "5G";
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

    if ("<%182%>" == "enabled")
        WPS_wizard_ungrayout();
    else
        WPS_wizard_grayout();

    $('.whatPwd').on('change', function () {

        if ($(this).val() === '2') {
            if ($(this).parents('#24g_network').length)
                cf.ap_24g_sec.value = "None";
            else if($(this).parents('#5g_network').length)
                cf.ap_5g_sec.value = "None";
            setOpMode();
        }
    });
}

function show_24g_div()
{
    var cf = document.forms["mainForm"];
    var secu_type_24g = cf.ap_24g_sec.value;
    var same_sec_24g = cf.wl_same_sec.value;
    var i;

    if (cf.wire_mode.value == "1")
    {
        cf.what_pwd[1].selected = true;
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
        if ("1" == same_sec_24g) 
            cf.what_pwd[0].selected = true;
        else if ("None" == secu_type_24g)
            cf.what_pwd[2].selected = true;
        else
        {
            cf.what_pwd[1].selected = true;
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
}

function show_5g_div()
{
    var cf = document.forms["mainForm"];
    var secu_type_5g = cf.ap_5g_sec.value;
    var same_sec_5g = cf.wl_same_sec_5g.value;
    var i;

    if (cf.wire_mode.value == "1")
    {
        cf.what_pwd_5g[1].selected = true;
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
        if ("1" == same_sec_5g) 
            cf.what_pwd_5g[0].selected = true;
        else if ("None" == secu_type_5g)
            cf.what_pwd_5g[2].selected = true;
        else
        {
            cf.what_pwd_5g[1].selected = true;
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
}

function select_ether_band()
{
    var cf = document.forms["mainForm"];
    var ether_ports = cf.ether_port;
    if(null != ether_ports)
    {
        if(ether_ports[0].checked)
            cf.eth_bind_band.value = "2.4G";
        else if(ether_ports[1].checked)
            cf.eth_bind_band.value = "5G";
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

        if (!cf.what_pwd[1].selected)
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideUp();
        }
        else if("WEP" == sec_type)
        {
            thisParent.find('.wepInput').slideDown();
            thisParent.find('.pwdInput').slideUp();
        }
        else if("None" == sec_type)
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideUp();
        }
        else if("WPA-PSK" == sec_type)
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideDown();

            if (cf.opmode_5g.selectedIndex != 0)
            {
                /* we add a redundant assignment here so that the 
                   following warning alert wouldn't appear twice */
                cf.opmode_5g.selectedIndex = 0;
                alert("<%106%>");
                setChannel();
                cf.opmode_5g.selectedIndex = 0;
            }
        }
        else if("WPA2-PSK" == sec_type)
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideDown();
        }
        else if("WPA-AUTO-PSK" == sec_type)
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideDown();
            if ((cf.opmode.selectedIndex != 0) && (security_2g != "init")
                && ((cf.opmode.selectedIndex!=selected1) || (sec_type != security_2g)) 
                )
            {
                alert("<%107%>");
            }
        }
        else
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideUp();
        }

        cf.ap_24g_sec.value = sec_type;
        selected1 = cf.opmode.selectedIndex;
        security_2g = sec_type;
    }
    else if ("5g" == band_type)
    {
        thisParent = $('#5g_network');

        if (!cf.what_pwd_5g[1].selected)
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideUp();
        }
        else if("WEP" == sec_type)
        {
            thisParent.find('.wepInput').slideDown();
            thisParent.find('.pwdInput').slideUp();
        }
        else if("None" == sec_type)
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideUp();
        }
        else if("WPA-PSK" == sec_type)
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideDown();

            if (cf.opmode.selectedIndex != 0)
            {
                /* we add a redundant assignment here so that the 
                   following warning alert wouldn't appear twice */
                cf.opmode.selectedIndex = 0;
                alert("<%106%>");
                cf.opmode.selectedIndex = 0;
            }
        }
        else if("WPA2-PSK" == sec_type)
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideDown();
        }
        else if("WPA-AUTO-PSK" == sec_type)
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideDown();
            if ((cf.opmode_5g.selectedIndex != 0) && (security_5g != "init")
                && ((cf.opmode_5g.selectedIndex!=selected2) || (sec_type != security_5g)) 
                )
            {
                alert("<%107%>");
            }
        }
        else
        {
            thisParent.find('.wepInput').slideUp();
            thisParent.find('.pwdInput').slideUp();
        }

        cf.ap_5g_sec.value = sec_type;
        selected2 = cf.opmode_5g.selectedIndex;
        security_5g = sec_type;
    }

    setOpMode();
}

function checkData_wireless()
{
    var cf = document.forms["mainForm"];
    var success_24g = false;
    var success_5g = false;
    var success = false;
    var need_disable_wps = 0;
    
    if (cf.wregion.options[cf.wregion.selectedIndex].value == "21")
        cf.enable_ap_5g.checked = false;
        
    if (cf.wire_mode.value == "1")
    {
        if (cf.enable_ap.checked == false && cf.enable_ap_5g.checked == false)
        {
            alert("<%183%>");
            return false;
        }
    }
    //fix me
    cf.wl_same_sec.value = "0";
    cf.wl_same_sec_5g.value = "0";

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

    if (success_24g && (cf.ap_24g_sec.value == "WEP" || cf.ap_24g_sec.value == "WPA-PSK"))
        need_disable_wps = 1;

    if (success_5g && (cf.ap_5g_sec.value == "WEP" || cf.ap_5g_sec.value == "WPA-PSK"))
        need_disable_wps = 1;

    /* advanced wrap */
    if (!cf.ssid_bc.checked || !cf.ssid_bc_5g.checked)
    {
        if (!confirm("<%184%>"))
            return false;
        
        WPS_wizard_grayout();
    }

    if (need_disable_wps)
    {
        if (!confirm("<%185%>"))
            return false;
        
        WPS_wizard_grayout();
    }

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
        alert("<%81%>");
        return false;
    }

    if (cf.ssid.value.match( /[^\x20-\x7E]/ ))
    {
        alert("<%82%>");
        return false;
    }
 
    if(!checkRegion()) return false;

    //alert(cf.whatPwd.selectedIndex);
    if(0 == cf.what_pwd.selectedIndex) /*Same with exist network*/
    {
        cf.wl_same_sec.value = 1;
        return true;
    }
    else
        cf.wl_same_sec.value = 0;

    if("None" == cf.ap_24g_sec.value)
        return CheckData_NONE();
    else if( ("WPA-AUTO-PSK" == cf.ap_24g_sec.value) || ("WPA-PSK" == cf.ap_24g_sec.value) || ("WPA2-PSK" == cf.ap_24g_sec.value) )
    {
        if (pass != ver_pass)
          return alertR("<%186%>");
        
        return CheckData_WPA(cf.passphrase);
    }
    else if("WEP" == cf.ap_24g_sec.value)
        return CheckData_WEP(cf.wep_enc, cf.wep_key_no, "key", cf.key1, cf.key2, cf.key3, cf.key4);
    else
    {
        cf.sec_type.focus();
        return alertR("<%171%>");
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
        alert("<%81%>");
        return false;
    }

    if (cf.ssid_5g.value.match( /[^\x20-\x7E]/ ))
    {
        alert("<%82%>");
        return false;
    }

    if(!checkRegion()) return false;
    
    //alert(cf.whatPwd_5g.selectedIndex);
    if(0 == cf.what_pwd_5g.selectedIndex) /*Same with exist network*/
    {
        cf.wl_same_sec_5g.value = 1;
        return true;
    }
    else
        cf.wl_same_sec_5g.value = 0;

    if("None" == cf.ap_5g_sec.value)
        return CheckData_NONE();
    else if( ("WPA-AUTO-PSK" == cf.ap_5g_sec.value) || ("WPA-PSK" == cf.ap_5g_sec.value) || ("WPA2-PSK" == cf.ap_5g_sec.value) ) 
    {
        if (pass != ver_pass)
          return alertR("<%186%>");
        
        return CheckData_WPA(cf.passphrase_5g);
    }
    else if("WEP" == cf.ap_5g_sec.value)
        return CheckData_WEP(cf.wep_enc_5g, cf.wep_key_no_5g, "key5g", cf.key5g1, cf.key5g2, cf.key5g3, cf.key5g4);
    else
    {
        cf.sec_type_5g.focus();
        return alertR("<%171%>");
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

//If the interface is psr mode, the channel should be Auto and grey out
function setAutoChannel_24g()
{
    var cf = document.forms["mainForm"];
    cf.channel.options[0].text = "<%187%>";
    cf.channel.options[0].value = 0;
}

function setAutoChannel_5g()
{
    var cf = document.forms["mainForm"];
    cf.channel_5g.options[0].text = "<%187%>";
    cf.channel_5g.options[0].value = 0;
}

function setChannel_24g()
{
    var cf = document.forms["mainForm"];
    var index = cf.wregion.selectedIndex;
    var chIndex = cf.channel.selectedIndex;

    //var b1=0, b2=0, b3=0, b4=0;
    
    //JP Regulation
    //ch1~ch13: all data rate
    //ch14: CCK data rate (1~11Mbps)
    
    if (FinishChannel[index]==14 && cf.opmode.selectedIndex!=0)
        cf.channel.options.length = FinishChannel[index] - StartChannel[index] + 1;
    else
        cf.channel.options.length = FinishChannel[index] - StartChannel[index] + 2;

    cf.channel.options[0].text = "<%187%>";
    cf.channel.options[0].value = 0;

    for (var i = StartChannel[index]; i <= FinishChannel[index]; i++) {
        if (i==14 && cf.opmode.selectedIndex!=0)
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
    //var index = cf.wRegion.options[cf.wRegion.selectedIndex].value;
    var index = cf.wregion.selectedIndex;
    var currChVal_5g = "<%188%>";
    var len=0, i=0;

    ch_list.options.length = 100;
    if (cf.opmode_5g.selectedIndex == 3)    // 80MHz
    {
        if (Channel_All_80M[index].length > 1)
        {
            if (Channel_All_80M[index][0] == 0)
            {
                ch_list.options[len].text = "<%187%>";
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
    }
    else if (cf.opmode_5g.selectedIndex == 2)    // 40MHz
    {
        if (Channel_All_40M[index].length > 1)
        {
            if (Channel_All_40M[index][0] == 0)
            {
                ch_list.options[len].text = "<%187%>";
                ch_list.options[len].value = Channel_All_40M[index][0];
                len++;
            }
            for (i=1; i<Channel_All_40M[index].length; i++)
            {
                ch_list.options[len].text = Channel_All_40M[index][i];
                ch_list.options[len].value = Channel_All_40M[index][i];
                len++;
            }
            if (curr_region != cf.wregion.value)
                currChVal_5g = Channel_All_40M[index][0];
        }
    }
    else
    {
        if (Channel_All_20M[index].length > 1)
        {
            if (Channel_All_20M[index][0] == 0)
            {
                ch_list.options[len].text = "<%187%>";
                ch_list.options[len].value = Channel_All_20M[index][0];
                len++;
            }
            for (i=1; i<Channel_All_20M[index].length; i++)
            {
                ch_list.options[len].text = Channel_All_20M[index][i];
                ch_list.options[len].value = Channel_All_20M[index][i];
                len++;
            }
            if (curr_region != cf.wregion.value)
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
    var cf = document.forms["mainForm"];
    if (cf.wregion.options[cf.wregion.selectedIndex].value == "0")
    {
        cf.wregion.focus();
        return alertR("<%189%>");
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
    var pre_wlanstate2 = "<%190%>";
    
    if (from == 1)
    {
        if (cf.wregion.options[cf.wregion.selectedIndex].value != "21")
        {
            setDivVisiable($("#5g_network"), 0);
            if (preRegion == "21") /* switch region from 21 to non-21 */
            {
                if (pre_wlanstate2 == "Enable")
                    cf.enable_ap_5g.checked = true;
                else if (pre_wlanstate2 == "Disable")
                    cf.enable_ap_5g.checked = false;
            }
        }
        
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
    var currentMode = cf.opmode.selectedIndex;
    var tmp_secu = cf.sec_type.value;
    var tmp = "#secType option[value='"+tmp_secu+"']";

    if (cf.wire_mode.value == "1")
    {
        if (currentMode == 0)//Up to 54Mbps
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
            cf.sec_type.options[1].text=SecuOption_OFF_TEXT;
            cf.sec_type.options[1].value=SecuOption_OFF_VALUE;
            cf.sec_type.options[2].text=SecuOption_WPA2_TEXT;
            cf.sec_type.options[2].value=SecuOption_WPA2_VALUE
            cf.sec_type.options[3].text=SecuOption_MIX_TEXT;
            cf.sec_type.options[3].value=SecuOption_MIX_VALUE;
        }
    }
    else
    {
        if (currentMode == 0)//Up to 54Mbps
        {
            cf.sec_type.options.length = 4;
            cf.sec_type.options[1].text=SecuOption_WEP_TEXT;
            cf.sec_type.options[1].value=SecuOption_WEP_VALUE;
            cf.sec_type.options[2].text=SecuOption_WPA2_TEXT;
            cf.sec_type.options[2].value=SecuOption_WPA2_VALUE
            cf.sec_type.options[3].text=SecuOption_MIX_TEXT;
            cf.sec_type.options[3].value=SecuOption_MIX_VALUE;
        }
        else
        {
            cf.sec_type.options.length = 3;
            cf.sec_type.options[1].text=SecuOption_WPA2_TEXT;
            cf.sec_type.options[1].value=SecuOption_WPA2_VALUE
            cf.sec_type.options[2].text=SecuOption_MIX_TEXT;
            cf.sec_type.options[2].value=SecuOption_MIX_VALUE;
        }
    }
    
    if ($(tmp).length>0)
        $("#secType").val(tmp_secu);
    else
        $("#secType").val(SecuOption_MIX_VALUE);

    if (cf.what_pwd[2].selected)
        change_sec("24g", "None");
    else if (cf.what_pwd[1].selected)
        change_sec("24g",cf.sec_type.value);
}

function displayWepWPARadio_5g()
{
    var cf = document.forms["mainForm"];
    var currentMode = cf.opmode_5g.selectedIndex;
    var tmp_secu = cf.sec_type_5g.value;
    var tmp = "#secType5g option[value='"+tmp_secu+"']";

    if (cf.wire_mode.value == "1")
    {
        if (currentMode == 0)
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
            cf.sec_type_5g.options[1].text=SecuOption_OFF_TEXT;
            cf.sec_type_5g.options[1].value=SecuOption_OFF_VALUE;
            cf.sec_type_5g.options[2].text=SecuOption_WPA2_TEXT;
            cf.sec_type_5g.options[2].value=SecuOption_WPA2_VALUE;
            cf.sec_type_5g.options[3].text=SecuOption_MIX_TEXT;
            cf.sec_type_5g.options[3].value=SecuOption_MIX_VALUE
        }
    }
    else
    {
        if (currentMode == 0)
        {
            cf.sec_type_5g.options.length = 4;
            cf.sec_type_5g.options[1].text=SecuOption_WEP_TEXT;
            cf.sec_type_5g.options[1].value=SecuOption_WEP_VALUE;
            cf.sec_type_5g.options[2].text=SecuOption_WPA2_TEXT;
            cf.sec_type_5g.options[2].value=SecuOption_WPA2_VALUE   
            cf.sec_type_5g.options[3].text=SecuOption_MIX_TEXT;
            cf.sec_type_5g.options[3].value=SecuOption_MIX_VALUE   
        }
        else
        {
            cf.sec_type_5g.options.length = 3;
            cf.sec_type_5g.options[1].text=SecuOption_WPA2_TEXT;
            cf.sec_type_5g.options[1].value=SecuOption_WPA2_VALUE;
            cf.sec_type_5g.options[2].text=SecuOption_MIX_TEXT;
            cf.sec_type_5g.options[2].value=SecuOption_MIX_VALUE
        }
    }    
    if ($(tmp).length>0)
        $("#secType5g").val(tmp_secu);
    else
        $("#secType5g").val(SecuOption_MIX_VALUE);

    if (cf.what_pwd_5g[2].selected)
        change_sec("5g", "None");
    else if (cf.what_pwd_5g[1].selected)
        change_sec("5g",cf.sec_type_5g.value);
}

/**
* IP Address section
*/

function setIP()
{
    var cf = document.forms["mainForm"];
    var dflag = cf.ip_assign[0].checked;

    setDisabled(dflag,cf.ethr1,cf.ethr2,cf.ethr3,cf.ethr4,cf.mask1,cf.mask2,cf.mask3,cf.mask4,cf.gateway1,cf.gateway2,cf.gateway3,cf.gateway4,cf.pri_addr1,cf.pri_addr2,cf.pri_addr3,cf.pri_addr4);
    DisableFixedIP = dflag;
}

function dhcp_update(index)
{
    var cf = document.forms["mainForm"];
    switch (index) {
        case 1:
            cf.sysPoolStartingAddr1.value=cf.LPethr1.value;
            cf.sysPoolFinishAddr1.value=cf.LPethr1.value;
            break;
        case 2:
            cf.sysPoolStartingAddr2.value=cf.LPethr2.value;
            cf.sysPoolFinishAddr2.value=cf.LPethr2.value;
            break;
        case 3:
            cf.sysPoolStartingAddr3.value=cf.LPethr3.value;
            cf.sysPoolFinishAddr3.value=cf.LPethr3.value;
            break;
        case 4:
            cf.sysPoolStartingAddr3.value=cf.LPethr3.value;
            cf.sysPoolFinishAddr3.value=cf.LPethr3.value;
            break;
    }
}

function setNAT()
{
    var cf = document.forms["mainForm"];

    if (cf.NATEnable.checked) {
        cf.dhcp_server.checked = true;
        $('#DivNATOn').css("display", "block");
    } else {
        $('#DivNATOn').css("display", "none");
    }
}

function loadSettings_nat()
{
    var cf = document.forms["mainForm"];
    var lan_ipaddr = cf.lan_ipaddr.value.split(".");
    var lan_netmask = cf.lan_netmask.value.split(".");
    var nat_mode = cf.nat_mode.value;
    var dhcp_start = cf.dhcp_start.value.split(".");
    var dhcp_end = cf.dhcp_end.value.split(".");
    var lan_proto = cf.lan_proto.value;
    var wan_proto = cf.wan_proto.value;

    cf.LPethr1.value = $.trim(lan_ipaddr[0]);
    cf.LPethr2.value = $.trim(lan_ipaddr[1]);
    cf.LPethr3.value = $.trim(lan_ipaddr[2]);
    cf.LPethr4.value = $.trim(lan_ipaddr[3]);
    cf.LMask1.value = $.trim(lan_netmask[0]);
    cf.LMask2.value = $.trim(lan_netmask[1]);
    cf.LMask3.value = $.trim(lan_netmask[2]);
    cf.LMask4.value = $.trim(lan_netmask[3]);

    cf.sysPoolStartingAddr1.value = $.trim(dhcp_start[0]);
    cf.sysPoolStartingAddr2.value = $.trim(dhcp_start[1]);
    cf.sysPoolStartingAddr3.value = $.trim(dhcp_start[2]);
    cf.sysPoolStartingAddr4.value = $.trim(dhcp_start[3]);
    cf.sysPoolFinishAddr1.value = $.trim(dhcp_end[0]);
    cf.sysPoolFinishAddr2.value = $.trim(dhcp_end[1]);
    cf.sysPoolFinishAddr3.value = $.trim(dhcp_end[2]);
    cf.sysPoolFinishAddr4.value = $.trim(dhcp_end[3]);

    if (nat_mode == "1")
        cf.NATEnable.checked = true;
    else
        cf.NATEnable.checked = false;

    setNAT();

    if (lan_proto == "dhcp")
        cf.dhcp_server.checked = true;
    else
        cf.dhcp_server.checked = false;

    if (wan_proto == "pppoe")
    {
        cf.NATEnable.disabled = true;
        cf.dhcp_server.disabled = true;
    }
}

function loadSettings_ipAddress()
{
    var cf = document.forms["mainForm"];
    var wan_proto = cf.wan_proto.value;
    var wan_ipaddr = cf.wan_ipaddr.value.split(".");
    var wan_netmask = cf.wan_netmask.value.split(".");
    var wan_gateway = cf.wan_gateway.value.split(".");
    var wan_dns_tmp = cf.wan_dns1_pri.value.split(" ");
    var wan_dns1_pri = wan_dns_tmp[0].split(".");

    if (wan_proto == "static")
        cf.ip_assign[1].checked = true;
    else
    {
        cf.ip_assign[0].checked = true;
        wan_dns_tmp = cf.wan_dns.value.split(" ");
        wan_dns1_pri = wan_dns_tmp[0].split(".");
    }

    cf.ethr1.value = wan_ipaddr[0];
    cf.ethr2.value = wan_ipaddr[1];
    cf.ethr3.value = wan_ipaddr[2];
    cf.ethr4.value = wan_ipaddr[3];
    cf.mask1.value = wan_netmask[0];
    cf.mask2.value = wan_netmask[1];
    cf.mask3.value = wan_netmask[2];
    cf.mask4.value = wan_netmask[3];
    cf.gateway1.value = wan_gateway[0];
    cf.gateway2.value = wan_gateway[1];
    cf.gateway3.value = wan_gateway[2];
    cf.gateway4.value = wan_gateway[3];
    cf.pri_addr1.value = wan_dns1_pri[0];
    cf.pri_addr2.value = wan_dns1_pri[1];
    cf.pri_addr3.value = wan_dns1_pri[2];
    cf.pri_addr4.value = wan_dns1_pri[3];

    setIP();

    if (cf.wire_mode.value == "1" && cf.nat_support.value == "1")
    {
        $('#DivNAT').css("display", "block");
        loadSettings_nat();
    }
}

function checkData_nat()
{
    var cf = document.forms["mainForm"];
    var msg = "";

    if (cf.NATEnable.checked) {
        var l_ipaddr = (cf.LPethr1.value << 24) | (cf.LPethr2.value << 16)
                        | (cf.LPethr3.value << 8)| (cf.LPethr4.value);
        var l_netmask = (cf.LMask1.value<< 24) | (cf.LMask2.value<< 16)
                        | (cf.LMask3.value<< 8)| (cf.LMask4.value);

        if(checkIP(cf.LMask1,cf.LMask2,cf.LMask3,cf.LMask4,255)
            ||!isNetmask(cf.LMask1, cf.LMask2, cf.LMask3, cf.LMask4))
            msg+= "<%191%>";
        if(checkIP(cf.LPethr1,cf.LPethr2,cf.LPethr3,cf.LPethr4,254)
            ||(parseInt(cf.LPethr4.value,10)==0)
            ||parseInt(cf.LPethr1.value,10)>224)
            msg+= "<%161%>";

        var wan_proto = cf.wan_proto.value;
        var wan_ipaddr = cf.wan_ipaddr.value.split(".");
        var wan_netmask = cf.wan_netmask.value.split(".");
        var w_ipaddr = (wan_ipaddr[0] << 24) | (wan_ipaddr[1] << 16)
                        | (wan_ipaddr[2] << 8)| (wan_ipaddr[3]);
        var w_netmask = (wan_netmask[0]<< 24) | (wan_netmask[1]<< 16)
                        | (wan_netmask[2]<< 8)| (wan_netmask[3]);
        
        if (w_ipaddr
            && (l_netmask & w_netmask & l_ipaddr)
                == (l_netmask & w_netmask & w_ipaddr))
        {
            alert("<%192%>");
            return false;
        }
            
        if (wan_proto == "pppoe")
        {
            var pppoe_wan_ipaddr = cf.pppoe_wan_ipaddr.value.split(".");

            w_ipaddr = (pppoe_wan_ipaddr[0] << 24)
                        | (pppoe_wan_ipaddr[1] << 16)
                        | (pppoe_wan_ipaddr[2] << 8)
                        | (pppoe_wan_ipaddr[3]);
            w_netmask = l_netmask;
        }
        
        if (w_ipaddr
            && (l_netmask & w_netmask & l_ipaddr)
                == (l_netmask & w_netmask & w_ipaddr))
        {
            alert("<%192%>");
            return false;
        }

        /* checking whether DNS address as LAN address, no matter got automatically or not */
        if (w_netmask != 0)
        {
            var dns_pri_ipaddr = cf.wan_dns1_pri.value.split(".");
            var dns_sec_ipaddr = cf.wan_dns1_sec.value.split(".");
            var dns_pri_ipaddr2 = (dns_pri_ipaddr[0] << 24)
                                   | (dns_pri_ipaddr[1] << 16)
                                   | (dns_pri_ipaddr[2] << 8)
                                   | (dns_pri_ipaddr[3]);
            var dns_sec_ipaddr2 = (dns_sec_ipaddr[0] << 24)
                                   | (dns_sec_ipaddr[1] << 16)
                                   | (dns_sec_ipaddr[2] << 8)
                                   | (dns_sec_ipaddr[3]);
            
            /* first DNS conflict check */
            if (dns_pri_ipaddr2
                && (l_netmask & w_netmask & l_ipaddr)
                    == (l_netmask & w_netmask & dns_pri_ipaddr2))
            {
                alert("<%192%>");
                return false;
            }
            
            /* second DNS conflict check */
            if (dns_sec_ipaddr2
                && (l_netmask & w_netmask & l_ipaddr)
                    == (l_netmask & w_netmask & dns_sec_ipaddr2))
            {
                alert("<%192%>");
                return false;
            }
        }
        cf.lan_ipaddr.value = cf.LPethr1.value+'.'+cf.LPethr2.value+'.'+cf.LPethr3.value+'.'+cf.LPethr4.value;
        cf.lan_netmask.value = cf.LMask1.value+'.'+cf.LMask2.value+'.'+cf.LMask3.value+'.'+cf.LMask4.value;

        if (cf.dhcp_server.checked)
        {
            var lan_netaddr_min = (l_ipaddr & l_netmask) + 1;
            var lan_netaddr_max = ((l_ipaddr & l_netmask) | (~l_netmask)) - 1;
            var n1=parseInt(cf.sysPoolStartingAddr4.value,10);
            var n2=parseInt(cf.sysPoolFinishAddr4.value,10);
            
            if (isNaN(n1) || isNaN(n2)
                || (n1<1||n1>254||n2<1||n2>254||n1>n2)
                || ((n1 == n2) && (n1 == (l_ipaddr & 0xFF)))
                || (n1 < (lan_netaddr_min & 0xFF))
                || (n2 > (lan_netaddr_max & 0xFF))
                || ((cf.sysPoolStartingAddr4.value & cf.LMask4.value)
                        != (cf.LPethr4.value & cf.LMask4.value))
                || ((cf.sysPoolFinishAddr4.value & cf.LMask4.value)
                        != (cf.LPethr4.value & cf.LMask4.value)) ) {
                alert("<%193%>");
                return false;
            }
            cf.dhcp_start.value = cf.LPethr1.value+'.'+cf.LPethr2.value+'.'+cf.LPethr3.value+'.'+cf.sysPoolStartingAddr4.value;
            cf.dhcp_end.value = cf.LPethr1.value+'.'+cf.LPethr2.value+'.'+cf.LPethr3.value+'.'+cf.sysPoolFinishAddr4.value;
            cf.lan_proto.value = "dhcp";
        }
        else
        {
            cf.lan_proto.value = "static";
            alert("<%194%>");
        }
    }

    if (msg.length > 1)
    {
        alert(msg);
        return false;
    }

    if (cf.NATEnable.checked)
        cf.nat_mode.value = "1";
    else
        cf.nat_mode.value = "0";

    return true;
}

function checkData_ipAddress()
{
    var cf = document.forms["mainForm"];
    var msg = "";

    if (cf.device_name.value == "")
        msg+= "<%195%>\n";
    
    if (cf.device_name.value.match( /[^\x20-\x7E]/ ))
    {
        cf.device_name.focus();
        msg+= "<%196%>\n";
    }

    if(cf.ip_assign[1].checked)
    {
        if(checkIP(cf.ethr1,cf.ethr2,cf.ethr3,cf.ethr4,255))
            msg+= "<%161%>";
        else
        {
            cf.ethr1.value = parseInt(cf.ethr1.value,10);
            cf.ethr2.value = parseInt(cf.ethr2.value,10);
            cf.ethr3.value = parseInt(cf.ethr3.value,10);
            cf.ethr4.value = parseInt(cf.ethr4.value,10);
        }
        if(checkIP(cf.mask1,cf.mask2,cf.mask3,cf.mask4,255))
            msg+= "<%191%>";
        else
        {
            cf.mask1.value = parseInt(cf.mask1.value,10);
            cf.mask2.value = parseInt(cf.mask2.value,10);
            cf.mask3.value = parseInt(cf.mask3.value,10);
            cf.mask4.value = parseInt(cf.mask4.value,10);
        }
        if(checkIP(cf.gateway1,cf.gateway2,cf.gateway3,cf.gateway4,255))
            msg+= "<%197%>";
        else
        {
            cf.gateway1.value = parseInt(cf.gateway1.value,10);
            cf.gateway2.value = parseInt(cf.gateway2.value,10);
            cf.gateway3.value = parseInt(cf.gateway3.value,10);
            cf.gateway4.value = parseInt(cf.gateway4.value,10);
        }
        if(checkIP(cf.pri_addr1,cf.pri_addr2,cf.pri_addr3,cf.pri_addr4,254)||(parseInt(cf.pri_addr4.value,10)==0))
            msg+= "<%198%>";
        else
        {
            cf.pri_addr1.value = parseInt(cf.pri_addr1.value,10);
            cf.pri_addr2.value = parseInt(cf.pri_addr2.value,10);
            cf.pri_addr3.value = parseInt(cf.pri_addr3.value,10);
            cf.pri_addr4.value = parseInt(cf.pri_addr4.value,10);
        }
    }

    if (msg.length > 1)
    {
        alert(msg);
        return false;
    }

    cf.wan_ipaddr.value = cf.ethr1.value+'.'+cf.ethr2.value+'.'+cf.ethr3.value+'.'+cf.ethr4.value;
    cf.wan_netmask.value = cf.mask1.value+'.'+cf.mask2.value+'.'+cf.mask3.value+'.'+cf.mask4.value;
    cf.wan_gateway.value = cf.gateway1.value+'.'+cf.gateway2.value+'.'+cf.gateway3.value+'.'+cf.gateway4.value;
    cf.wan_dns1_pri.value = cf.pri_addr1.value+'.'+cf.pri_addr2.value+'.'+cf.pri_addr3.value+'.'+cf.pri_addr4.value;

    if (cf.ip_assign[1].checked)
    {
        if (cf.nat_mode.value == "0")
        {
            if (!confirm("<%199%>"))
                return false;
        }
    }

    if (cf.wire_mode.value == "1" && cf.nat_support.value == "1")
    {
        if (!checkData_nat())
            return false;
    }
    
    return true;
}

/**
* WPS section
*/
function setGray()
{
    var cf = document.forms["mainForm"];
    if((cf.wps_enable.value == "enabled") && (cf.auto_dis_pin.checked))
    {
        cf.num_of_entries.disabled = false;
    }
    else
    {
        cf.num_of_entries.disabled = true;
    }
}

function loadSettings_WPS()
{
    <%200%>
    
    var cf = document.forms["mainForm"];
    display_wps_attack_compromise();
        

    if (cf.wps_enable.value == "disabled")
    {
        cf.enable_pin.disabled = true;
        cf.auto_dis_pin.disabled = true;
        cf.num_of_entries.disabled = true;
        cf.keep_settings.disabled = true;
        WPS_wizard_grayout();
    }
    else
        WPS_wizard_ungrayout(); 
    
}

function checkData_WPS()
{
    var cf=document.forms["mainForm"];

    return true;
}

function showWpsAlert()
{
    var cf = document.forms["mainForm"];
    if(cf.show_wps_alert.value == "1")
        setTimeout("alert('<%80%>')",500);
}

function changeWscConfig(isChecked)
{
    var cf = document.forms["mainForm"];
    if(isChecked)
    {
        cf.keep_settings.checked = true;
    }
    else
    {
        cf.keep_settings.checked = false;
        cf.keep_unconfig.value = "1";
    }
}

function display_wps_attack_compromise()
{
    var cf = document.forms["mainForm"];
    if (cf.enable_pin.checked == true)
    {
        document.getElementById("wps_display").style.display="block";
        setGray();
    }
    else
    {
        document.getElementById("wps_display").style.display="none";
    }
}

function checkChannel()
{
    var cf = document.forms["mainForm"];
    var idx = cf.channel_5g.selectedIndex;
    var ch = cf.channel_5g.options[idx].value;

    if (ch < 52 || ch > 140)
        return true;

    alert("<%201%>");
    return true;
}

function dfs_alert()
{
    var block_ch_time = "<%202%>";
    var minute = Math.floor(block_ch_time / 60);
    var second = Math.floor(block_ch_time % 60);
    var msg = "<%105%>" + "<%203%>"
        + minute + "<%204%>" 
        + second + "<%205%>";
    alert(msg);
}

function showDFSAlert()
{
    var cf = document.forms["mainForm"];
    if(cf.show_dfs_alert.value == "1")
        setTimeout('dfs_alert()',500);
}
