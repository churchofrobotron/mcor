var fs = require("fs");

files = fs.readdirSync("./data");

var numGameSummary = {};

console.log("var scoreTable = [");
for (i = 0; i < files.length; i++) {
  file = files[i];
  if (file.indexOf(".gif") != -1) {
    var fields = file.split("_");
    fields[2] = fields[2].replace(/T/g, " ");
    fields[3] = fields[3].replace(/\.gif/g, "");
    fields.splice(3, 0, file);
    console.log(fields);
    if (i != files.length - 1)
      console.log(",");
    if (numGameSummary[fields[4]] == null)
        numGameSummary[fields[4]] = 0;
    numGameSummary[fields[4]] = numGameSummary[fields[4]] + 1
  }
}
console.log("];");

var total = 0;
for (i in numGameSummary) 
  total += numGameSummary[i];
numGameSummary["TOTAL"] = total;
numGameSummary["DIODE_TOTAL"] = numGameSummary["DIODE"] + numGameSummary["WINDOW"] + numGameSummary["VR"];
console.log("var summaryTable = ");
console.log(numGameSummary);
console.log(";");

