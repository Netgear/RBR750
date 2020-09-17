<!-- hide script from old browsers
//this file is included in IA_finish.htm, IA_finish2.htm, WIZ_summary.htm

var maxchars = 10;
var delay_time = 5000; // msecs
var charcount = 0;

function updateProgress()
{
    var cf = document.forms[0];
    if("IA_finish_2" == cf.current_page.value){
        if(0 == cf.change_ether_port.value)
            delay_time = 2500; // msecs
    }
    if (charcount < maxchars)
    {
        charcount=charcount+0.5;
        setTimeout("updateProgress()",delay_time);
    }
    else
    {
        load_default(3);
        loadSettings();
        load_image();
        
        cf = document.forms[0];
    
        var security_type = cf.sec_type.value;
        var security_type_5g = cf.sec_type_5g.value;
        document.getElementById("countdown").style.display="none";
        document.getElementById("summary").style.display="block";
        document.getElementById("button").style.display="block";
        
        if (security_type == "WEP")
        {
            document.getElementById("wep").style.display="block";
            loadvalue(0);
        }
        else if(security_type == "WPA-PSK" || security_type == "WPA2-PSK" || security_type == "WPA/WPA2-PSK")
        {
            document.getElementById("wpa").style.display="block";
            loadvalue(1);
        }
        if (security_type_5g == "WEP")
        {
            document.getElementById("wep_5g").style.display="block";
            loadvalue(0);
        }
        else if(security_type_5g == "WPA-PSK" || security_type_5g == "WPA2-PSK" || security_type_5g == "WPA/WPA2-PSK")
        {
            document.getElementById("wpa_5g").style.display="block";
            loadvalue(1);
        }        
    }
}

function loadSettings()
{
    var checkbox = document.getElementById('checkbox');
    if(checkbox != null)
    {
        if(checkbox.checked)
        {
            var button = document.getElementById('continue_button');
            button.disabled = false;
            button.className = "short_common_bt";
        }
    }
    
    var cf = document.forms[0];
     if("IA_finish" == cf.current_page.value)
     {
        if("1" == cf.enable_ap_mode.value)
        {
            document.getElementById("ca_15_log").innerHTML = "<%3374%>";
            document.getElementById("24g_title").innerHTML = "<b><%3375%></b>";
            document.getElementById("5g_title").innerHTML = "<b><%3376%></b>";
            document.getElementById("ca_15_log_3").innerHTML = "<%3377%>";
        }
    }
    if("1" == cf.mode.value)
    {
        if("2.4G" == cf.apband.value)
        {
            document.getElementById("5g_title").style.display = "none";
            document.getElementById("5g_ssid").style.display = "none";
            document.getElementById("5g_sec").style.display = "none";
            document.getElementById("5g_passPhrase").style.display = "none";
            document.getElementById("24g_title").colspan = "2";
            document.getElementById("24g_ssid").colspan = "2";
            document.getElementById("24g_sec").colspan = "2";
            document.getElementById("24g_passPhrase").colspan = "2";
        }
        else if("5G" == cf.apband.value)
        {
            document.getElementById("24g_title").style.display = "none";
            document.getElementById("24g_ssid").style.display = "none";
            document.getElementById("24g_sec").style.display = "none";
            document.getElementById("24g_passPhrase").style.display = "none";
            document.getElementById("5g_title").colspan = "2";
            document.getElementById("5g_ssid").colspan = "2";
            document.getElementById("5g_sec").colspan = "2";
            document.getElementById("5g_passPhrase").colspan = "2";
        }
    }
}

function next_url()
{
    var cf = document.forms[0];
    change_url('IA_review_all.htm');
}

function copyToClipboard(txt) 
{ 
        var obj = document.getElementById(txt);
        window.clipboardData.clearData();   
        window.clipboardData.setData("Text",obj.innerText);
} 

function loadvalue(num)
{
        if (num == 0)
        {
            if (get_browser() == "IE")
            {
                document.getElementById('cpy1').style.visibility="visible";
                document.getElementById('cpy2').style.visibility="visible";
                document.getElementById('cpy3').style.visibility="visible";
                document.getElementById('cpy4').style.visibility="visible";
                document.getElementById('cpy_5g1').style.visibility="visible";
                document.getElementById('cpy_5g2').style.visibility="visible";
                document.getElementById('cpy_5g3').style.visibility="visible";
                document.getElementById('cpy_5g4').style.visibility="visible";
            }
            else
            {
                document.getElementById('cpy1').style.visibility="hidden";
                document.getElementById('cpy2').style.visibility="hidden";
                document.getElementById('cpy3').style.visibility="hidden";
                document.getElementById('cpy4').style.visibility="hidden";
                document.getElementById('cpy_5g1').style.visibility="hidden";
                document.getElementById('cpy_5g2').style.visibility="hidden";
                document.getElementById('cpy_5g3').style.visibility="hidden";
                document.getElementById('cpy_5g4').style.visibility="hidden";
            }
        }
        else if (num == 1)
        {
            if(get_browser() == "IE")
            {
                document.getElementById('cpy_wpa').style.visibility="visible";
                document.getElementById('cpy_wpa_5g').style.visibility="visible";
            }
            else
            {
                document.getElementById('cpy_wpa').style.visibility="hidden";
                document.getElementById('cpy_wpa_5g').style.visibility="hidden";
            }
        }
}
function buttonToggle()
{
     var button = document.getElementById('continue_button');
     var checkbox = document.getElementById('checkbox');
     if(checkbox.checked == true){
        button.disabled = false;
        button.className = "short_common_bt";
     }else{
        button.disabled = true;
        button.className = "short_common_gray_bt";
     }
}

function load_image()
{
 var sUserAgent = navigator.userAgent;  
 var fAppVersion = parseFloat(navigator.appVersion);  
 //test the Win Platform  
 var isWin = (navigator.platform == "Win32") || (navigator.platform == "Windows");  
    
 //test the Macintosh Platform  
 var isMac = (navigator.platform == "Mac68K") || (navigator.platform == "MacPPC")   
             || (navigator.platform == "Macintosh") || (navigator.platform == "MacIntel");  
   
 //test the Linux Platform 
 var isLin = (navigator.platform == "Linux i686") && !isWin && !isMac;  
   
 var isWin95 = isWin98 = isWinNT4 = isWin2K = isWinME = isWinXP = isWinVista = isWin7 = false;  
 var isMac68K = isMacPPC = isMacOS = false;  
 var isLinux = false;  

 //An accurate version of the type of platform
 if (isLin)
        isLinux =  sUserAgent.indexOf("Linux") > -1;  
 if (isWin) {  
     isWin95 = sUserAgent.indexOf("Win95") > -1   
               || sUserAgent.indexOf("Windows 95") > -1;  
     isWin98 = sUserAgent.indexOf("Win98") > -1   
               || sUserAgent.indexOf("Windows 98") > -1;  
     isWinME = sUserAgent.indexOf("Win 9x 4.90") > -1   
               || sUserAgent.indexOf("Windows ME") > -1;  
     isWin2K = sUserAgent.indexOf("Windows NT 5.0") > -1   
               || sUserAgent.indexOf("Windows 2000") > -1;  
     isWinXP = sUserAgent.indexOf("Windows NT 5.1") > -1   
               || sUserAgent.indexOf("Windows XP") > -1; 
     isWinVista = sUserAgent.indexOf("Windows NT 6.0") > -1   
               || sUserAgent.indexOf("Windows Vista") > -1;  
     isWin7 = sUserAgent.indexOf("Windows NT 6.1") > -1   
               || sUserAgent.indexOf("Windows 7") > -1;      
     isWinNT4 = sUserAgent.indexOf("WinNT") > -1   
               || sUserAgent.indexOf("Windows NT") > -1   
               || sUserAgent.indexOf("WinNT4.0") > -1   
                || sUserAgent.indexOf("Windows NT 4.0") > -1   
                && (!isWinME && !isWin2K && !isWinXP);                   
 }   

 //An accurate version of the type of platform  
 if (isMac) {  
     isMac68K = sUserAgent.indexOf("Mac_68000") > -1   
                || sUserAgent.indexOf("68K") > -1;  
     isMacPPC = sUserAgent.indexOf("Mac_PowerPC") > -1   
                || sUserAgent.indexOf("PPC") > -1; 
     isMacOS =  sUserAgent.indexOf("Mac OS") > -1;  
  }
  
    if(isWinVista || isWin7)
        document.getElementById("img_vista").style.display = "block";
    else if(isLinux)
        document.getElementById("img_linux").style.display = "block";
    else if(isMac)
        document.getElementById("img_mac").style.display = "block";
    else
        document.getElementById("img_xp").style.display = "block";
}
//-->