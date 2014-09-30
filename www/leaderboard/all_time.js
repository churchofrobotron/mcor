function getUrlParam( name )
{
  name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
  var regexS = "[\\?&]"+name+"=([^&#]*)";
  var regex = new RegExp( regexS );
  var results = regex.exec( window.location.href );
  if( results == null )
    return "";
  else
    return results[1].replace("+", " ");
}

$(document).ready(function() {
    window.setInterval(rotateBorderPallette, 200);

    var isearch = getUrlParam("filter");

    $('#scores_display').dataTable( {
        "aaData": scoreTable,
        "oSearch" : {
            "sSearch" : isearch
        },
        "aoColumns" : [
            { "sTitle": "Initials", "sWidth": "10em" },
            { "sTitle": "Scores", "sWidth": "10em" },
            { "sTitle": "Date", "sWidth" : "15em" },
            {
                "sTitle": "Death Face",
                "sClass": "center",
                "fnRender": function(obj) {
                    var sReturn = obj.aData[ obj.iDataColumn ];
                    sReturn = "<img src='data/" + sReturn + "' width='176' height='144'></img>"
                    return sReturn;
                }
            },
            { "sTitle": "Location", "sWidth" : "10em" }
        ],
        "aaSorting" : [[1, "desc"]],
        "bDeferRender" : true,
        "bLengthChange" : false,
        "bAutoWidth" : false
    });
});

var colors = ['rgb(255, 0, 255)', 'rgb(0, 0, 255)', 'rgb(0, 255, 0)', 'rgb(255, 255, 0)'];

function rotateBorderPallette(){
	nextColorForId('#border1');
	nextColorForId('#border2');
	nextColorForId('#border3');
	nextColorForId('#border4');
}

function nextColorForId(itemId){
	currentColor = $(itemId).css('border-color');
    if (currentColor == "")
        currentColor = $(itemId).css('border-left-color');
	next = nextColor(currentColor);
	$(itemId).css('border-color', next);
}

function nextColor(current){
	for(i=0; i < colors.length; i++){
		nextIndex = i+1;
		if(nextIndex >= colors.length){
			nextIndex = 0;
		}
		if(colors[i] == current){
			return colors[nextIndex];
		}
	}
	 return colors[0];
}
