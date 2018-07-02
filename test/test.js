const audio=require("audio");
const opt={channels:2,format:8,rate:44100,frames:512,seconds:1};
var ais=new audio.InputStream(opt);

console.log(typeof ais);
ais.on("data",function(data){
    console.log(data.length,data);
});
ais.record(10);
