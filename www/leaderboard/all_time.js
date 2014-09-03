$(document).ready(function() {
    window.setInterval(rotateBorderPallette, 200);

    $('#scores_display').dataTable( {
        "aaData": scoreTable,
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
            }
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
