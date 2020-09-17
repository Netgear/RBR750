function load_default(step)
{
    if (parent.stepframe != null)
        parent.stepframe.load_default(step);
}

function reset_init_scale()
{
    var viewport;
    viewport = parent.document.getElementById("viewport");
    if (viewport)
        viewport.content = "initial-scale=1.0"
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
    var ca_close_window="<%2706%>"
    if(get_browser() != "Firefox"){
        if (confirm(ca_close_window)){
            top.window.opener = null;
            top.window.open('','_self',''); 
            top.window.close();
        }
    }
}

function addfavorite()
{
   if (document.all)
   {
      window.external.addFavorite('http://www.mywifiext.net','<%12%> Configuration');
   }
   else if (window.sidebar)
   {
      window.sidebar.addPanel('<%12%> Configuration', 'http://www.mywifiext.net', "");
   }
}

function msg_close()
{
    var ca_add_bookmark="<%2707%>";
    
    if( get_browser() == "IE" || get_browser() == "Firefox")
    {
        if (confirm(ca_add_bookmark))
            addfavorite();
    }
    /*if( get_browser() == "Firefox")
        alert(ca_finish);
    else
        CloseWindow();*/
}
function pc_or_mobile(){
    var agent = navigator.userAgent.toLowerCase(); 
    if (agent.match(/iphone/i) || agent.match(/ipod/i)){
        if (document.cookie.indexOf("iphone_redirect = false") == -1)
            return 1;
    }
    else if (agent.match(/ipad/i)){
        return 0;
    }
    else if ((agent.match(/android/i) && agent.match(/mobile/i)) ||(agent.match(/android/i) && agent.match(/mozilla/i)) || (agent.match(/android/i)&& agent.match(/opera/i))){
        return 1;
    }
    else if (agent.match(/webos/i) || agent.match(/windows phone/i) || agent.match(/mqqbrowser/i)){
        return 1;
    }
    else if (agent.match(/ucweb/i) || agent.match(/ucbrowser/i)){
        return 1;
    }
    else if ((agent.match(/windows nt/i)) || (agent.match(/macintosh/i))){
        return 0;
    }
    else if (!!agent.match(/applewebkit.*mobile.*/)){
       return 1;
    }
    else 
       return 0;
}

