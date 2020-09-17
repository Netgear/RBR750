<!-- 

function setHelpIframeVisible(){
    $('.help-frame-div').css("visibility","visible");
}


function showHelpIframe() {
    var imgSrc=document.getElementById('help-button');

    if(imgSrc.src.search("up")>=0)
    {
        $(".help-frame-div").show();
        imgSrc.src="img/helparrowdown-icon.gif";
    }
    else
    {
        $(".help-frame-div").hide();
        imgSrc.src="img/helparrowup-icon.gif";
        setTimeout(setHelpIframeVisible,500);
    }
         change_size();

}

function moveToHTMLend()
{
    window.location.href='#helpframe-anchor';
    setHelpIframeVisible();
}


function loadhelp(fname,anchname)
{
/*
    var pane = window.frames["helpframe"].$('#content');
    var imgSrc=document.getElementById('help-button');
    if(imgSrc.src.search("up")<0)
    {

        $(".help-frame-div").show();
        pane.jScrollPane({showArrows:true});


        if ((loadhelp.arguments.length == 1 ) || (anchname == "" ))
        {
                window.frames["helpframe"].location.href=fname+"_h.htm";
                $(".help-frame-div").show();
        }
        else
        {
                window.frames["helpframe"].location.href=fname+"_h.htm#" + anchname;
                $(".help-frame-div").show();
        }

        $(".help-frame-div").show();
        pane.jScrollPane({showArrows:true});

    }
    */
    if(!$(".help-frame-div").is(':visible'))
        showHelpIframe();
    /*
    if ((loadhelp.arguments.length == 1 ) || (anchname == "" ))
    {
        //window.frames["helpframe"].location.href=fname+"_h.htm";
        $("#helpframe").attr('src', fname+"_h.htm");
    }
    else
    {
        //window.frames["helpframe"].location.href=fname+"_h.htm#" + anchname;
        $("#helpframe").attr('src', fname+"_h.htm#" + anchname);
    }*/
    if ((loadhelp.arguments.length == 1 ) || (anchname == "" ))
    {
        var y=$("#helpframe").position().top;
        $('#helpframe').contents().find(".jspPane").css('top',"0px");
    }
    else
    {
        if($("#helpframe").contents().find("a[name="+anchname+"]").length)
            target_top=$("#helpframe").contents().find("a[name="+anchname+"]").position().top;
        else if($("#helpframe").contents().find("#"+anchname).length)
            target_top=$("#helpframe").contents().find("#"+anchname).position().top;
        else
            target_top=0;
        y=$("#helpframe").position().top + target_top;

        y=y-10;
        if(y<0)
            y=0;
        
        $('#helpframe').contents().find(".jspPane").css('top',"-"+y+"px");
        
    }

    
}

//-->