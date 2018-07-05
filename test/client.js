var net=require("net");
var util=require("util");
var pack=require("pack");
var audio=require("audio");
const fs=require("fs");


var pos=new pack.outputStream();
var pis=new pack.inputStream();
var c;
const opt={channels:2,format:8,rate:44100,frames:44100};
var ais=new audio.InputStream(opt);
var aos=new audio.OutputStream(opt);
var gsize=0;
var path="default.pcm";

aos.on("done",function(){
    console.log("play done");
    pis.push({act:"play",sts:"ok"});
})

ais.on("data",function(data){
    if(gsize){
    	fs.appendFile(path,data);
    }else{
    	console.log("rewrite!!!!");
    	fs.writeFile(path,data);
    }
    gsize=1;
});
ais.on("done",function(){
    gsize=0;
    console.log("record done");
    pis.push({act:"record",sts:"ok"});
})

pos.on("data",function(data){
    switch(data.act){
    case "record":
	path=data.path;
	ais.record(data.sec);
	break;
    case "play":
	fs.readFile(data.path,function(ev,data){
	    aos.push(data);
	});
	break;
    }
});
pis.on("data",function(data){
    c.write(data);
});

module.exports.test=function(){    
    c=net.connect(8009,"127.0.0.1",function(){
	c.on("data",function(data){
	    pos.push(data);
	});
    });
}
