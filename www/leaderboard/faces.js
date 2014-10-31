var currIndex = 0;
var numImage = 0;

var startIndex = 0;
var maxImage = 500;

function addImage() {
    var content = "";
    for (i = startIndex; i < currIndex; i++) {
        var img = scoreTable[i][3];
        var lnk = scoreTable[i][2].replace(" ", "+");
        content += '<a href="all_time.html?filter='+lnk+'" target="_blank"><img src="data/' + img + '" width="200" height="164"></img>';
    }
    $("#wallcontent").html(content);
}

function shuffle(o){ //v1.0
    for(var j, x, i = o.length; i; j = Math.floor(Math.random() * i), x = o[--i], o[i] = o[j], o[j] = x);
    return o;
};

$(document).ready(function() {
  scoreTable = shuffle(scoreTable);

  window.setInterval(rotateBorderPallette, 200);
  numImage = Math.floor($(document).width() / 200) + ($(document).height() / 164);
  numImage = Math.floor(4 * numImage);
  currIndex = numImage;
  addImage();

  $(window).scroll(function(){
      if ($(window).scrollTop() == $(document).height()-$(window).height()){
          currIndex += numImage;
          if (currIndex >= scoreTable.length)
              currIndex = scoreTable.length;
          var scrollTop = false;
          if (currIndex - startIndex > maxImage) {
              startIndex = currIndex;
              currIndex = startIndex + numImage;
              scrollTop = true;
          }
          addImage();
          if (scrollTop)
              window.scrollTo(0,0);
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
