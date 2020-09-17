function load_default(step)
{
    if (parent.stepframe != null)
        parent.stepframe.load_default(step);
}

function get_browser()
{
        if(navigator.userAgent.indexOf("MSIE") != -1)
                return "IE";
        else if(navigator.userAgent.indexOf("Firefox") != -1)
                return "Firefox";
        else if(navigator.userAgent.indexOf("Chrome") != -1 )
                return "Chrome";		
        else if(navigator.userAgent.indexOf("Safari") != -1 )
                return "Safari";
        else if(navigator.userAgent.indexOf("Camino") != -1)
                return "Camino";
        else if(navigator.userAgent.indexOf("Gecko/") != -1)
                return "Gecko";
        else if(navigator.userAgent.indexOf("Trident") != -1)
                return "IE";
        else
                return "";
}

function CloseWindow()
{
    var ca_close_window="<%915%>"
    if(get_browser() != "Firefox"){
        if (confirm(ca_close_window)){
            top.window.opener = null;
            top.window.open('','_self',''); 
            top.window.close();
        }
    }
}

function hasBookmarkAPI()
{
    if (document.all && window.external)
        return true;

    if (window.sidebar && window.sidebar.addPanel)
        return true;

    return false;
}

function addfavorite()
{
    if (document.all && window.external)
        window.external.addFavorite('http://www.mywifiext.net','NETGEAR genie');
    else if (window.sidebar && window.sidebar.addPanel)
        window.sidebar.addPanel('NETGEAR genie', 'http://www.mywifiext.net', "");
}

function msg_close()
{
    var ca_add_bookmark="<%916%>";
    
    if( get_browser() == "IE" || get_browser() == "Firefox")
    {
        //if (confirm(ca_add_bookmark))
        //    addfavorite();
    }
    /*if( get_browser() == "Firefox")
        alert(ca_finish);
    else
        CloseWindow();*/
}
