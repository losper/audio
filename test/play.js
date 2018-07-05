const audio=require("audio");
const opt={channels:2,format:8,rate:44100,frames:44100};
var aos=new audio.OutputStream(opt);
const fs=require("fs");
fs.readFile("out.pcm",function(ev,data){
    aos.push(data);
});
aos.on("done",function(){
    aos.destroy();
    console.log("play done");
})
