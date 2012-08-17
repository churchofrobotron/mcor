
var colors = ['rgb(255, 0, 255)', 'rgb(0, 0, 255)', 'rgb(0, 255, 0)', 'rgb(255, 255, 0)'];

var currentLeader = 0;
var leaders = [];
var leaderboardLastModified = '';

function pageInit(){
	fetchAndUpdateLeaderboard();	//bootstrap initial state
	window.setInterval(fetchAndUpdateLeaderboard, 250);	//TODO: Dial this back when working (for faster updates)
	window.setInterval(showNextEntry, 2000);
	window.setInterval(rotateBorderPallette, 200);
}

function fetchAndUpdateLeaderboard(){
	jqxhr = $.ajax({ url: './data/leaderboard.txt', 'ifModified': true})
		.done(function(msg){
			if(msg){
				//console.log("%s", jqxhr.getResponseHeader('Last-Modified'));
				lastModified = jqxhr.getResponseHeader('Last-Modified');
				 if((leaderboardLastModified == lastModified) || (lastModified == null)){
					return;
				}
				parseDataFile(msg);
				updateDisplay();
				leaderboardLastModified = lastModified;
			}
		});

}

function updateDisplay(){
	//alert(JSON.stringify(leaders, null, 2));
	for(i=0; i < 20; i++){
		var content;
		if(i < leaders.length){
			content = leaders[i]['initials'] + ' ' + leaders[i]['score'];
		}
		else{
			content = '___ _______';
		}
		$('#slot' + (i+1)).html('' + (i+1) + ') ' + content);
	}
}

function parseDataFile(fileContent){
	var lines = fileContent.split('\n');
	leaders = [];
	clearCurrentEntry();
	for(i=0; i < lines.length; i++){
		if(lines[i].length){
			var line = lines[i];
			parts = line.split(',');
			leaders[i] = {'line': i, 'initials': parts[0], 'score': parts[1], 'image': parts[2]};
			if(i == 0){
				current = leaders[i];	//hold onto this, cuz we about to sort
			}
		}
	}
	leaders.sort(function(a,b){ return b['score'] - a['score'] });
	leaders = leaders.slice(0, 20, leaders);
	currentLeader = $.inArray(current, leaders);
	//alert('last updated leader is in slot ' + currentLeader);
	showCurrentEntry();
}

function showNextEntry(){
	clearCurrentEntry();
	currentLeader++;
	if(currentLeader >= leaders.length){
		currentLeader = 0;
	}
	showCurrentEntry();
}

function clearCurrentEntry(){
	$('#slot' + (currentLeader+1)).removeClass('currentslot');
}

function showCurrentEntry(){
	$('#slot' + (currentLeader+1)).attr('class', 'currentslot');
	$('#currentinitials').html(leaders[currentLeader]['initials'] + '<br/>' + leaders[currentLeader]['score']);
	$('#deathface1').css('background-image', 'url(data/' + leaders[currentLeader]['image'] + ')');
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
