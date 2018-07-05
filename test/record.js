const audio=require("audio");
const opt={channels:2,format:8,rate:44100,frames:44100};
var ais=new audio.InputStream(opt);
const fs=require("fs");
var gsize=0;
ais.on("data",function(data){
    if(gsize){
    	fs.appendFile("out.pcm",data);
    }else{
    	console.log("rewrite!!!!");
    	fs.writeFile("out.pcm",data);
    }
    console.log(gsize+=data.length);
});
ais.on("done",function(){
    ais.destroy();
    console.log("record done");
})
ais.record(3);
