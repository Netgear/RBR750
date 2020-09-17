/* for WPS 2.0 checking */
var wps_version = "<%917%>";  
var show_open_or_wps_v20_msg = false;

// ---------------------------- Start of  WEP functions ----------------------------
var lastkeypressed;
var keyTooBig = false;
var keyTooBig_5g = false;
var mustbeHEX = false;
var keysize;
var keysize_5g;
var lastObj;
var lastObj_5g;

if (document.layers) document.captureEvents(Event.KEYPRESS);
    document.onkeypress = checkKey;
function checkKey(evt)
{
    evt = (evt) ? evt : ((window.event) ? window.event : null);
    if (evt)
        lastkeypressed = (evt.keyCode) ? evt.keyCode : (evt.which ) ? evt.which : null;
    if ((lastkeypressed != 13) && (lastkeypressed != 8) && ( keyTooBig || keyTooBig_5g) )
    {
        if(keyTooBig==true)
        alert("<%918%> " + keysize + " <%919%>");
        if(keyTooBig_5g==true)
        alert("<%918%> " + keysize_5g + " <%919%>");
        keyTooBig = false;
        keyTooBig_5g = false;
        chkSize(lastObj); // for NS 6/7
        chkSize_5g(lastObj_5g);
        return false;
    }
    else if (mustbeHEX) // not used here, don't know which input is being used
    {
        mustbeHEX = false;
        if ( ((lastkeypressed > 47) && (lastkeypressed < 58 ))
            || ((lastkeypressed > 96) && (lastkeypressed < 103))
            || ((lastkeypressed > 64) && (lastkeypressed < 71 ))
            || (lastkeypressed == 8)
            || (lastkeypressed == 13) )
            return true; // OK
        else
            return false;
    }
    return true;
}


function chkSize(fobj)
{
    if(!fobj || typeof(fobj) == "undefined")
        return false;

    if(fobj.value.length > keysize)
        fobj.value = fobj.value.substr(0,keysize);
}

function chkSize_5g(fobj)
{
    if(!fobj || typeof(fobj) == "undefined")
        return false;

    if(fobj.value.length > keysize_5g)
        fobj.value = fobj.value.substr(0,keysize_5g);
}

function CheckData_NONE()
{
    if (wps_version == "2.0")
    {
        if (!show_open_or_wps_v20_msg) {
            if (!confirm("<%920%>")) {
                return false;
            }
        }
        show_open_or_wps_v20_msg = true;
    }
    return true;
}

function CheckData_WEP(wepenc, wep_key_no, key, key1, key2, key3, key4)
{
    var msg_key64_type = "<%921%>";
    var msg_key128_type = "<%922%>";
    
    var msg = ""; var blankFlag = false; var sizeOK = false; var i;
    var len = (wepenc.selectedIndex==0)?10:((wepenc.selectedIndex==1)?26:32);
    for (i = 1; i < 5; i++ )
    {
        msg = "";
        blankFlag = ! (eval(wep_key_no[i-1].checked)); // default key
        if( (blankFlag) && (eval("(key"+ i + ".value.length == 0)")) )
            continue; // blank ok
        sizeOK = (((eval("isHex(key" + i +".value)")) && (eval("(key" + i + ".value.length == len)"))) || (eval("(key" + i + ".value.length == len/2)")));
        if (!sizeOK)
        {
            msg += (len == 10)?msg_key64_type:msg_key128_type;
            alertR(msg);
            if (i==1)
                key1.focus();
            else if (i==2)
                key2.focus();
            else if (i==3)
                key3.focus();
            else if (i==4)
                key4.focus();
            return false;
        }
    }
    
    return true;
}


function keyCheck(fobj)
{
    lastObj = fobj;
    keyTooBig = (fobj.value.length >= keysize ) ? true : false;
}

function keyCheck_5g(fobj)
{
    lastObj_5g = fobj;
    keyTooBig_5g = (fobj.value.length >= keysize_5g ) ? true : false;
}

function calcPassphrase(wepenc, passphraseStr, wep_key_no, key1, key2, key3, key4)
{
    if(passphraseStr.value.length == 0)
        return;
    switch(wepenc.selectedIndex)
    {
        case 0:
            PassPhrase40(passphraseStr, wep_key_no, key1, key2, key3, key4);
            break;
        case 1:
            PassPhrase104(passphraseStr, wep_key_no, key1, key2, key3, key4);
            break;
        default:
            break;
    }
}

function PassPhrase40(passphraseStr, wep_key_no, key1, key2, key3, key4)
{
    var seed = 0;
    var pseed = new Array(0, 0, 0, 0);
    var pkey = new Array(4);
    var asciiObj = new Array(4);
    Length = passphraseStr.value.length;
    if(Length != 0)
    {
        for (i=0; i<Length; i++ )
            pseed[i%4] ^= passphraseStr.value.charCodeAt(i);
        seed = pseed[0];
        seed += pseed[1] << 8;
        seed += pseed[2] << 16;
        seed += pseed[3] << 24;
    }
    key1.value = key2.value = "";
    key3.value = key4.value = "";
    pkey[0] = key1;
    pkey[1] = key2;
    pkey[2] = key3;
    pkey[3] = key4;
    for(j=0; j<4; j++)
    {
        for (i=0; i<5 ;i++ )
        {
            seed = (214013 * seed) & 0xffffffff;
            if(seed & 0x80000000)
                seed = (seed & 0x7fffffff) + 0x80000000 + 0x269ec3;
            else
                seed = (seed & 0x7fffffff) + 0x269ec3;
            temp = ((seed >> 16) & 0xff);
            if(temp < 0x10)
                pkey[j].value += "0" + temp.toString(16).toUpperCase();
            else
                pkey[j].value += temp.toString(16).toUpperCase();
        }
    }
    wep_key_no[0].checked = true;
}

function PassPhrase104(passphraseStr, wep_key_no, key1, key2, key3, key4)
{
    var pseed2 = "";
    Length2 = passphraseStr.value.length;
    var asciiObj = "";
    for(p=0; p<64; p++)
    {
        tempCount = p % Length2;
        pseed2 += passphraseStr.value.substring(tempCount, tempCount+1);
    }
    md5Str = calcMD5(pseed2);
    key1.value = md5Str.substring(0, 26).toUpperCase();
    key2.value = key1.value;
    key3.value = key1.value;
    key4.value = key1.value;
    wep_key_no[0].checked = true;
    //F.KEY2.disabled = F.KEY3.disabled = F.KEY4.disabled = true;
}

function setKeys(wepenc, key1, key2, key3, key4, band)
{
    //key1.disabled = key2.disabled = key3.disabled = key4.disabled = false;
    if (wepenc.selectedIndex == 0)
    {
        if(band==2)
        keysize = 10;
        else if(band==5)
        keysize_5g = 10;
        
        key1.size = key2.size = key3.size = key4.size = 10 + 2 ; // extra for Mac
        if (key1.value.length>10)
            key1.value = key1.value.substring(0,10);
        if (key2.value.length>10)
            key2.value = key2.value.substring(0,10);
        if (key3.value.length>10)
            key3.value = key3.value.substring(0,10);
        if (key4.value.length>10)
            key4.value = key4.value.substring(0,10);
    }
    else if(wepenc.selectedIndex == 1)
    {
        if(band==2)
        keysize = 26;
        else if(band==5)
        keysize_5g = 26;
        
        key1.size = key2.size = key3.size = key4.size = 26 + 2; // extra for Mac
        if (key1.value.length>26)
            key1.value = key1.value.substring(0,26);
        if (key2.value.length>26)
            key2.value = key2.value.substring(0,26);
        if (key3.value.length>26)
            key3.value = key3.value.substring(0,26);
        if (key4.value.length>26)
            key4.value = key.value.substring(0,26);
    }
    else if(wepenc.selectedIndex == 2)
    {
        if(band==2)
        keysize = 32;
        else if(band==5)
        keysize_5g = 32;
        
        key1.size = key2.size = key3.size = key4.size = 32 + 2; // extra for Mac
    }
    //auto_disable();
}
// ---------------------------- End of  WEP functions ----------------------------

function CheckData_WPA(passphrase)
{
    if(passphrase.value.length < 8)
    {
        alert("<%874%>");
        passphrase.focus();
        return false;    
    }
    if(passphrase.value.length > 63)
    {
        if ( isHex(passphrase.value) == false)
        {
            alert("<%874%>");
            //passphrase.value = "";
            passphrase.focus();
            return false;
        }    
    }
    
    if (passphrase.value.match( /[^\x20-\x7E]/ ))
    {
        alert("<%923%>");
        passphrase.focus();
        return false;
    }
    
    return true;
}

function CheckData_Manual(ssid, security_type, passphrase, wepenc, wep_key_no, key, key1, key2, key3, key4)
{
    var msg_key64_type = "<%921%>";
    var msg_key128_type = "<%922%>";

    var security_type_value = security_type.value;    
    var msg = "";

    if (ssid.value == "")
    {
        alert("<%480%>");
        ssid.focus();
        return false;
    }

    if (ssid.value.match( /[^\x20-\x7E]/ ))
    {
        alert("<%481%>");
        ssid.focus();
        return false;
    }
    
    if (security_type_value == -1) // not selected
    {
        alert("<%482%>");
        security_type.focus();
        return false;
    }
    else if (security_type_value == 1) // WEP
    {
        return CheckData_WEP(wepenc, wep_key_no, key, key1, key2, key3, key4);
    }
    else if (security_type_value == 2 || security_type_value == 3 || security_type_value == 4) //WPA-PSK/WPA2-PSK
    {
        return CheckData_WPA(passphrase);
    }    
    return true;
}

