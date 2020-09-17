<!-- 
function check_top_url()
{
    if(top.location.href.indexOf("www.mywifiext.com") > 0)
        return "www.mywifiext.com";
    else if (top.location.href.indexOf("www.mywifiext.net") > 0)
        return "www.mywifiext.net";
    else if(top.location.href.indexOf("mywifiext.com") > 0)
        return "mywifiext.com";
    else if (top.location.href.indexOf("mywifiext.net") > 0)
        return "mywifiext.net";
    else
        return "";
}

function change_url(file_name)
{
    
    if("www.mywifiext.com" == check_top_url())
        top.location.href = "http://www.mywifiext.net" + "/" + file_name;
    else if("www.mywifiext.net" == check_top_url())
        top.location.href = "http://mywifiext.com" + "/" + file_name;
    else if("mywifiext.com" == check_top_url())
        top.location.href = "http://mywifiext.net" + "/" + file_name;
    else if("mywifiext.net" == check_top_url())
        top.location.href = "http://www.mywifiext.com" + "/" + file_name;
    else 
        top.location.href = file_name;
}


//-->