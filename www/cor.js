var slogans = ["Praise the mutant!", "Rejoice in error!", "Embrace futility!", "Beware of sinister progs!", "Take up the two 8 ways!", "Use the ninth position wisely!"];
var colors = ['rgb(255, 0, 255)', 'rgb(0, 0, 255)', 'rgb(0, 255, 0)', 'rgb(255, 255, 0)'];

function pageInit(){
	window.setInterval(rotateBorderPallette, 200);
    window.setInterval(rotateSlogans, 5000);
}

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

function rotateSlogans()
{
    var slogan = slogans[Math.floor(Math.random() * slogans.length)];
	 $('#slogans').text(slogan);
}