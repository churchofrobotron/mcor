var fs = require("fs");

files = fs.readdirSync("./data");

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
  }
}
console.log("];");