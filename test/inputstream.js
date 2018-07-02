var audio = require("audio");
const fs=require("fs");
const opt={channels:2,format:audio.int16,rate:44100,frames:512,seconds:2};

const ais=new audio.InputStream(opt);
ais.on("data",function(data){
    fs.appendFile("test.pcm",data);
})
ais.on("done",function(){});
ais.record(10);
ais.record(5);
console.log("test Audio!!!");
