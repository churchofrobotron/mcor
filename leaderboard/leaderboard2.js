
var colors = ['rgb(255, 0, 255)', 'rgb(0, 0, 255)', 'rgb(0, 255, 0)', 'rgb(255, 255, 0)'];

function parseDataFile(fileContent){
	var data = [];
	var lines = fileContent.split();
	for(i=0; i < lines.length; i++){
		var line = lines[i];
		parts = line.split(',');
		data[i] = {'rank': i, 'initials': parts[0], 'score': parts[1], 'image': parts[2]};
	}
	alert(new String(data));
}

function pageInit(){
	//$.ajax({ url: 'leaderboard.txt'})
	$.ajax({ url: './data/leaderboard.txt'})
		.done(parseDataFile);
	return;
	window.setInterval(rotateBorderPallette, 200);
}

function rotateBorderPallette(){
	nextColorForId('#border1');
	nextColorForId('#border2');
	nextColorForId('#border3');
	nextColorForId('#border4');
}

function nextColorForId(itemId){
	currentColor = $(itemId).css('border-color');
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
	return 'nathan!';
}
