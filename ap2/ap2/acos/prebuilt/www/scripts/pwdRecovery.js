
function showQuestion1()
{
    	var selected = [ "", "", "", "", "", "", "", "", "", ""];
    	
    	selected[0] = "selected";
    	
    	document.open();
    	document.write("<select name=\"question1\" id=\"question1\">");
    		document.write("<option " + selected[0] + " value=\"\"><%1010%></option>");
    		document.write("<option " + selected[1] + " value=\"1\"><%1011%></option>");
    		document.write("<option " + selected[2] + " value=\"2\"><%1012%></option>");
    		document.write("<option " + selected[3] + " value=\"3\"><%1013%></option>");
    		document.write("<option " + selected[4] + " value=\"4\"><%1014%></option>");
    		document.write("<option " + selected[5] + " value=\"5\"><%1015%></option>");
    		document.write("<option " + selected[6] + " value=\"6\"><%1016%></option>");
    		document.write("<option " + selected[7] + " value=\"7\"><%1017%></option>");
    		document.write("<option " + selected[8] + " value=\"8\"><%1018%></option>");
    		document.write("<option " + selected[9] + " value=\"9\"><%1019%></option>");
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
    document.write("<option " + selected[0] + " value=\"\"><%1020%></option>");
   	document.write("<option " + selected[1] + " value=\"1\"><%1021%></option>");
   	document.write("<option " + selected[2] + " value=\"2\"><%1022%></option>");
   	document.write("<option " + selected[3] + " value=\"3\"><%1023%></option>");
   	document.write("<option " + selected[4] + " value=\"4\"><%1024%></option>");
    document.write("<option " + selected[5] + " value=\"5\"><%1025%></option>");
  	document.write("<option " + selected[6] + " value=\"6\"><%1026%></option>");
   	document.write("<option " + selected[7] + " value=\"7\"><%1027%></option>");
  	document.write("<option " + selected[8] + " value=\"8\"><%1028%></option>");
    document.write("</select>");	
    document.close();
   	
  	return true;
}

function recoveryQuestion1()
{
	var questions = [	"<%1010%>",
						"<%1011%>",
						"<%1012%>",
						"<%1013%>",
						"<%1014%>",
						"<%1015%>",
						"<%1016%>",
						"<%1017%>",
						"<%1018%>",
						"<%1019%>"	];
						
	document.write(questions[<%1029%>]);				
	return true;
}
function recoveryQuestion2()
{
	var questions = [	"<%1020%>",
						"<%1021%>",
						"<%1022%>",
						"<%1023%>",
						"<%1024%>",
						"<%1025%>",
						"<%1026%>",
						"<%1027%>",
						"<%1028%>"	];
						
	document.write(questions[<%1030%>]);				
	return true;
}
