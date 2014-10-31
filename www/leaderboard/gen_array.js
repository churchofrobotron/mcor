var fs = require("fs");

files = fs.readdirSync("./data");

var numGameSummary = {};
var scores = [];

for (i = 0; i < files.length; i++) {
  file = files[i];
  if (file.indexOf(".gif") != -1) {
    var fields = file.split("_");
    fields[1] = Number(fields[1]);
    fields[2] = fields[2].replace(/T/g, " ");
    fields[3] = fields[3].replace(/\.gif/g, "");
    fields.splice(3, 0, file);
    scores.push(fields);
    if (numGameSummary[fields[4]] == null)
        numGameSummary[fields[4]] = 0;
    numGameSummary[fields[4]] = numGameSummary[fields[4]] + 1
  }
}

scores.sort(function(a, b) {
   var s1 = Number(a[1]);
   var s2 = Number(b[1]);
   if (s1>s2)
       return -1;
   if (s1<s2)
       return 1;
   return 0;
});

for (i = 0; i < scores.length; i++)
  scores[i].push(i+1);

console.log("var scoreTable = ");
console.log(scores);
console.log(";");

var total = 0;
for (i in numGameSummary) 
  total += numGameSummary[i];
numGameSummary["TOTAL"] = total;
numGameSummary["DIODE_TOTAL"] = numGameSummary["DIODE"] + numGameSummary["WINDOW"] + numGameSummary["VR"];
console.log("var summaryTable = ");
console.log(numGameSummary);
console.log(";");

