const cp=require("child_process");
const fs=require("fs");

for(var i=1;i<5;i++){
	cp.exec("cmd /c ffprobe -v quiet -print_format json -show_format -show_streams data/"+i+".wav > data/test"+i+".json");
}
fs.read