var audio = require("../binary/Debug/audiostream.js");
const fs=require("fs");
const opt={channels:2,format:audio.int16,rate:44100,frames:512,seconds:2};

// const aps=new audio.PlayStream(arg)
// const ars=new audio.RecordStream(arg,function(ev,data){
//     aps.push(data,function(){
// 	ars.resume();
//     });
// })
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
