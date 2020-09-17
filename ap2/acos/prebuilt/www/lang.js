var gui_lang_local;

function resetLanguageList()
{
    var i;
    var form=document.forms["langForm"];
    var suportlanguage = form.srptlang.value.split(" ");
    var gui_region = form.curlang.value;
    var langList = document.getElementById("showLangList");

    for(i=0; i < suportlanguage.length; i++)
    {
        if(suportlanguage[i] == gui_region)
        {
            langList.selectedIndex = i;
            return;
        }
    }
}

function ShowLanguageList()
{
    var form=document.forms["langForm"];
    var suportlanguage = form.srptlang.value.split(" ");
    var gui_region;
    var gui_lang = form.gui_lang.value;
    var local_srpt = form.local_srptlang.value.split(" ");

    if("<%512%>" == "1")
    {
        if("<%513%>" == "1")
            form.curlang.value = "Auto";
        else
            form.curlang.value = "<%160%>";
    }
    else
        form.curlang.value = "<%160%>";

    gui_region = form.curlang.value;

    for(i=0; i < suportlanguage.length; i++)
    {
        if(suportlanguage[i] == gui_region)
            document.write('<option value='+ gui_region +' selected >'+local_srpt[i]+'</option>');
        else
            document.write('<option value='+ suportlanguage[i] +'>'+local_srpt[i]+'</option>');

        if(suportlanguage[i] == gui_lang)
            gui_lang_local = local_srpt[i];
    }
}

function do_submit_0()
{
    document.forms["langForm"].submit();
}

function chgLang2()
{
    var form=document.forms["langForm"];
    var gui_region = form.curlang.value;
    var choice = form.show_lang_list.value;
    var choice_option = form.show_lang_list.options[form.show_lang_list.selectedIndex];
    var choice_text;
    var auto_block = form.auto_block.value;
    
    if (choice_option.text != undefined)
        choice_text = choice_option.text;
    else if (choice_option.innerText != undefined)
        choice_text = choice_option.innerText;
    else
        choice_text = choice;
    
    if (choice != gui_region){
        if(confirm("<%327%> "+ gui_lang_local + ""+"<%328%> "+""+ choice_text + "?")){
            form.targetlang.value = document.getElementById('showLangList').value;
            setTimeout("do_submit_0()",1000);
        }
        else
            resetLanguageList();
    }
}

function chgLang()
{
    //avoid ipad3+iOS7.0.3 crash issue
    setTimeout("chgLang2()",1);
}
