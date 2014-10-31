var currIndex = 0;
var numImage = 0;

function addImage() {
    if (currIndex < scoreTable.length) {
        var img = scoreTable[currIndex][3];
        var lnk = scoreTable[currIndex][2].replace(" ", "+");
        $("#wallcontent").append('<a href="all_time.html?filter='+lnk+'" target="_blank"><img src="data/' + img + '" width="200" height="164"></img>');
        currIndex++;
    }
}

function shuffle(o){ //v1.0
    for(var j, x, i = o.length; i; j = Math.floor(Math.random() * i), x = o[--i], o[i] = o[j], o[j] = x);
    return o;
};

$(document).ready(function() {
  scoreTable = shuffle(scoreTable);

  window.setInterval(rotateBorderPallette, 200);
  numImage = ($(document).width() / 200) + ($(document).height() / 164);
  numImage *= 4;
  for (i = 0; i < numImage; i++)
      addImage();

  $(window).scroll(function(){
      if ($(window).scrollTop() == $(document).height()-$(window).height()){
          for (i = 0; i < numImage; i++)
              addImage();
      }});
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
