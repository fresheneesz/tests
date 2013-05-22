var scriptUri;
curScriptUrl(function(x)
{	scriptUri = x;
	alert(scriptUri);
});

function curScriptUrl(callback)
{	var scripts = document.getElementsByTagName("script");
	var scriptURI = scripts[scripts.length-1].src;	
	
	if(scriptURI != "")			// static include
	{	callback(scriptURI);
	}else if($ != undefined)	// jQuery ajax
	{	$(document).ajaxSuccess(function(e, xhr, s)
		{	callback(s.url);
		});	
	}
}
