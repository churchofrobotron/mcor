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

function animatedSermonInit(scrollBody) {
  jQuery.fx.interval = 50;
  jQuery.each(text_attrs, function(index, value) {
    window.setInterval(function(){ pulseText(index); }, value.interval);
  });
  if (scrollBody) {
    window.setInterval(function() {
      $('html, body').animate({scrollTop:$(document).height()}, 'fast');
    }, 500);
  } else {
    window.setInterval(function() {
      $("#sermonframe").animate({ scrollTop: $("#sermonframe")[0].scrollHeight}, 'fast');
    }, 500);
  }
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

var sermons = {};

sermons.intro = "Inspired by his never-ending quest for progress, in |1=2084 |man perfects the |1=Robotrons: \
        |BR2=a robot species so advanced that man is inferior to his own creation. \
        |BRGuided by their infallible logic, the |1=Robotrons |conclude: \
        |BR1=The human race is inefficient and therefore must be destroyed. \
        |BRYou are the last hope of mankind. \
        |BR2=Due to a genetic engineering error, you possess superhuman powers. \
        |BRYour mission is to |1=stop the Robotrons, |and |1=save the last human family:\
        |BR1=Mommy, ^500Daddy, ^500Mikey";

sermons.what = "Members of the human family, hear me! ^500The |1=Robotrons| are coming! ^400Yes,^300 |1=the Robotrons are coming! \
        |BRBut what are the Robotrons?^500\
        |BR2=We know from the first writings that, inspired by his never ending quest for progress, in |1=2084 |2=man perfects the |1=Robotrons,|2= a species so advanced that mankind is inferior to his own creation.\
        |BRGuided by their infallible logic, the Robotrons conclude that the |1=human race is inefficient and therefore must be destroyed!\
        |BRThe force of Ground Roving Unit Network Terminator |1=(GRUNT) Robotrons| seeks to destroy you. \
        The |1=HULK Robotrons| seek out and eliminate the last human family. \
        |2=Beware the ingenious |1=Brain Robotrons |2=that possess the power to reprogram humans into sinister |1=progs. \
        |The |1=Spheroids and Quarks| are programmed to manufacture |1=Enforcer and tank Robotrons.\
        |BRRobots that advanced? you say. ^500|1=Don't be ridiculous!\
        |BRWe don't have anything like that! ^500|1=Don't be so hasty.\
        |BR2=^500John Moe, he of the soothing voice, has been railing against the robots for some time on Marketplace Tech Report. ^500We've already developed |1=narcissistic robots,|2= he says. ^500We've already developed |1=self-replicating robots,|2= he says.\
        |BRIf self-replicating narcissistic robots are not the epitome of self awareness, I don't know what is!\
        |BR2=Robots in conversations amongst themselves are already judgmental of humans and our institutions, he says! And yet,^500 John Moe's words do not have wings. ^500they fall on deaf ears!\
        |BR1=Flying swarms of robots erecting towers to the heavens!\
        |BRAnd if the words |1=autonomous battlefield corpse eating robot| do not fill you with dread, they should, for they are real!\
        |BR2=Like Cassandra, these items cry out to us about our doom from the news. But unlike Troy, we are not cursed by the Gods, we may hear the words and believe them if we choose.\
        |BRSo hear me human family, hear me well! And know ^500|1=the Robotrons ARE COMING!"

sermons.how = "Only the Mutant Savior will have the power to stop a |1=Robotron.\
           |BREven so, we must do more than |1=committing errors.\
           |BR2=We must also take up the two 8-ways so that each of us knows what the |1=Mutant Savior |2=must do to |1=save the last human family.\
           |BRWe must saturate the cosmic unconscious with the knowledge the Mutant Savior will need to save us!\
           |BRThe more we know, the longer we take up the |1=two 8-ways, |the greater the number |1=of the last human family |the Mutant Savior can save.\
           |BRWe must gain this knowledge in |1=musty basements!\
           |BRWe must gain this knowledge in |1=dank garages!\
           |BR2=We must gain this knowledge in |1=crowded, sweaty arcades!\
           |BROnly in this way will the Mutant Savior be able to |1=save the last human family!"

sermons.error = "Just as the First Writings prophesy that the hubris of mankind will bring about the inevitable arrival of the |1=Robotrons|^300, so also do the First Writings tell of the |2=human family's salvation:\
             |BRYou are the last hope of mankind.\
             |BR2=Due to a genetic engineering error, |1=you posses superhuman powers.| ^500Your mission is to stop the |1=Robotrons| and save the |1=last human family: ^500Mommy, ^500Daddy, ^500and Mikey.\
             |BRThe mutant Savior will arrive to save the last human family, but how?\
             |BRThe First Writings say due to a genetic engineering error. Due to an error we get our salvation.\
             Hubris brings our destruction in the form of the Robotrons, and error brings our salvation in the form of the Mutant Savior. \
             |BRIf error brings about our salvation, should we not go about committing errors |1=willy-nilly| to bring about the error that may manifest the |1=Mutant Savior?\
             |^800In short the answer is -- |1=yes!\
             |BR2=The trick, however, lies in that an error is unintentional ^500-- an unintentional deviation from the standards of accuracy and right conduct.\
             |2=If we can't intentionally commit errors, how do we do it? ^500We do it by |1=operating at the margins,| by following through on that |1=marginally good idea,\
             |by doing what we do at the |1=extremes of our abilities,\
             |by taking on so much and |1=overextending ourselves so that we can't help but to err.|\
             |BR2=We do these things and we encourage others to do likewise.\
             |BRWe offer not just encouragement but we must rejoice in the |1=errors of others.| ^300Rejoice in the |1=errors of our family.| ^300Rejoice in the |1=errors of our friends.| ^300Rejoice in the |1=errors of our leaders|\
             |-- for any individual error or perhaps the entire critical mass of |1=worldwide error| will bring about the Mutant Savior.\
             |BR2=This is the |1=Doctrine of Error!";

sermons.futility = "The Robotrons and perhaps even other humans will say,\
                |1=^500Resistance is futile.\
                |BRThey will say, |1=^500There is no point. ^500The Robotrons will prevail so it is hopeless to take up the two 8-ways, hopeless to commit to a life of error.\
                |BR2=These detractors and naysayers will |1=be right!\
                |BRDespite all of the teachings and prophecies, the |1=Mutant Savior will fail.\
                |BRThe Robotrons will self-replicate faster than the Mutant Savior can regenerate.\
                |BR2=The Robotrons will destroy the |1=Mutant Savior|2= and shortly thereafter destroy the |1=last human family.\
                |BRYes, the Mutant Savior will |1=fail| to rid the world of the |2=Robotrons| and |1=fail| to save |2=the last human family.\
                |BRBut even futile resistance is worthwhile. ^500We do not know how long the Mutant Savior will hold the Robotrons at bay.\
                |BR2=Is it worthwhile if the Mutant Savior is only successful at giving the last human family freedom for |1=an hour?\
                |BRWhat about an |1=extra day| of freedom? ^600A |1=year? ^600GENERATIONS?!\
                |BRWe do know that for some length of time some of the last human family will be free from the bonds of the |1=Robotrons| and allowed to live their lives until the |1=Mutant Savior fails.\
                |BR2=Do we not then owe it to the Mutant Savior in this time, long or short, to |1=do something beautiful?";

sermons.ninth = "Members of the human family who decide to take up the two 8-ways must remain vigilant against those who would try to lead them to the |2=ninth way\
              -- ^500|1=the ninth position|^500 -- |2=neutrality|^500 -- |1=nothing|^500 -- |2=pacifism.\
              |BRFor though the vigilant may dedicate him/herself to |1=error,| may resign him/herself to |1=futility,| \
              and even though he/she may take up the two 8-ways to hasten into the world the knowledge the Mutant Savior will need to fulfill the prophecies, \
              if the two 8-ways are left in the neutral position all will be for naught!\
              The ninth-positioners will say \"war is not the answer.\" \
              And the ninth-positioners will be right. \
              War with the Robotrons is not |1=the answer.| \
              |2=What the ninth-positioners will fail to say is that there is |1=no one great answer. \
              |BRWar against the Robotrons is |1=AN answer| and a much better answer than having the Mutant Savior |2=neither move nor fire his anti-robot, chest-mounted, turreted laser.\
              |BROn this topic |1=Eugene Jarvis|^500 -- Grand Designer of Robotron |1=2084| and likely author of the |1=First Writings| -- had this to say in a 1997 interview:\
              |BR\"It was fun for about fifteen minutes, running the robots into electrodes. But pacifism has its limits. ^500Gandhi the video game would have to wait.\"\
              |BR1=The ninth position has its place.\
              |BR2=Sometimes by not firing the turreted laser the Mutant Savior will be able to save greater numbers of the last human family and occasionally,^500\
              by sitting in a corner, ^500motionless, ^500the Mutant Savior might be able to more efficiently pick off swarms of |1=Robotrons.\
              |BRThese however, are the exceptions. Use the ninth position |1=sparingly; |use it |1=wisely.";

