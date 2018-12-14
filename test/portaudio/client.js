var net=require("net");
var util=require("util");
var pack=require("pack");
var pcm=require("./pcm.js");
const fs=require("fs");


var pos=new pack.outputStream();
var pis=new pack.inputStream();
var c;

pos.on("data",function(data){
    switch(data.act){
    case "record":
	pcm.record(data.path,data.sec,function(){
	    console.log("record done");
	    pis.push({act:"record",sts:"ok"});
	})
	break;
    case "play":
	pcm.play(data.path,function(){
	    console.log("play done");
	    pis.push({act:"play",sts:"ok"});
	})
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
