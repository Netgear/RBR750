
function showQuestion1()
{
    	var selected = [ "", "", "", "", "", "", "", "", "", ""];
    	
    	selected[0] = "selected";
    	
    	document.open();
    	document.write("<select name=\"question1\" id=\"question1\">");
    		document.write("<option " + selected[0] + " value=\"\"><%333%></option>");
    		document.write("<option " + selected[1] + " value=\"1\"><%334%></option>");
    		document.write("<option " + selected[2] + " value=\"2\"><%335%></option>");
    		document.write("<option " + selected[3] + " value=\"3\"><%336%></option>");
    		document.write("<option " + selected[4] + " value=\"4\"><%337%></option>");
    		document.write("<option " + selected[5] + " value=\"5\"><%338%></option>");
    		document.write("<option " + selected[6] + " value=\"6\"><%339%></option>");
    		document.write("<option " + selected[7] + " value=\"7\"><%340%></option>");
    		document.write("<option " + selected[8] + " value=\"8\"><%341%></option>");
    		document.write("<option " + selected[9] + " value=\"9\"><%342%></option>");
    	document.write("</select>");
    	document.close();
    	return true;
}
function showQuestion2()
{
 var selected = [ "", "", "", "", "", "", "", "", "", ""];
    	
 	selected[0] = "selected";
    	
  	document.open();
  	document.write("<select name=\"question2\" id=\"question2\">");
    document.write("<option " + selected[0] + " value=\"\"><%343%></option>");
   	document.write("<option " + selected[1] + " value=\"1\"><%344%></option>");
   	document.write("<option " + selected[2] + " value=\"2\"><%345%></option>");
   	document.write("<option " + selected[3] + " value=\"3\"><%346%></option>");
   	document.write("<option " + selected[4] + " value=\"4\"><%347%></option>");
    document.write("<option " + selected[5] + " value=\"5\"><%348%></option>");
  	document.write("<option " + selected[6] + " value=\"6\"><%349%></option>");
   	document.write("<option " + selected[7] + " value=\"7\"><%350%></option>");
  	document.write("<option " + selected[8] + " value=\"8\"><%351%></option>");
    document.write("</select>");	
    document.close();
   	
  	return true;
}

function recoveryQuestion1()
{
	var questions = [	"<%333%>",
						"<%334%>",
						"<%335%>",
						"<%336%>",
						"<%337%>",
						"<%338%>",
						"<%339%>",
						"<%340%>",
						"<%341%>",
						"<%342%>"	];
						
	document.write(questions[<%352%>]);				
	return true;
}
function recoveryQuestion2()
{
	var questions = [	"<%343%>",
						"<%344%>",
						"<%345%>",
						"<%346%>",
						"<%347%>",
						"<%348%>",
						"<%349%>",
						"<%350%>",
						"<%351%>"	];
						
	document.write(questions[<%353%>]);				
	return true;
}
