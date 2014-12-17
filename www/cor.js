//vi:sw=2 ts=2 expandtab
var slogans = ["Praise the mutant!", "Rejoice in error!", "Embrace futility!", "Beware of sinister progs!", "Take up the two 8 ways!", "Use the ninth position wisely!"];
var colors = ['rgb(255, 0, 255)', 'rgb(0, 0, 255)', 'rgb(0, 255, 0)', 'rgb(255, 255, 0)'];
var typed_page_timeout = 5000;
var paragrah_delay = 500;

var text_attrs = [
  {
    index:0,
    interval: 500,
      fade: 400,
    colors: ['rgb(255, 0, 255)', 'rgb(0, 0, 255)', 'rgb(0, 255, 0)', 'rgb(255, 255, 255)']
  },
  {
    index:0,
    interval: 100,
    fade: 90,
    colors: ['rgb(255, 0, 255)', 'rgb(0, 0, 255)', 'rgb(0, 255, 0)', 'rgb(255, 255, 0)']
  },
  {
    index:0,
    interval: 200,
    fade: 150,
    colors: ['rgb(255, 0, 0)', 'rgb(255, 255, 255)']
  }
];

function pageInit(){
  window.setInterval(rotateBorderPallette, 200);
  window.setInterval(rotateSlogans, 5000);
}

function animatedSermonInit() {
  jQuery.fx.interval = 50;
  jQuery.each(text_attrs, function(index, value) {
    window.setInterval(function(){ pulseText(index); }, value.interval);
  });
  window.setInterval(function() {
    $('html, body').animate({scrollTop:$(document).height()}, 'fast');
  }, 500);
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

function pulseText(group)
{
  var colors = text_attrs[group].colors;
  var fade = text_attrs[group].fade;
  var cindex = text_attrs[group].index;
  text_attrs[group].index = (cindex + 1) % colors.length;
  var c = colors[cindex];
  $(".rt" + String(group)).animate({color:c}, fade);
}

//textarray is actually just one long string
//break apart elements in array with pipes, optional arguments after pipe
//|BR gives a line break before text, line breaks also insert a delay
//|n= [0,1,2,3] puts the following text into that text group, default is zero,
//so |1= makes the following text flash a lot.
//^n anywhere, ie ^500 adds a delay of n milliseconds
//|XXX pauses for pauses typed_page_timeout and then clears the page and starts
//fresh
//
//all content is appended to the html element specified by append_element
function typetext(append_element, textarray, donecallback) {
  var shit = textarray.split("|");
  var f = function(index) {
    if (index < shit.length)
      type(append_element, shit[index], function() { f(index + 1); } );
    else {
      donecallback();
    }
  };
  f(0);
}

function type(append_element, text, donefunc) {
  var group = 0;
  var element = document.createElement("span");
  var re = /^(\d)=/;
  var br = false;
  if (/BR/.exec(text)) {
    $(append_element).append("<br><br>");
    text = text.substr(2);
    br = true;
  }

  var test = re.exec(text);
  if (test) {
    group = test[1];
    text = text.substr(2);
  }
  if (/XXX/.exec(text)) {
    var x = function() {
      $(append_element).empty();
      donefunc();
    };
    setTimeout(x, typed_page_timeout);
  } else {
    if (br && !/\s*\^/.exec(text)) {
      text = "^" + paragrah_delay + text;
    }
    $(element).addClass("rt" + String(group));
    //$(element).data("group", group);
    $(append_element).append(element);
    $(element).typed({
      strings: [text],
      typeSpeed: 0,
      showCursor: false,
      callback: donefunc
    });
  }
}

//slop overflow:
//http://stackoverflow.com/questions/19491336/get-url-parameter-jquery
function getUrlParameter(sParam)
{
  var sPageURL = window.location.search.substring(1);
  var sURLVariables = sPageURL.split('&');
  for (var i = 0; i < sURLVariables.length; i++) 
  {
    var sParameterName = sURLVariables[i].split('=');
    if (sParameterName[0] == sParam) 
    {
      return sParameterName[1];
    }
  }
  return null;
}
