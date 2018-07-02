var audio = require("audio");
const fs=require("fs");
const opt={channels:2,format:audio.int16,rate:44100,frames:512,seconds:2};

const ais=new audio.InputStream(opt);
const aos=new audio.OutputStream(opt);
ais.on("data",function(data){
    console.log(data);
    aos.push(data);
})
aos.on("done",function(){
    console.log("play");
    ais.resume();
});
console.log("test Audio!!!");